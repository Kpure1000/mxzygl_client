#ifndef MX_SEARCHINFO_H
#define MX_SEARCHINFO_H

#include <QStringList>
#include <QJsonObject>
#include <QJsonDocument>

#include "assetinfo.h"

namespace res {

struct SearchInfo : public AssetInfo {

    enum class SearchType : int {
        CONTENT     =   0x01 << 0,  // 内容
        TYPE        =   0x01 << 1,  // 类型
        TAG         =   0x01 << 2   // 标签
    };

    SearchType       searchType;

    float similarity;

    SearchInfo(SearchType searchType);

    std::shared_ptr<QJsonObject>    getJsonObject()     const override;

    QStringList                     getInfoValueList()  const override;
    static QStringList              getInfoNameList()   ;

};

}

#endif // SEARCHINFO_H
