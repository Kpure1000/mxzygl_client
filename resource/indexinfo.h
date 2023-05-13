#ifndef INDEXINFO_H
#define INDEXINFO_H

#include "assetinfo.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QString>

namespace res {


struct IndexInfo : public AssetInfo
{
    enum class IndexType : int {
        CONTENT = 0x01 << 0,
        TYPE    = 0x01 << 1,
        TAGS    = 0x01 << 2
    };
    static QJsonObject get_data(IndexType type, const QString &name);
    static QJsonArray get_headers();
};

struct IndexModelInfo : public AssetInfo
{
    using IndexType = IndexInfo::IndexType;
//    static QJsonObject get_data(IndexType type);
    static QJsonArray get_headers(const QJsonArray &header);
};

}
#endif // INDEXINFO_H
