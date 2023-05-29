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

    /**
     * @brief Load FBX. If 'doGlobalTransform' is set to false, 'doNormalize' will be invalid
     */
    static std::shared_ptr<res::Model> loadFBX(const QString& filePath, bool doGlobalTransform = true, bool doNormalize = false);
    static std::shared_ptr<res::Model> loadOBJ(const QString& filePath, bool doNormalize);
    static std::shared_ptr<res::Model> loadOFF(const QString& filePath, bool doNormalize);

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
