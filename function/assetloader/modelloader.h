#ifndef MX_MODELLOADER_H
#define MX_MODELLOADER_H

#include <QObject>
#include <QTimer>
#include <functional>

#include "resource/model.h"
#include "assetcache.h"

struct aiNode;

class ModelLoader : public AssetCache
{
    Q_OBJECT
public:
    explicit ModelLoader(QObject *parent = nullptr);

    static std::shared_ptr<res::Model> loadFBX(const QString& filePath, bool doGlobalTransform = true);
    static std::shared_ptr<res::Model> loadOBJ(const QString& filePath);
    static std::shared_ptr<res::Model> loadOFF(const QString& filePath);

    void cachedAsyncLoad(const QString &filePath, std::function<void(bool)> loadCallBack);
    void tempAsyncLoad(const QString &filePath, std::function<void(std::shared_ptr<res::Model>)> loadCallBack);

public:

    static ModelLoader* getInstance(QObject *parent = nullptr)
    {
        static ModelLoader ml(parent);
        return &ml;
    }

};

#endif // MX_MODELLOADER_H
