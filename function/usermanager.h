#ifndef MX_USERMANAGER_H
#define MX_USERMANAGER_H

#include <QObject>

#include "resource/UserInfo.h"

class Client;

class UserManager : public QObject
{
    Q_OBJECT
public:
    explicit UserManager(QObject *parent = nullptr);

    void login(const QString &name, const QString &pswd);

    void signup(const QString &name, const QString &pswd);

    inline const QString &getName() const { return m_name; }

    inline bool isAdmin() { return m_permission == res::UserInfo::ADMIN; }

signals:
    void onResponsing(const QString &info, bool);
    void onResponsingFailed(const QString &info);

    void onLoginFailed(const QString &info);
    void onLoginSucceeded(const QString &name, res::UserInfo::UserPermission per);

    void onSignupFailed(const QString &info);
    void onSignupSucceeded(const QString &name, res::UserInfo::UserPermission per);

private:

    Client *m_client;

    QString m_name;
    res::UserInfo::UserPermission m_permission;

public:
    static UserManager *getInstance()
    {
        static UserManager instance;
        return &instance;
    }
};

#endif // MX_USERMANAGER_H
