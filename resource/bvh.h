#ifndef MX_BVH_H
#define MX_BVH_H

#include <QString>
#include <QVector3D>
#include <QMatrix4x4>
#include <memory>
#include <vector>

#include "utils/assetmanager.h"

namespace res
{

struct Bone
{
    std::vector<QMatrix4x4> trans_mats;
};

struct BoneMesh {
    std::vector<QVector3D> vertices, normals;
    std::vector<QVector4D> trans_r0,trans_r1,trans_r2,trans_r3;
    std::vector<QVector3D> trans_nor_r0,trans_nor_r1,trans_nor_r2;
//    std::vector<QMatrix4x4> trans;
//    std::vector<QMatrix3x3> trans_nor;
    std::vector<unsigned int> indices;
    BoneMesh(size_t nBones);
    BoneMesh(const QMatrix4x4 &trans_mat, const QMatrix3x3 &trans_nor);
    void operator<<(const BoneMesh& boneMesh);
};

struct BVH
{
    /**
     * @brief Samples Per Second, i.e. Ticks Per Second
     */
    float ssp = .0f;
    unsigned int nFrames = 0u;
    std::vector<std::shared_ptr<BoneMesh>> boneMeshes;
};

} // namespace res

class BVHManager : public QObject, public IAssetManager<std::string, std::shared_ptr<res::BVH>>
{
    Q_OBJECT
public:
    explicit BVHManager(QObject *parent = nullptr) : QObject(parent) {}

public:

    static BVHManager *getInstance(QObject *parent = nullptr)
    {
        static BVHManager instance_am(parent);
        return &instance_am;
    }

signals:
};

#endif // MX_BVH_H
