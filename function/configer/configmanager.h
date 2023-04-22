#ifndef MX_CONFIGMANAGER_H
#define MX_CONFIGMANAGER_H

#include <QObject>
#include <QTimer>

#include <functional>

#include "config.h"

class ConfigManager : public QObject
{
    Q_OBJECT

public:

    explicit ConfigManager(const QString &filePath = "./conf.ini", QObject *parent = nullptr);

    void setServer(const conf::ServerInfo& info);
    conf::ServerInfo getServer() const;

//    void addConfigListener(QObject *rec, std::function<void()>&& func);
//    void removeConfigListener(QObject *rec);

public:
    static ConfigManager* getInstance(const QString &filePath = "./conf.ini", QObject *parent = nullptr);

signals:
    void onConfModified();

private:

    QString m_filePath;
    QTimer timer;

};

#endif // MX_CONFIGMANAGER_H
