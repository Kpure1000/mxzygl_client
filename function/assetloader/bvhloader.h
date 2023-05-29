#ifndef MX_BVHLOADER_H
#define MX_BVHLOADER_H

#include <memory>
#include <functional>

#include "assetcache.h"

namespace res {
struct BVH;
}

class BVHLoader : public AssetCache
{
    Q_OBJECT
public:
    explicit BVHLoader(QObject *parent = nullptr);

    static std::shared_ptr<res::BVH> loadBVH(const QString& filePath);

    void cachedAsyncLoad(const QString &filePath, std::function<void(bool)> loadCallBack);
    void tempAsyncLoad(const QString &filePath, std::function<void(std::shared_ptr<res::BVH>)> loadCallBack);

public:

    static BVHLoader* getInstance(QObject *parent = nullptr)
    {
        static BVHLoader bl(parent);
        return &bl;
    }

signals:

};

#endif // MX_BVHLOADER_H
