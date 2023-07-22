#ifndef MX_VERSIONCONTROLLER_H
#define MX_VERSIONCONTROLLER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <unordered_set>

class Client;

class VersionController : public QObject
{
    Q_OBJECT
public:
    explicit VersionController(QObject *parent = nullptr);
    ~VersionController();

    void createRepo(const QString &repo_name);
    void pullRepo();
    void addVersion(const QString &repo_id, const QString &ver_name);
    void syncVersion(int repo_index, int ver_index);
    void queryVersion();
    void deleteVersion(int repo_index, int ver_index);
    void rollbackVersion(int repo_index, int ver_index);

    QJsonArray *getRepoInfo() const;
    QJsonObject *getVersionInf() const;

    QJsonObject getRepo(int repo_index) const;
    QJsonArray getVersions(int repo_index) const;
    QJsonObject getVersion(int repo_index, int version_index) const;

    int getRepoIndexFromName(const QString &name);
    int getRepoIndexFromID(const QString &id);
    int getVersionIndexFromHash(int repo_index, const QString &hash);

    void addRepoTmp(const QJsonObject &repo_info);

    bool hasRepo(const QString &repo_name) const;

signals:
    void onResponsing(const QString &info, bool is_continue);
    void onCreateReopSuccessful(const QJsonObject &repo_info);
    void onPullReopSuccessful();
    void onAddVersionSuccessful();
    void onSyncVersionSuccessful();
    void onQueryVersionSuccessful(const QJsonObject &repo_info);
    void onDeleteVersionSuccessful();
    void onRollbackVersionSuccessful();

private:
    void setPullData(const QJsonObject &data);

    Client *m_client;
    QJsonArray *m_repo;
    QJsonObject *m_version;

    std::unordered_set<std::string> m_repoSet;

};

#endif // MX_VERSIONCONTROLLER_H
