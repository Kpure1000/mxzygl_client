#include "configmanager.h"

#include <QSettings>
#include <QApplication>
#include <QDebug>

ConfigManager::ConfigManager(const QString &filePath, QObject *parent) : QObject(parent), m_filePath(filePath)
{
    int interval = 300;
    timer.connect(&timer, &QTimer::timeout, this, [interval, this](){
        emit onConfModified();
        timer.start(interval);
    });
    timer.start(interval);
}

void ConfigManager::setServer(const conf::ServerInfo &info)
{
    QSettings qst(m_filePath, QSettings::Format::IniFormat, this);
    qst.setValue("Server/Addr", QString(info.addr.c_str()));
    qst.setValue("Server/Port", info.port);

    //    emit onConfModified();
}

conf::ServerInfo ConfigManager::getServer() const
{
    QSettings qst(m_filePath, QSettings::Format::IniFormat, const_cast<ConfigManager*>(this));
    return {
        qst.value("Server/Addr").toString().toStdString(),
        qst.value("Server/Port").toInt()
    };
}

//void ConfigManager::addConfigListener(QObject *rec, std::function<void ()> &&func)
//{
//    connect(ConfigManager::instance(), &ConfigManager::onConfModified, rec, func);
//}

//void ConfigManager::removeConfigListener(QObject *rec)
//{
//    disconnect(ConfigManager::instance(),&ConfigManager::onConfModified, rec, 0);
//}

ConfigManager *ConfigManager::instance(const QString &filePath, QObject *parent)
{
    static ConfigManager cm(filePath, parent);
    return &cm;
}
