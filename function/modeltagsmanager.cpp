#include "modeltagsmanager.h"

#include "function/network/client.h"
#include "function/network/protocal.h"
#include "function/metacategory.h"

ModelTagsManager::ModelTagsManager(AssetType type, QObject *parent) : QObject(parent), m_type(type), m_tags(new QJsonObject())
{
    m_client = new Client(this);

    connect(m_client, &Client::onSendingStateChanged, this, &ModelTagsManager::onResponsing);

    connect(m_client, &Client::onReadOver, this, [=](const QJsonObject &data, const QByteArray &extraData) {
        auto response_type = static_cast<Protocal::HeaderField>(data["type"].toInt());
        auto status = data["status"].toString();
        if (Protocal::HeaderField::RESPONSE_ERROR == response_type) {
            emit onResponsing(tr("服务请求错误. Info:") + status, false);
        } else {
            if (!status.isEmpty()) {
                emit onResponsing(tr("标签拉取失败. Info: ") + status, false);
            } else {
                emit onResponsing(tr("标签拉取成功!"), false);
                setTagsData(data);
                emit onPullSuccessful();
            }
        }
    });

}

ModelTagsManager::~ModelTagsManager()
{
    delete m_tags;
}

void ModelTagsManager::pull()
{
    emit onResponsing(tr("开始拉取标签信息"), true);
    QJsonObject data = {{
        {"type",       static_cast<int>(Protocal::HeaderField::REQUEST_PULLTAGS)},
        {"category",   MetaCategory::getInstance()->getCategoryInt()},
        {"headers",    QJsonArray() << res::AssetInfo::toHeaderElement("assetType", true, false, true)},
        {"data",       QJsonArray() << QJsonObject({{"assetType", static_cast<int>(m_type)}})}
    }};
    m_client->sendData(data);
}

QJsonArray ModelTagsManager::getTagsNameList()
{
    QJsonArray ret;
    auto data = (*m_tags)["data"].toArray();
    auto headerName = (*m_tags)["headers"].toArray()[0].toObject()["name"].toString();
    for (const auto &d : data) {
        ret << d.toObject()[headerName];
    }
    return ret;
}

void ModelTagsManager::setTagsData(const QJsonObject &data)
{
    auto headerName = data["headers"].toArray()[0].toString();
    (*m_tags) = QJsonObject{{{"headers",
                               QJsonArray() << QJsonObject{
                                                           {"name", headerName},
                                                           {"editable", true},
                                                           {"is_array", false},
                                                           {"visible", true},
                                                           }},
                              {"data", data["data"]}}};
}
