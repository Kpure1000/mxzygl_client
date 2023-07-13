#ifndef VERSIONCONTROLLER_H
#define VERSIONCONTROLLER_H

#include <QObject>

class Client;

class VersionController : public QObject
{
    Q_OBJECT
public:
    explicit VersionController(QObject *parent = nullptr);

    void createRepo();
    void pullRepo();
    void addVersion();
    void syncVersion();
    void queryVersion();
    void deleteVersion();
    void rollbackVersion();

signals:
    void onResponsing(const QString &info, bool is_continue);
    void onCreateReopSuccessful();
    void onPullReopSuccessful();
    void onAddVersionSuccessful();
    void onSyncVersionSuccessful();
    void onQueryVersionSuccessful();
    void onDeleteVersionSuccessful();
    void onRollbackVersionSuccessful();

private:
    Client *m_client;

};

#endif // VERSIONCONTROLLER_H
