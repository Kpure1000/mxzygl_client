#include "modelsearch.h"

#include <QJsonArray>
#include <QDebug>

#include "resource/assetinfo.h"

ModelSearch::ModelSearch(SearchType type, QObject *parent)
    : QObject{parent}, m_type(type)
{
    {
        QJsonArray headers;
        auto header_names = res::AssetInfo::getInfoNameList();
        for (auto& header : header_names) {
            headers.append(header);
        }
        QJsonArray data;
        m_search_info = std::make_shared<QJsonObject>();
        m_search_info->insert("headers", headers);
        m_search_info->insert("data", data);

    }
    {
        QJsonArray headers;
        QJsonArray data;
        m_result_info = std::make_shared<QJsonObject>();
        m_result_info->insert("headers", headers);
        m_result_info->insert("data", data);
    }
}

void ModelSearch::setSearchInfo(const QString &info)
{
    if (m_type == SearchType::CONTENT) {
        auto asset = res::AssetInfo(res::AssetInfo::AssetType::MODEL, info);
        auto data = QJsonArray();
        data.append(*asset.getJsonObject());
        (*m_search_info)["data"] = data;
    } else if (m_type == SearchType::TYPE || m_type == SearchType::TAG) {
        //
    }
}
