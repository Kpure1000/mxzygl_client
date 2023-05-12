#include "assetcache.h"
#include "function/configer/configmanager.h"

#include <QDebug>

AssetCache::AssetCache(QObject *parent) : QObject(parent)
{
    m_cache_timeout = 1000 * 60 * ConfigManager::getInstance()->getConfig("Asset/CacheTimeout").toInt();
    m_cache_timeout_retry = 1000 * 60 * ConfigManager::getInstance()->getConfig("Asset/CacheTimeoutRetry").toInt();

    connect(this, &AssetCache::onAssetLoaded, this, &AssetCache::cacheStart);

    connect(ConfigManager::getInstance(), &ConfigManager::onConfModified, this, [this](){
        m_cache_timeout = 1000 * 60 * ConfigManager::getInstance()->getConfig("Asset/CacheTimeout").toInt();
        m_cache_timeout_retry = 1000 * 60 * ConfigManager::getInstance()->getConfig("Asset/CacheTimeoutRetry").toInt();
    });

}

void AssetCache::removeCache(const QString &filePath)
{
    remove(filePath.toStdString());
}

void AssetCache::restartCache(const QString &filePath)
{
    get(filePath.toStdString())->start(m_cache_timeout_retry);
}


void AssetCache::cacheStart(const QString &filePath)
{
//    qDebug() << "AssetCache::cacheStart>>" << filePath;
    auto filePath_std = filePath.toStdString();
    QTimer *timer;
    if (this->has(filePath_std)) {
        timer = this->get(filePath_std).get();
    } else {
        timer = this->add(filePath_std, std::make_shared<QTimer>()).get();
        connect(timer, &QTimer::timeout, this, [=]() {
//            qDebug() << "AssetCache::QTimer::timeout>> " << filePath;
            timer->stop();
            emit this->onCacheTimeout(filePath);
        }, Qt::DirectConnection);
    }
    timer->stop();
    timer->start(m_cache_timeout);
}
