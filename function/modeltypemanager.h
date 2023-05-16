#ifndef MX_MODELTYPEMANAGER_H
#define MX_MODELTYPEMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

#include "resource/assetinfo.h"

class Client;

class ModelTypeManager : public QObject
{
    Q_OBJECT
public:
    using AssetType = res::AssetInfo::AssetType;

    explicit ModelTypeManager(AssetType type, QObject *parent = nullptr);
    ~ModelTypeManager();

    void pull();

    inline QJsonObject *getTypes() const { return m_types; }
    QJsonArray getTypesNameList();

signals:
    void onResponsing(const QString &info, bool is_continue);
    void onPullSuccessful();

private:
    void setTypesData(const QJsonObject &data);

    AssetType m_type;

    Client *m_client;

    QJsonObject *m_types;
};

#endif // MX_MODELTYPEMANAGER_H
