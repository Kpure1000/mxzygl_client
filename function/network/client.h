#ifndef MX_CLIENT_H
#define MX_CLIENT_H

#include <QByteArray>
#include <QObject>
#include <QJsonDocument>
#include <QTcpSocket>

class ClientSession : public QObject {
    Q_OBJECT
public:
    explicit ClientSession(QObject *p) : QObject(p) {}
    bool head_received = false;
    int data_len = 0, extra_len = 0;
    QByteArray data, extra;
};

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

    ~Client();

    void sendData(const QJsonObject &data, const QByteArray &extraData = QByteArray{});

signals:
    void onConnectSuccessful();
    void onConnectError(QAbstractSocket::SocketError, const QString &info);

    void onSendSuccessful();
    void onSendFailed();

    void onReadReady();
    void onReadOver(const QJsonObject &data, const QByteArray &extraData);

    void onConnectingTimeOut(int timerID, int msec);

    void onSendingStateChanged(const QString &info, bool is_continue);

private slots:
    void doSendData(const QJsonObject &data, const QByteArray &extraData);
    void doReceiveData(ClientSession *session);

private:
    QTcpSocket *m_socket;
    QString m_host;
    int m_port;
};

#endif // MX_CLIENT_H
