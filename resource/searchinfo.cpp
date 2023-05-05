#include "searchinfo.h"

res::SearchInfo::SearchInfo(SearchType searchType)
    : AssetInfo{AssetType::MODEL, ""}, searchType(searchType)
{


}

std::shared_ptr<QJsonObject> res::SearchInfo::getJsonObject() const
{
    auto obj = dynamic_cast<const AssetInfo*>(this)->getJsonObject();
    obj->insert("similarity", similarity);
    return obj;
}

QStringList res::SearchInfo::getInfoValueList() const
{
    return dynamic_cast<const AssetInfo*>(this)->getInfoValueList() << std::to_string(similarity).c_str();
}

QStringList res::SearchInfo::getInfoNameList()
{
    return AssetInfo::getInfoNameList() << "similarity";
}
