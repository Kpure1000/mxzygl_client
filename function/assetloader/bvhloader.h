#ifndef MX_BVHLOADER_H
#define MX_BVHLOADER_H

#include <QObject>

#include "resource/bvh.h"

class BVHLoader : public QObject
{
    Q_OBJECT
public:
    explicit BVHLoader(QObject *parent = nullptr);

    std::shared_ptr<res::BVH> loadBVH(const QString& filePath, const QString& bvhName) const;
    
public:

    static BVHLoader* getInstance(QObject *parent = nullptr)
    {
        static BVHLoader bl(parent);
        return &bl;
    }

signals:

};

#endif // MX_BVHLOADER_H
