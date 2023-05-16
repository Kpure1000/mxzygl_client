#include "indexinfo.h"

QJsonObject res::IndexInfo::get_data(IndexType type, const QString &name)
{
    return QJsonObject({
        {"name",    name                    },
        {"type",    static_cast<int>(type)  }
    });
}

QJsonArray res::IndexInfo::get_headers()
{
    return QJsonArray() << toHeaderElement("name", true, false, true) << toHeaderElement("type", false, false, true);
}

QJsonArray res::IndexModelInfo::get_headers(const QJsonArray &header)
{
    QJsonArray ret;
    for (const auto &header : header) {
        if (header.toString() == "hash") ret << toHeaderElement(header.toString(), false, false, false);
//        if (header.toString() == "type") ret << toHeaderElement(header.toString(), true, true);
//        if (header.toString() == "tags") ret << toHeaderElement(header.toString(), true, true);
        else
            ret << toHeaderElement(header.toString(), false, false, true);
    }
    return ret;
}
