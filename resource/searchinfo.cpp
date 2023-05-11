#include "searchinfo.h"

std::shared_ptr<QJsonObject> res::SearchInfo::get_data(SearchType searchType, float similarity)
{
    auto obj = AssetInfo::get_data(AssetType::MODEL, "");
    obj->insert("similarity", similarity);
    return obj;
}

QJsonArray res::SearchInfo::get_headers()
{
    return AssetInfo::get_headers() << toHeaderElement("similarity", false, true);
}
