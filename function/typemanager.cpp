#include "typemanager.h"

#include "function/network/client.h"
#include "function/network/protocal.h"
#include "function/metacategory.h"

TypeManager::TypeManager(AssetType type, QObject *parent) : QObject(parent), m_type(type), m_types(new QJsonObject())
{
    m_client = new Client(this);

    connect(m_client, &Client::onSendingStateChanged, this, &TypeManager::onResponsing);

    connect(m_client, &Client::onReadOver, this, [=](const QJsonObject &data, const QByteArray &extraData) {
        auto response_type = static_cast<Protocal::HeaderField>(data["type"].toInt());
        auto status = data["status"].toString();
        if (Protocal::HeaderField::RESPONSE_ERROR == response_type) {
            emit onResponsing(tr("服务请求错误. Info:") + status, false);
        } else {
            if (!status.isEmpty()) {
                emit onResponsing(tr("类型拉取失败. Info: ") + status, false);
            } else {
                if (Protocal::HeaderField::RESPONSE_PULLTYPE == response_type) {
                    emit onResponsing(tr("类型拉取成功!"), false);
                    setTypesData(data);
                    emit onPullSuccessful();
                } else if (Protocal::HeaderField::RESPONSE_PUSHTYPE == response_type) {
                    emit onResponsing(tr("类型推送成功!"), false);
                    emit onPushSuccessful();
                }
            }
        }
    });

}

TypeManager::~TypeManager()
{
    delete m_types;
}

void TypeManager::pull()
{
    emit onResponsing(tr("开始拉取类型信息"), true);
    QJsonObject data = {{
        {"type",       static_cast<int>(Protocal::HeaderField::REQUEST_PULLTYPE)},
        {"category",   MetaCategory::getInstance()->getCategoryInt()},
        {"assetType",  static_cast<int>(m_type)},
        {"headers",    QJsonArray() << QJsonObject()},
        {"data",       QJsonArray() << QJsonObject()}
    }};
    m_client->sendData(data);
}

void TypeManager::push()
{
    emit onResponsing(tr("开始推送类型信息"), true);
    QJsonObject pushData = {{
        {"type",       static_cast<int>(Protocal::HeaderField::REQUEST_PUSHTYPE)},
        {"category",   MetaCategory::getInstance()->getCategoryInt()},
        {"assetType", static_cast<int>(m_type)},
        {"headers",    QJsonArray() << res::AssetInfo::toHeaderElement("name", false, false, false)},
        {"data",    (*m_types)["data"]}
    }};
    m_client->sendData(pushData);
}

void TypeManager::add(const QString &typeName)
{
    auto typeNameStd = typeName.toStdString();
    if (m_nameSet.find(typeNameStd) != m_nameSet.end()) {
        emit onAddRepeat(typeName);
        return;
    }
    m_nameSet.insert(typeNameStd);
    auto data = (*m_types)["data"].toArray();
    data << QJsonObject{{"name", typeName}};
    (*m_types)["data"] = data;
    emit onAddSuccessed();
}

void TypeManager::remove(int index)
{
    auto data = (*m_types)["data"].toArray();
    auto typeNameStd = data[index].toObject()["name"].toString().toStdString();
    m_nameSet.erase(m_nameSet.find(typeNameStd));
    data.removeAt(index);
    (*m_types)["data"] = data;
    emit onRemoveSuccessed();
}

QString TypeManager::type(int index)
{
    auto data = (*m_types)["data"].toArray();
    return data[index].toObject()["name"].toString();
}

QJsonArray TypeManager::getTypesNameList()
{
    QJsonArray ret;
    auto data = (*m_types)["data"].toArray();
    for (const auto &d : data) {
        ret << d.toObject()["name"];
    }
    return ret;
}

void TypeManager::setTypesData(const QJsonObject &data)
{
    auto headerName = "name";
    (*m_types) = QJsonObject{{{"headers",
                               QJsonArray() << QJsonObject{
                                   {"name", headerName},
                                   {"editable", true},
                                   {"is_array", false},
                                   {"visible", true},
                               }},
                              {"data", data["data"]}}};
    auto types = data["data"].toArray();
    m_nameSet.clear();
    for (const auto &t : types) {
        m_nameSet.insert(t.toObject()[headerName].toString().toStdString());
    }
}
