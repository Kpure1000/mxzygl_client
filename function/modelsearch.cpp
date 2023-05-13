#include "modelsearch.h"

#include <QJsonArray>
#include <QDebug>
#include <QMetaEnum>

#include "resource/assetinfo.h"
#include "function/network/client.h"
#include "function/network/protocal.h"
#include "function/metacategory.h"

ModelSearch::ModelSearch(SearchType type, QObject *parent)
    : QObject{parent}, m_type(type), m_search_keyword("")
{
    m_client = new Client(this);
    connect(m_client, &Client::onReadOver, this, [=](const QJsonObject &data, const QByteArray &extraData) {
        auto response_type = static_cast<Protocal::HeaderField>(data["type"].toInt());
        auto status = data["status"].toString();
        if (Protocal::HeaderField::RESPONSE_ERROR == response_type) {
            emit onResponsing(tr("服务请求错误. Info:") + status, false);
//            emit onSearchOver(tr("服务请求错误. Info:") + status, false);
        } else {
            if (!status.isEmpty()) {
                emit onResponsing(tr("检索失败. Info: ") + status, false);
//                emit onSearchOver(tr("检索失败. Info: ") + status, false);
            } else {
                emit onResponsing(tr("检索成功!"), false);
//                emit onSearchOver(tr("检索成功!"), true);
                setResultInfo(data);
            }
        }
        emit onResultUpdate();
    });
    connect(m_client, &Client::onSendingStateChanged, this, &ModelSearch::onResponsing);

    {
        QJsonArray headers = res::AssetInfo::get_headers();
        QJsonArray data;
        m_search_info = std::make_shared<QJsonObject>();
        m_search_info->insert("headers", headers);
        m_search_info->insert("data", data);

        switch (type) {
        case SearchType::CONTENT:
            m_search_info->insert("type", static_cast<int>(Protocal::HeaderField::REQUEST_SEARCHMODEL));
            break;
        case SearchType::TAGS:
            m_search_info->insert("type", static_cast<int>(Protocal::HeaderField::REQUEST_SEARCHLABEL));
            break;
        case SearchType::TYPE:
            m_search_info->insert("type", static_cast<int>(Protocal::HeaderField::REQUEST_SEARCHTYPE));
            break;
        }
        m_search_info->insert("category", MetaCategory::getInstance()->getCategoryInt());
        // TODO: 连接MetaCategory修改信号
        connect(MetaCategory::getInstance(), &MetaCategory::onCategoryModyfied, this, [=](){
            (*m_search_info)["category"] = MetaCategory::getInstance()->getCategoryInt();
        });
    }

    {
        QJsonArray headers;
        QJsonArray data;
        m_result_info = std::make_shared<QJsonObject>();
    }
}

void ModelSearch::setSearchInfo(const QString &info)
{
    if (m_type == SearchType::CONTENT) {
        auto data = QJsonArray();
        data.append(*res::AssetInfo::get_data(res::AssetInfo::AssetType::MODEL, info));
        (*m_search_info)["data"] = data;
    } else if (m_type == SearchType::TYPE || m_type == SearchType::TAGS) {
        auto data = QJsonArray();
        data.append(QJsonObject());
        (*m_search_info)["data"] = data;
        m_search_keyword = info;
    }
}

void ModelSearch::search()
{
    switch (m_type) {
    case SearchType::CONTENT: emit onResponsing(tr("模型内容检索开始"), true); break;
    case SearchType::TYPE:    emit onResponsing(tr("模型类型检索开始"), true); break;
    case SearchType::TAGS:     emit onResponsing(tr("模型标签检索开始"), true); break;
    }
    m_client->sendData(*m_search_info, m_search_keyword.toUtf8());
}

void ModelSearch::clearResults()
{
    (*m_result_info) = QJsonObject();
    emit onResultClear();
}

QStringList ModelSearch::getFilePaths(const std::vector<int> &index) const
{
    QStringList ret;
    auto data = (*m_result_info)["data"].toArray();
    for (auto id : index)
        {
            auto row = data[id].toObject();
            ret << row["hash"].toString();
        }
    return ret;
}

QStringList ModelSearch::getPreviewInfo(const std::vector<int> &index) const
{
    QStringList ret;
    auto data = (*m_result_info)["data"].toArray();
    for (auto id : index) {
        auto row = data[id].toObject();
        if (m_type == SearchType::CONTENT)
            ret << (row["name"].toString() + "; sim = " + row["similarity"].toString());
        else
            ret << (row["name"].toString());
    }
    return ret;
}

void ModelSearch::setResultInfo(const QJsonObject &result)
{
    (*m_result_info) = QJsonObject{{
        {"headers",  res::SearchInfo::get_headers(result["headers"].toArray())   },
        {"data",    result["data"].toArray()                                    }
    }};
}
