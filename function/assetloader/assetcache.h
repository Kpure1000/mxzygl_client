#ifndef MX_ASSETLOADER_H
#define MX_ASSETLOADER_H

#include <QObject>
#include <QTimer>
#include <memory>

#include "utils/assetmanager.h"

class AssetCache : public QObject, public IAssetManager<std::string, std::shared_ptr<QTimer>>
{
    Q_OBJECT
public:
    explicit AssetCache(QObject *parent = nullptr);

protected:
    void removeCache(const QString &filePath);

    void restartCache(const QString &filePath);

signals:
    void onAssetLoaded(const QString &filePath);

    void onCacheTimeout(const QString &filePath);

private slots:
    void cacheStart(const QString &filePath);

private:
    /**
     * @brief 缓存时长
     */
    int m_cache_timeout;
    /**
     * @brief 尝试清理缓存被使用资源的时长
     */
    int m_cache_timeout_retry;

};

#endif // MX_ASSETLOADER_H
