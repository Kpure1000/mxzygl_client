#ifndef MODELTAGSMANAGER_H
#define MODELTAGSMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

#include "resource/assetinfo.h"

class Client;

class ModelTagsManager : public QObject
{
    Q_OBJECT
public:
    using AssetType = res::AssetInfo::AssetType;

    explicit ModelTagsManager(AssetType type, QObject *parent = nullptr);
    ~ModelTagsManager();

    void pull();

    QJsonObject *getTags() const { return m_tags; }
    QJsonArray getTagsNameList();

signals:
    void onResponsing(const QString &info, bool is_continue);
    void onPullSuccessful();

private:
    void setTagsData(const QJsonObject &data);

    AssetType m_type;

    Client *m_client;

    QJsonObject *m_tags;

};

#endif // MODELTAGSMANAGER_H
