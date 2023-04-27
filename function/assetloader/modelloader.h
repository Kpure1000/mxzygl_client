#ifndef MX_MODELLOADER_H
#define MX_MODELLOADER_H

#include <QObject>
#include <QTimer>
#include <functional>

#include "resource/model.h"
#include "assetcache.h"

class ModelLoader : public AssetCache
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

};

#endif // MX_MODELLOADER_H
