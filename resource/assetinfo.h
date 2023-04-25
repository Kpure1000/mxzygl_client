#ifndef MX_ASSETINFO_H
#define MX_ASSETINFO_H

#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

#include "model.h"
#include "bvh.h"
#include "effect.h"

namespace res {

struct AssetInfo {

    enum class AssetType : int {
        MODEL           = 0x1 << 0,
        BVH             = 0x1 << 1,
        EFFECT          = 0x1 << 2,
    };

    AssetType       assetType = AssetType::MODEL;  // 资源种类

//    QString         catagory;   // 大类

    QString         name;       // 名称
    QString         type;       // 类型
    QString         tag;        // 标签

    QString         fileType;   // 文件类型
    QString         path;       // 路径

    AssetInfo() = default;
    AssetInfo(AssetType asset_type, const QString &filePath);

    std::shared_ptr<QJsonDocument>  getJsonDoc()        const;
    std::shared_ptr<QJsonObject>    getJsonObject()     const;

    QStringList                     getInfoValueList()  const;
    QStringList                     getInfoNameList()   const;

};

}

#endif // MX_ASSETINFO_H
