#include "configmanager.h"

#include <QSettings>
#include <QApplication>
#include <QDebug>
#include <QTextCodec>

ConfigManager::ConfigManager(const QString &filePath, QObject *parent) : QObject(parent), m_filePath(filePath)
{
    m_setting_watcher = new QFileSystemWatcher({filePath}, this);
    connect(m_setting_watcher, &QFileSystemWatcher::fileChanged, this, [this](){
        emit onConfModified();
    });
}

void ConfigManager::setConfig(const QString &key, const QVariant &value)
{
    QSettings qst(m_filePath, QSettings::Format::IniFormat, const_cast<ConfigManager*>(this));
    qst.setValue(key, value);
//    emit onConfModified();
}

void ConfigManager::setConfigs(std::initializer_list<std::pair<QString, QVariant>> &&inList)
{
    QSettings qst(m_filePath, QSettings::Format::IniFormat, const_cast<ConfigManager*>(this));
    for (auto &_p : inList) {
        qst.setValue(_p.first, _p.second);
    }
//    emit onConfModified();
}

QVariant ConfigManager::getConfig(const QString &key, const QVariant& defaultValue) const
{
    QSettings qst(m_filePath, QSettings::Format::IniFormat, const_cast<ConfigManager*>(this));
    return qst.value(key, defaultValue);
}

QVariantList ConfigManager::getConfigs(const QStringList &keys) const
{
    QSettings qst(m_filePath, QSettings::Format::IniFormat, const_cast<ConfigManager*>(this));
    qst.setIniCodec(QTextCodec::codecForName("utf-8"));
    QVariantList ret;
    ret.reserve(keys.size());
    for (auto& key : keys) {
        ret += qst.value(key);
    }
    return ret;
}

ConfigManager *ConfigManager::getInstance(const QString &filePath, QObject *parent)
{
    static ConfigManager cm(filePath, parent);
    return &cm;
}
