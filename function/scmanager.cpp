#include "scmanager.h"

#include "function/network/client.h"
#include "function/network/protocal.h"
#include "function/metacategory.h"
#include "resource/assetinfo.h"

SCManager::SCManager(QObject *parent)
    : QObject{parent}, m_scInfo(new QJsonObject())
{
    m_client = new Client(this);

    connect(m_client, &Client::onSendingStateChanged, this, &SCManager::onResponsing);

    connect(m_client, &Client::onReadOver, this, [=](const QJsonObject &data, const QByteArray &extraData) {
        auto response_type = static_cast<Protocal::HeaderField>(data["type"].toInt());
        auto status = data["status"].toString();
        if (Protocal::HeaderField::RESPONSE_ERROR == response_type) {
            emit onResponsing(tr("服务请求错误. Info:") + status, false);
        } else {
            if (Protocal::HeaderField::RESPONSE_PULLSC == response_type && !status.isEmpty()) {
                emit onResponsing(tr("标准分类拉取失败. Info: ") + status, false);
            } else if (Protocal::HeaderField::RESPONSE_PUSHSC == response_type && !status.isEmpty()) {
                emit onResponsing(tr("标准分类推送失败. Info: ") + status, false);
            }
            else {
                if (Protocal::HeaderField::RESPONSE_PULLSC == response_type) {
                    emit onResponsing(tr("标准分类拉取成功!"), false);
                    setScData(data);
                    emit onPullSuccessful();
                } else if (Protocal::HeaderField::RESPONSE_PUSHSC == response_type) {
                    emit onResponsing(tr("标准分类推送成功"), false);
                    emit onPushSuccessful();
                }
            }
        }
    });

}

SCManager::~SCManager()
{
    delete m_scInfo;
}

void SCManager::pull()
{
    emit onResponsing(tr("开始获取标准分类信息"), true);
    QJsonObject data = {{
        {"type",       static_cast<int>(Protocal::HeaderField::REQUEST_SC)},
        {"scType", static_cast<int>(Protocal::HeaderField::REQUEST_PULLSC)},
        {"category",   MetaCategory::getInstance()->getCategoryInt()},
        {"headers",    QJsonArray() << QJsonObject()},
        {"data",       QJsonArray() << QJsonObject()}
    }};
    m_client->sendData(data);
}

void SCManager::push()
{
    emit onResponsing(tr("开始推送标准分类信息"), true);
    QJsonObject data = {{
        {"type",       static_cast<int>(Protocal::HeaderField::REQUEST_SC)},
        {"scType", static_cast<int>(Protocal::HeaderField::REQUEST_PUSHSC)},
        {"category",   MetaCategory::getInstance()->getCategoryInt()},
        {"headers",    QJsonArray() << res::AssetInfo::toHeaderElement("name", false, false, false)},
        {"data",       (*m_scInfo)["data"]}
    }};
    m_client->sendData(data);
}

void SCManager::add(const QString &scName)
{
    auto scNameStd = scName.toStdString();
    if (m_nameSet.find(scNameStd) != m_nameSet.end()) {
        emit onAddRepeat(scName);
        return;
    }
    m_nameSet.insert(scNameStd);
    auto data = (*m_scInfo)["data"].toArray();
    data << QJsonObject{{"name", scName}};
    (*m_scInfo)["data"] = data;
    emit onAddSuccessed();
}

void SCManager::remove(int index)
{
    auto data = (*m_scInfo)["data"].toArray();
    auto scNameStd = data[index].toObject()["name"].toString().toStdString();
    m_nameSet.erase(m_nameSet.find(scNameStd));
    data.removeAt(index);
    (*m_scInfo)["data"] = data;
    emit onRemoveSuccessed();
}

void SCManager::modify(int index, const QString &name)
{
    auto newNameStd = name.toStdString();
    if (m_nameSet.find(newNameStd) != m_nameSet.end()) {
        emit onModifyRepeat(name);
        return;
    }
    // add new in set
    m_nameSet.insert(newNameStd);
    auto data = (*m_scInfo)["data"].toArray();
    auto oldNameStd = data[index].toObject()["name"].toString().toStdString();
    // remove old in set
    m_nameSet.erase(m_nameSet.find(oldNameStd));
    // modify name in data
    auto newObj = data[index].toObject();
    newObj["name"] = name;
    data[index] = newObj;
    (*m_scInfo)["data"] = data;
    emit onModifySuccessed();
}

QString SCManager::sc(int index) const
{
    auto data = (*m_scInfo)["data"].toArray();
    return data[index].toObject()["name"].toString();
}

QJsonArray SCManager::getSCNameList()
{
    QJsonArray ret;
    auto data = (*m_scInfo)["data"].toArray();
    for (const auto &d : data) {
        ret << d.toObject()["name"];
    }
    return ret;
}

void SCManager::setScData(const QJsonObject &data)
{
    auto headerName = "name";
    (*m_scInfo) = QJsonObject{{{"headers",
                              QJsonArray() << QJsonObject{
                                                          {"name", headerName},
                                                          {"editable", true},
                                                          {"is_array", false},
                                                          {"visible", true},
                                                          }},
                             {"data", data["data"]}}};
    auto SCs = data["data"].toArray();
    m_nameSet.clear();
    for (const auto &s : SCs) {
        m_nameSet.insert(s.toObject()[headerName].toString().toStdString());
    }
}
