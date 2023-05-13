#include "protocal.h"

#include "resource/assetinfo.h"

Protocal::Protocal(QObject *parent) : QObject(parent)
{

}

QJsonObject Protocal::pack(const QJsonObject &org)
{
    auto org_category   = org[  "category"  ].toInt();
    auto org_type       = org[  "type"      ].toInt();
    auto org_headers    = org[  "headers"   ].toArray();
    auto org_data       = org[  "data"      ].toArray();

    QJsonObject packed;

    packed.insert( "category",   org_category);
    packed.insert( "type",       org_type);

    QJsonArray headers;
    for (const auto &header : org_headers) {
        headers.append(header.toObject()["name"]);
    }

    packed.insert( "titles",     headers);
    packed.insert( "values",     org_data);

    QJsonArray block_size;
    for (int i = 0; i < org_data.size(); i++) {
        block_size.append(0);
    }

    packed.insert( "block_size", block_size);

    return packed;
}

QJsonObject Protocal::unpack(const QJsonObject &packed)
{
    auto packed_type    =   packed          [   "type"      ].toInt();
    auto packed_status  =   packed          [   "status"    ].toString();
    auto packed_models  =   packed          [   "models"    ].toObject();

    QJsonObject unpacked;

    unpacked.insert( "type",    packed_type);
    unpacked.insert( "status",  packed_status);

    if (packed_models.size() == 0){
        return unpacked;
    }

    auto packed_titles  =   packed_models   [   "titles"    ].toArray();
    auto packed_values  =   packed_models   [   "values"    ].toArray();

    QJsonArray data;

    for (int i = 0; i < packed_values.size(); i++) {
        auto value = packed_values[i].toArray();
        QJsonObject data_element;
        for (int j = 0; j < packed_titles.size(); j++) {
            data_element.insert(packed_titles[j].toString(), value[j]);
        }
        data << data_element;
    }

    unpacked.insert( "headers",  packed_titles);
    unpacked.insert( "data",  data);
    return unpacked;
}
