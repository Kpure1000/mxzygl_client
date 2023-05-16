#include "tagsmanager.h"

#include "function/network/client.h"
#include "function/network/protocal.h"
#include "function/metacategory.h"

TagsManager::TagsManager(AssetType type, QObject *parent) : QObject(parent), m_type(type), m_tags(new QJsonObject())
{
    m_client = new Client(this);

    connect(m_client, &Client::onSendingStateChanged, this, &TagsManager::onResponsing);

    connect(m_client, &Client::onReadOver, this, [=](const QJsonObject &data, const QByteArray &extraData) {
        auto response_type = static_cast<Protocal::HeaderField>(data["type"].toInt());
        auto status = data["status"].toString();
        if (Protocal::HeaderField::RESPONSE_ERROR == response_type) {
            emit onResponsing(tr("服务请求错误. Info:") + status, false);
        } else {
            if (!status.isEmpty()) {
                emit onResponsing(tr("标签拉取失败. Info: ") + status, false);
            } else {
                if (Protocal::HeaderField::RESPONSE_PULLTAGS == response_type) {
                    emit onResponsing(tr("标签拉取成功!"), false);
                    setTagsData(data);
                    emit onPullSuccessful();
                } else if (Protocal::HeaderField::RESPONSE_PUSHTAGS == response_type) {
                    emit onResponsing(tr("标签推送成功"), false);
                    emit onPushSuccessful();
                }
            }
        }
    });

}

TagsManager::~TagsManager()
{
    delete m_tags;
}

void TagsManager::pull()
{
    emit onResponsing(tr("开始拉取标签信息"), true);
    QJsonObject data = {{
        {"type",       static_cast<int>(Protocal::HeaderField::REQUEST_PULLTAGS)},
        {"category",   MetaCategory::getInstance()->getCategoryInt()},
        {"assetType",  static_cast<int>(m_type)},
        {"headers",    QJsonArray() << QJsonObject()},
        {"data",       QJsonArray() << QJsonObject()}
    }};
    m_client->sendData(data);
}

void TagsManager::push()
{
    emit onResponsing(tr("开始推送标签信息"), true);
    QJsonObject pushData = {{
        {"type",       static_cast<int>(Protocal::HeaderField::REQUEST_PUSHTAGS)},
        {"category",   MetaCategory::getInstance()->getCategoryInt()},
        {"assetType", static_cast<int>(m_type)},
        {"headers",    QJsonArray() << res::AssetInfo::toHeaderElement("name", false, false, false)},
        {"data",    (*m_tags)["data"]}
    }};
    m_client->sendData(pushData);
}

void TagsManager::add(const QString &tagName)
{
    auto tagNameStd = tagName.toStdString();
    if (m_nameSet.find(tagNameStd) != m_nameSet.end()) {
        emit onAddRepeat(tagName);
        return;
    }
    m_nameSet.insert(tagNameStd);
    auto data = (*m_tags)["data"].toArray();
    data << QJsonObject{{"name", tagName}};
    (*m_tags)["data"] = data;
    emit onAddSuccessed();
}

void TagsManager::remove(int index)
{
    auto data = (*m_tags)["data"].toArray();
    auto tagNameStd = data[index].toObject()["name"].toString().toStdString();
    m_nameSet.erase(m_nameSet.find(tagNameStd));
    data.removeAt(index);
    (*m_tags)["data"] = data;
    emit onRemoveSuccessed();
}

QString TagsManager::tags(int index)
{
    auto data = (*m_tags)["data"].toArray();
    return data[index].toObject()["name"].toString();
}

QJsonArray TagsManager::getTagsNameList()
{
    QJsonArray ret;
    auto data = (*m_tags)["data"].toArray();
    for (const auto &d : data) {
        ret << d.toObject()["name"];
    }
    return ret;
}

void TagsManager::setTagsData(const QJsonObject &data)
{
    auto headerName = "name";
    (*m_tags) = QJsonObject{{{"headers",
                               QJsonArray() << QJsonObject{
                                                           {"name", headerName},
                                                           {"editable", true},
                                                           {"is_array", false},
                                                           {"visible", true},
                                                           }},
                              {"data", data["data"]}}};
    auto tags = data["data"].toArray();
    m_nameSet.clear();
    for (const auto &t : tags) {
        m_nameSet.insert(t.toObject()[headerName].toString().toStdString());
    }
}
