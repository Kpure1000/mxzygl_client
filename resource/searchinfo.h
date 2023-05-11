#ifndef MX_SEARCHINFO_H
#define MX_SEARCHINFO_H

#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "assetinfo.h"

namespace res {

struct SearchInfo : public AssetInfo {

    enum class SearchType : int {
        CONTENT     =   0x01 << 0,  // 内容
        TYPE        =   0x01 << 1,  // 类型
        TAG         =   0x01 << 2   // 标签
    };

    static std::shared_ptr<QJsonObject> get_data(SearchType searchType, float similarity);

    static QJsonArray                   get_headers();

};

}

#endif // SEARCHINFO_H
