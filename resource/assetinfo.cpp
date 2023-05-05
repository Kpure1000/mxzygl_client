#include "assetinfo.h"

#include <QFileInfo>

res::AssetInfo::AssetInfo(AssetType asset_type, const QString &filePath)
    :assetType(asset_type), path(filePath)
{
    auto fileInfo = QFileInfo(filePath);
    name = fileInfo.baseName();
//    tag = type = fileInfo.baseName().split('_')[0];
    tag = type = "";
    if (assetType == AssetType::MODEL || assetType == AssetType::BVH) {
        fileType = fileInfo.filePath().split('.').back();
    } else {
        // TODO: Effect Asset fileType
    }
}

std::shared_ptr<QJsonDocument> res::AssetInfo::getJsonDoc() const
{
    auto doc = std::make_shared<QJsonDocument>();
    doc->setObject(*getJsonObject());
    return doc;
}

std::shared_ptr<QJsonObject> res::AssetInfo::getJsonObject() const
{
    auto obj = std::make_shared<QJsonObject>();
    obj->insert("name", name);
//    obj->insert("catagory", catagory);
    obj->insert("type", type);
    obj->insert("tag", tag);
    obj->insert("fileType", fileType);
    obj->insert("path", path);
    return obj;
}

QStringList res::AssetInfo::getInfoValueList() const
{
    return QStringList()
//                           << catagory
                           << name
                           << type
                           << tag
                           << fileType
                           << path;
}

QStringList res::AssetInfo::getInfoNameList()
{
    return QStringList()
//                           << "catagory"
                           << "name"
                           << "type"
                           << "tag"
                           << "fileType"
                           << "path";
}
