#include "searchinfo.h"

//std::shared_ptr<QJsonObject> res::SearchInfo::get_data(SearchType searchType, float similarity)
//{
//    auto obj = AssetInfo::get_data(AssetType::MODEL, "");
//    obj->insert("similarity", similarity);
//    return obj;
//}

QJsonArray res::SearchInfo::get_headers(const QJsonArray &headers)
{
    QJsonArray ret;
    for (const auto &header : headers) {
        if (header.toString() == "hash")
            ret << toHeaderElement(header.toString(), false, false);
        else
            ret << toHeaderElement(header.toString(), false, true);
    }
    return ret;
}
