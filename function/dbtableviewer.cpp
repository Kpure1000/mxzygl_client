#include "dbtableviewer.h"

#include "function/network/client.h"
#include "function/network/protocal.h"

DBTableViewer::DBTableViewer(QObject *parent) : QObject(parent), m_info(new QJsonObject())
{
    m_client = new Client(this);

    connect(m_client, &Client::onSendingStateChanged, this, &DBTableViewer::onResponsing);

    connect(m_client, &Client::onReadOver, this, [=](const QJsonObject &data, const QByteArray &extraData) {
        auto response_type = static_cast<Protocal::HeaderField>(data["type"].toInt());
        auto status = data["status"].toString();
        if (Protocal::HeaderField::RESPONSE_ERROR == response_type) {
            emit onResponsing(tr("服务请求错误. Info:") + status, false);
        } else {
            if (!status.isEmpty()) {
                emit onResponsing(tr("数据表拉取失败. Info: ") + status, false);
            } else {
                if (Protocal::HeaderField::RESPONSE_SHOWTBMODEL == response_type) {
                    emit onResponsing(tr("数据表拉取成功!"), false);
                    setInfoData(data);
                    emit onTBPullSuccessful();
                }
            }
        }
    });

}

DBTableViewer::~DBTableViewer()
{
    delete m_info;
}

void DBTableViewer::pullTBInfo()
{
    emit onResponsing(tr("开始拉取数据表信息"), true);
    QJsonObject data = {{
        {"type",       static_cast<int>(Protocal::HeaderField::REQUEST_SHOWTBMODEL)},
        {"headers",    QJsonArray() << QJsonObject()},
        {"data",       QJsonArray() << QJsonObject()}
    }};
    m_client->sendData(data);
}

void DBTableViewer::setInfoData(const QJsonObject &data)
{
    QJsonArray headers;
    auto org_h = data["headers"].toArray();
    for (const auto&h : org_h) {
        headers << QJsonObject{
            {"name", h.toString()},
            {"editable", false},
            {"is_array", false},
            {"visible",  true}
        };
    }
    (*m_info)["headers"] = headers;
    (*m_info)["data"] = data["data"];
//    qDebug() << (*m_info);
}
