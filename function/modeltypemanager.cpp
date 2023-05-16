#include "modeltypemanager.h"

#include "function/network/client.h"
#include "function/network/protocal.h"
#include "function/metacategory.h"

ModelTypeManager::ModelTypeManager(AssetType type, QObject *parent) : QObject(parent), m_type(type), m_types(new QJsonObject())
{
    m_client = new Client(this);

    connect(m_client, &Client::onSendingStateChanged, this, &ModelTypeManager::onResponsing);

    connect(m_client, &Client::onReadOver, this, [=](const QJsonObject &data, const QByteArray &extraData) {
        auto response_type = static_cast<Protocal::HeaderField>(data["type"].toInt());
        auto status = data["status"].toString();
        if (Protocal::HeaderField::RESPONSE_ERROR == response_type) {
            emit onResponsing(tr("服务请求错误. Info:") + status, false);
        } else {
            if (!status.isEmpty()) {
                emit onResponsing(tr("类型拉取失败. Info: ") + status, false);
            } else {
                emit onResponsing(tr("类型拉取成功!"), false);
                setTypesData(data);
                emit onPullSuccessful();
            }
        }
    });

}

ModelTypeManager::~ModelTypeManager()
{
    delete m_types;
}

void ModelTypeManager::pull()
{
    emit onResponsing(tr("开始拉取类型信息"), true);
    QJsonObject data = {{
        {"type",       static_cast<int>(Protocal::HeaderField::REQUEST_PULLTYPE)},
        {"category",   MetaCategory::getInstance()->getCategoryInt()},
        {"headers",    QJsonArray() << res::AssetInfo::toHeaderElement("assetType", true, false, true)},
        {"data",       QJsonArray() << QJsonObject({{"assetType", static_cast<int>(m_type)}})}
    }};
    m_client->sendData(data);
}

QJsonArray ModelTypeManager::getTypesNameList()
{
    QJsonArray ret;
    auto data = (*m_types)["data"].toArray();
    auto headerName = (*m_types)["headers"].toArray()[0].toObject()["name"].toString();
    for (const auto &d : data) {
        ret << d.toObject()[headerName];
    }
    return ret;
}

void ModelTypeManager::setTypesData(const QJsonObject &data)
{
    auto headerName = data["headers"].toArray()[0].toString();
    (*m_types) = QJsonObject{{{"headers",
                               QJsonArray() << QJsonObject{
                                   {"name", headerName},
                                   {"editable", true},
                                   {"is_array", false},
                                   {"visible", true},
                               }},
                              {"data", data["data"]}}};
}
