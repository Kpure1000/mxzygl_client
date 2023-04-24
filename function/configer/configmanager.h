#ifndef MX_CONFIGMANAGER_H
#define MX_CONFIGMANAGER_H

#include <QObject>
#include <QTimer>
#include <functional>
#include <initializer_list>
#include <QVariant>
#include <QFileSystemWatcher>

class ConfigManager : public QObject
{
    Q_OBJECT

public:

    explicit ConfigManager(const QString &filePath = "./conf.ini", QObject *parent = nullptr);

    void setConfig(const QString &key, const QVariant &value);
    void setConfigs(std::initializer_list<std::pair<QString, QVariant>> &&inList);

    QVariant getConfig(const QString &key, const QVariant &defaultValue = QVariant()) const;
    QVariantList getConfigs(const QStringList& keys) const;

public:
    static ConfigManager* getInstance(const QString &filePath = "./conf.ini", QObject *parent = nullptr);

signals:
    void onConfModified();

private:

    QString m_filePath;
//    QTimer m_timer;
    QFileSystemWatcher *m_setting_watcher;


};

#endif // MX_CONFIGMANAGER_H
