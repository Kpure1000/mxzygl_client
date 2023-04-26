#ifndef MX_MODELLOADER_H
#define MX_MODELLOADER_H

#include <QObject>
#include <QTimer>
#include <functional>

#include "utils/assetmanager.h"
#include "resource/model.h"

class ModelLoader : public QObject, public IAssetManager<std::string, std::shared_ptr<QTimer>>
{
    Q_OBJECT
public:
    explicit ModelLoader(QObject *parent = nullptr);

    static std::shared_ptr<res::Model> loadFBX(const QString& filePath, const QString& modelName, bool doGlobalTransform = true);
    static std::shared_ptr<res::Model> loadOBJ(const QString& filePath, const QString& modelName);
    static std::shared_ptr<res::Model> loadOFF(const QString& filePath, const QString& modelName);

    void asyncLoad(const QString &filePath, const QString& modelName, std::function<void()> loadCallBack);

public:

    static ModelLoader* getInstance(QObject *parent = nullptr)
    {
        static ModelLoader ml(parent);
        return &ml;
    }

private:
    void cacheStart(const QString &modelName);

signals:
    void onModelLoaded(const QString &modelName);

private:
    /**
     * @brief 缓存时长, 5分钟
     */
    const int m_cache_timeout = 1000 * 60 * 5;
    /**
     * @brief 尝试清理缓存被使用资源的时长, 20秒
     */
    const int m_used_try_timeout = 1000 * 20;

};

#endif // MX_MODELLOADER_H
