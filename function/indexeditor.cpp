#include "indexeditor.h"

#include "function/network/client.h"
#include "function/network/protocal.h"
#include "function/metacategory.h"

IndexEditor::IndexEditor(IndexType type, QObject *parent)
    : QObject{parent}, m_type(type), m_org_info{new QJsonObject()}, m_index_info{new QJsonObject()}
{
    m_client = new Client(this);

    connect(m_client, &Client::onSendingStateChanged, this, &IndexEditor::onResponsing);

    connect(m_client, &Client::onReadOver, this, [=](const QJsonObject &data, const QByteArray &extraData) {
        auto response_type = static_cast<Protocal::HeaderField>(data["type"].toInt());
        auto status = data["status"].toString();
        if (Protocal::HeaderField::RESPONSE_ERROR == response_type) {
            emit onResponsing(tr("服务请求错误. Info:") + status, false);
        } else {
            if (!status.isEmpty()) {
                emit onResponsing(tr("数据拉取失败. Info: ") + status, false);
            } else {
                if (Protocal::HeaderField::RESPONSE_INDEXPULL == response_type) {
                    emit onResponsing(tr("数据拉取成功!"), false);
                    setOrgData(data);
                    emit onPullSuccessful();
                } else if (Protocal::HeaderField::RESPONSE_INDEXPUSH == response_type) {
                    emit onResponsing(tr("数据推送成功"), false);
                    emit onPushSuccessful();
                }
            }
        }
    });

}

IndexEditor::~IndexEditor()
{
    delete m_org_info;
    delete m_index_info;
}

void IndexEditor::pull_org()
{
    int indexType = 0;
    switch (m_type) {
    case IndexEditor::IndexType::CONTENT: indexType = static_cast<int>(Protocal::HeaderField::REQUEST_MODELINDEX); break;
    case IndexEditor::IndexType::TYPE: indexType = static_cast<int>(Protocal::HeaderField::REQUEST_TYPEINDEX); break;
    case IndexEditor::IndexType::TAGS: indexType = static_cast<int>(Protocal::HeaderField::REQUEST_LABELINDEX); break;
    }
    QJsonObject send = {{
        {"type", static_cast<int>(Protocal::HeaderField::REQUEST_INDEXPULL)},
        {"category",   MetaCategory::getInstance()->getCategoryInt()},
        {"indexType", static_cast<int>(indexType)},
        {"headers",    QJsonArray() << QJsonObject()},
        {"data",       QJsonArray() << QJsonObject()}
    }};
    m_client->sendData(send);
}

void IndexEditor::compressToIndex()
{
    std::unordered_map<std::string, std::string> index_map;
    QJsonArray index_headers;
    QJsonArray index_data;
    if (IndexEditor::IndexType::CONTENT == m_type) {
        index_headers << QJsonObject{
            {"name", "content"},
            {"editable", true},
            {"is_array", false},
            {"visible",  true}
        };
        for (const auto &d : (*m_org_info)["data"].toArray()) {
            const auto &row = d.toObject();
            if (!row["content"].toString().isEmpty() && index_map.find(row["content"].toString().toStdString()) == index_map.end()) {
                index_map.insert({row["content"].toString().toStdString(), ""});
                index_data << QJsonObject{{"content", row["content"]}};
            }
        }
    } else if (IndexEditor::IndexType::TYPE == m_type) {
        index_headers << QJsonObject{
            {"name", "type"},
            {"editable", true},
            {"is_array", false},
            {"visible",  true}
        };
        for (const auto &d : (*m_org_info)["data"].toArray()) {
            const auto &row = d.toObject();
            if (!row["type"].toString().isEmpty() && index_map.find(row["type"].toString().toStdString()) == index_map.end()) {
                index_map.insert({row["type"].toString().toStdString(), ""});
                index_data << QJsonObject{{"type", row["type"]}};
            }
        }
    } else if (IndexEditor::IndexType::TAGS == m_type) {
        index_headers << QJsonObject{
            {"name", "tags"},
            {"editable", true},
            {"is_array", false},
            {"visible",  true}
        };
        for (const auto &d : (*m_org_info)["data"].toArray()) {
            const auto &row = d.toObject();
            if (!row["tags"].toString().isEmpty() && index_map.find(row["tags"].toString().toStdString()) == index_map.end()) {
                index_map.insert({row["tags"].toString().toStdString(), ""});
                index_data << QJsonObject{{"tags", row["tags"]}};
            }
        }
    }
    (*m_index_info)["headers"] = index_headers;
    (*m_index_info)["data"] = index_data;
//    qDebug() << (*m_org_info);
//    qDebug() << (*m_index_info);
    emit onCompressed();
}

void IndexEditor::syncToOrg()
{
    emit onSyncToOrg();
}

void IndexEditor::push_org()
{
    int indexType = 0;
    QString keyName;
    switch (m_type) {
    case IndexEditor::IndexType::CONTENT:
        indexType = static_cast<int>(Protocal::HeaderField::REQUEST_MODELINDEX);
        keyName = "content";
        break;
    case IndexEditor::IndexType::TYPE:
        indexType = static_cast<int>(Protocal::HeaderField::REQUEST_TYPEINDEX);
        keyName = "type";
        break;
    case IndexEditor::IndexType::TAGS:
        indexType = static_cast<int>(Protocal::HeaderField::REQUEST_LABELINDEX);
        keyName = "tags";
        break;
    }
    QJsonObject send = {{
        {"type", static_cast<int>(Protocal::HeaderField::REQUEST_INDEXPUSH)},
        {"category",   MetaCategory::getInstance()->getCategoryInt()},
        {"indexType", static_cast<int>(indexType)},
        {"headers",    QJsonArray() << res::AssetInfo::toHeaderElement(keyName, false, false, false)},
        {"data",       (*m_index_info)["data"]}
    }};
    m_client->sendData(send);
}

QStringList IndexEditor::getFilePaths(const std::vector<int> &index) const
{
    QStringList ret;
    auto data = (*m_org_info)["data"].toArray();
    for (auto id : index)
    {
        auto row = data[id].toObject();
        ret << row["hash"].toString();
    }
//    qDebug() << ret;
    return ret;
}

QStringList IndexEditor::getPreviewInfo(const std::vector<int> &index) const
{
    QStringList ret;
    auto data = (*m_org_info)["data"].toArray();
    for (auto id : index) {
        auto row = data[id].toObject();
        ret << (row["name"].toString());
    }
    return ret;
}

QString IndexEditor::index(int row) const
{
    auto data = (*m_index_info)["data"].toArray();
    switch (m_type) {
    case IndexEditor::IndexType::CONTENT:
        return data[row].toObject()["content"].toString();
    case IndexEditor::IndexType::TAGS:
        return data[row].toObject()["tags"].toString();
    case IndexEditor::IndexType::TYPE:
        return data[row].toObject()["type"].toString();
    }
}

void IndexEditor::modify(int index, const QString &name)
{
    QString keyName;
    switch (m_type) {
    case IndexEditor::IndexType::CONTENT: keyName = "content"; break;
    case IndexEditor::IndexType::TAGS: keyName = "tags"; break;
    case IndexEditor::IndexType::TYPE: keyName = "type"; break;
    }
    auto data = (*m_index_info)["data"].toArray();
    for (const auto &r : data) {
        const auto &rObj = r.toObject();
        if (name == rObj[keyName].toString()) {
            emit onModifyRepeat(name);
            return;
        }
    }
    // modify name in data
    auto newObj = data[index].toObject();
    newObj[keyName] = name;
    data[index] = newObj;
    (*m_index_info)["data"] = data;
    emit onModifySuccessed();
}

void IndexEditor::setOrgData(const QJsonObject &data)
{
    QJsonArray headers;
    for (const auto &h : data["headers"].toArray()) {
        bool visible = true;
        if (h.toString() == "trans_model" or h.toString() == "trans_camera")
            visible = false;
        headers << QJsonObject{
            {"name", h.toString()},
            {"editable", false},
            {"is_array", false},
            {"visible",  visible}
        };
    }
    (*m_org_info)["headers"] = headers;
    (*m_org_info)["data"] = data["data"];
}
