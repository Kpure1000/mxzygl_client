#ifndef TAGSMANAGER_H
#define TAGSMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <unordered_set>

#include "resource/assetinfo.h"

class Client;

class TagsManager : public QObject
{
    Q_OBJECT
public:
    using AssetType = res::AssetInfo::AssetType;

    explicit TagsManager(AssetType type, QObject *parent = nullptr);
    ~TagsManager();

    void pull();

    void push();

    void add(const QString &typeName);

    void remove(int index);

    QString tags(int index);

    QJsonObject *getTags() const { return m_tags; }
    QJsonArray getTagsNameList();

    AssetType getType() const { return m_type; }

signals:
    void onResponsing(const QString &info, bool is_continue);
    void onPullSuccessful();
    void onPushSuccessful();
    void onAddRepeat(const QString &typeName);
    void onAddSuccessed();
    void onRemoveSuccessed();

private:
    void setTagsData(const QJsonObject &data);

    AssetType m_type;

    Client *m_client;

    QJsonObject *m_tags;

    std::unordered_set<std::string> m_nameSet;
};

#endif // TAGSMANAGER_H
