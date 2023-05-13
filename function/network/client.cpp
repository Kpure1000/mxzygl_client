#include "client.h"

#include <QDebug>
#include <QTimerEvent>

#include "utils/jobsystem.h"
#include "function/configer/configmanager.h"
#include "protocal.h"

Client::Client(QObject *parent) : QObject(parent)
{
    m_socket = new QTcpSocket(this);
    auto server_info_raw = ConfigManager::getInstance()->getConfigs({"Server/Addr", "Server/Port"});
    m_host = server_info_raw[0].toString();
    // TODO: port的配置最好都改成uint
    m_port = server_info_raw[1].toInt();
    connect(ConfigManager::getInstance(), &ConfigManager::onConfModified, this, [=](){
        auto server_info_raw = ConfigManager::getInstance()->getConfigs({"Server/Addr", "Server/Port"});
        m_host = server_info_raw[0].toString();
        m_port =server_info_raw[1].toInt();
    });

//    connect(m_socket, &QTcpSocket::stateChanged, this, [=](QAbstractSocket::SocketState state) {
//        qDebug() << "QTcpSocket::stateChanged>>" << state << "at port[" << m_socket->localPort() << "]";
//    }, Qt::QueuedConnection);

    connect(m_socket, &QTcpSocket::connected, this, [=]() {
        emit onConnectSuccessful();
    }, Qt::QueuedConnection);

    connect(m_socket,  QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error), this, [=](QAbstractSocket::SocketError error) {
        emit onConnectError(error, m_socket->errorString());
    }, Qt::QueuedConnection);

    connect(m_socket, &QTcpSocket::readyRead, this, [=](){
        emit onReadReady();
    });

    connect(this, &Client::onConnectSuccessful, this, [=]() {
        emit onSendingStateChanged(tr("服务器连接成功, 发送数据..."), true);
    });
    connect(this, &Client::onConnectError, this, [=](QAbstractSocket::SocketError, const QString &info) {
        emit onSendingStateChanged(tr("与服务器断开连接, 信息: ") + info, false);
    });
    connect(this, &Client::onSendSuccessful, this, [=]() {
        emit onSendingStateChanged(tr("数据发送成功, 接受服务响应..."), true);
    });
    connect(this, &Client::onSendFailed, this, [=]() {
        emit onSendingStateChanged(tr("数据发送失败"), false);
    });
    connect(this, &Client::onReadReady, this, [=]() {
        emit onSendingStateChanged(tr("正在接受服务响应..."), true);
    });
    connect(this, &Client::onReadOver, this, [=](const QJsonObject &data, const QByteArray &extraData) {
        emit onSendingStateChanged(tr("服务响应接受成功"), true);
    });

}

Client::~Client()
{
    m_socket->close();
//    qDebug() << "Client::~Client";
}

void Client::sendData(const QJsonObject &data, const QByteArray &extraData)
{

    if (QTcpSocket::UnconnectedState != m_socket->state()) {
        qDebug() << "Client::sendData>> 还没发完别急!!!!!!!!!!";
        return;
    }

    ClientSession *session = new ClientSession(this);

    connect(this, &Client::onConnectSuccessful, session, [=]() {
        doSendData(data, extraData);
    });

    connect(this, &Client::onConnectError, session, [=](QAbstractSocket::SocketError, const QString &info) {
        qDebug() << "Client::sendData>>Client::onConnectedFailed>>" << info;
        session->deleteLater();
    });

    connect(this, &Client::onSendFailed, session, [=](){
        qDebug() << "Client::onSendFailed";
        m_socket->disconnectFromHost();
        session->deleteLater();
    });

    connect(this, &Client::onReadReady, session, [=](){
        doReceiveData(session);
    });

    connect(this, &Client::onReadOver, session, [=](){
        session->deleteLater();
    });

    emit onSendingStateChanged(tr("正在连接服务器..."), true);
    m_socket->connectToHost(m_host, m_port);

}

void Client::doSendData(const QJsonObject &data, const QByteArray &extraData)
{
    auto packed = Protocal::pack(data);
    QByteArray json_bytes = QJsonDocument(packed).toJson();

    // 先发送数据长度
    uint data_len = json_bytes.size(), extra_len = extraData.size();
    m_socket->write(Protocal::toByteArray(data_len) + Protocal::toByteArray(extra_len));

    // 再发送真正的数据
    uint len_sended = 0, len = data_len + extra_len;

//    qDebug() << "length to send" << len;

    while (len_sended < len) {
        int ret = -1;

        if (len_sended < data_len) {
            ret = m_socket->write(json_bytes.data() + len_sended, data_len - len_sended);
        } else {
            ret = m_socket->write(extraData.data() + len_sended - data_len, len - len_sended);
        }

        if (-1 == ret) {
            emit onSendFailed();
            return;
        }

        len_sended += ret;

//        qDebug() << "length sended" << len;

        m_socket->flush();
    }

    emit onSendSuccessful();
}

void Client::doReceiveData(ClientSession *session)
{
    if (!session->head_received) // 首先读取数据长度信息
    {
        if (m_socket->bytesAvailable() >= 8) // 不能小于头字节数
        {
            // 读取头信息
            QByteArray data_len_b, extra_len_b;
            data_len_b = m_socket->read(4);
            extra_len_b = m_socket->read(4);

            session->data_len = Protocal::fromByteArray<uint>(data_len_b);
            session->extra_len = Protocal::fromByteArray<uint>(extra_len_b);

//            qDebug() << "length to receive " << session->data_len << session->extra_len;

            if (0 == session->data_len && 0 == session->extra_len) {
                // 没有后续数据
                return;
            }

            // 读取头信息并标注头信息已经被读取
            session->head_received = true;
        } else {
            return;
        }
    }

    // 读取实际的数据
    if (session->data.size() < session->data_len) {
        session->data.append(m_socket->read(session->data_len - session->data.size()));
    } else if (session->extra.size() < session->extra_len) {
        session->extra.append(m_socket->read(session->extra_len - session->extra.size()));
    }

//    qDebug() << "length received " << session->data.size() << session->extra.size();

    // 如果没有读完的话，还会再次进入槽函数
    if (session->data.size() < session->data_len || session->extra.size() < session->extra_len)
        return;

    auto data_obj = QJsonDocument::fromJson(session->data).object();
    auto data = Protocal::unpack(data_obj);
    emit onReadOver(data, session->extra);
}
