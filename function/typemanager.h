#ifndef MX_MODELTYPEMANAGER_H
#define MX_MODELTYPEMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <unordered_set>

#include "resource/assetinfo.h"

class Client;

class TypeManager : public QObject
{
    Q_OBJECT
public:
    using AssetType = res::AssetInfo::AssetType;

    explicit TypeManager(AssetType type, QObject *parent = nullptr);
    ~TypeManager();

    void pull();

    void push();

    void add(const QString &typeName);

    void remove(int index);

    QString type(int index);

    inline QJsonObject *getTypes() const { return m_types; }
    QJsonArray getTypesNameList();

    AssetType getType() const { return m_type; }

signals:
    void onResponsing(const QString &info, bool is_continue);
    void onPullSuccessful();
    void onPushSuccessful();
    void onAddRepeat(const QString &typeName);
    void onAddSuccessed();
    void onRemoveSuccessed();

private:
    void setTypesData(const QJsonObject &data);

    AssetType m_type;

    Client *m_client;

    QJsonObject *m_types;

    std::unordered_set<std::string> m_nameSet;
};

#endif // MX_MODELTYPEMANAGER_H
