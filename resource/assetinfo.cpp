#include "assetinfo.h"

#include <QFileInfo>

//std::shared_ptr<QJsonObject> res::AssetInfo::get_data(AssetType asset_type, const QString &filePath)
//{
//    AssetType       assetType = asset_type;

//    QString         name;           // 名称
//    QString         type;           // 类型
//    QString         tags;           // 标签
//    QString         description;    // 描述
//    QString         fileType;       // 文件类型
//    QString         path;           // 路径

//    path = filePath;
//    auto fileInfo = QFileInfo(filePath);
//    name = fileInfo.baseName();
//    description = tags = type = name.split('_')[0];
//    if (assetType == AssetType::MODEL || assetType == AssetType::BVH) {
//        fileType = fileInfo.filePath().split('.').back();
//    } else {
//        // TODO: Effect Asset fileType
//    }

//    auto obj = std::make_shared<QJsonObject>();
//    obj->insert("name", name);
//    obj->insert("type", type);
//    obj->insert("tags", tags);
//    obj->insert("description", description);
//    obj->insert("fileType", fileType);
//    obj->insert("path", path);

//    return obj;
//}

//QJsonArray res::AssetInfo::get_headers()
//{
//    return QJsonArray()
//            << toHeaderElement("name",          false,  false, true)
//            << toHeaderElement("type",          true,   true, true)
//            << toHeaderElement("tags",          true,   true, true)
//            << toHeaderElement("fileType",      false,  false, true)
//            << toHeaderElement("path",          false,  false, true);
//}

QJsonObject res::AssetInfo::toHeaderElement(const QString &header_name, bool editable, bool is_array, bool visible)
{
    return QJsonObject{
        {"name", header_name},
        {"editable", editable},
        {"is_array", is_array},
        {"visible", visible},
    };
}
