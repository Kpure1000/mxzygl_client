#ifndef MX_ASSETINFO_H
#define MX_ASSETINFO_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
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

    static std::shared_ptr<QJsonObject>    get_data(AssetType asset_type, const QString &filePath);

    static  QJsonArray                      get_headers();

    static  QJsonObject toHeaderElement(const QString&header_name, bool editable, bool visible);

};

}

#endif // MX_ASSETINFO_H
