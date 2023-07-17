#ifndef DBTABLEVIEWER_H
#define DBTABLEVIEWER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

#include "resource/assetinfo.h"

class Client;

class DBTableViewer : public QObject
{
    Q_OBJECT
public:

    using TableType = res::AssetInfo::AssetType;

    explicit DBTableViewer(TableType type, QObject *parent = nullptr);
    ~DBTableViewer();

    void pullTBInfo();

    QJsonObject *getTableInfo() const { return m_info; }

signals:
    void onResponsing(const QString &info, bool is_continue);
    void onTBPullSuccessful();

private:
    void setInfoData(const QJsonObject &data);

    TableType m_type;

    Client *m_client;

    QJsonObject *m_info;


};

#endif // DBTABLEVIEWER_H
