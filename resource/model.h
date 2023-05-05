#ifndef MX_MODEL_H
#define MX_MODEL_H

#include <vector>
#include <QVector3D>
#include <QVector2D>
#include <QDebug>

#include <memory>

#include "utils/assetmanager.h"

namespace res {

struct Mesh
{
    std::vector<QVector3D> vertices;
    std::vector<QVector3D> normals;
    std::vector<QVector2D> uvs;

    std::vector<unsigned int> indices;

    QVector3D centroid;
    float diagonal;

    inline int verticesNum() const { return static_cast<int>(vertices.size()); }
    inline int facesNum() const { return static_cast<int>(indices.size()) / 3; }

    static std::shared_ptr<Mesh> batchMesh(const std::vector<std::shared_ptr<Mesh>> &meshes);
};

struct Model
{
    std::vector<std::shared_ptr<Mesh>> meshes;

    inline int meshesNum() const { return static_cast<int>(meshes.size()); }
};

} // namespace res

class ModelManager : public QObject, public IAssetManager<std::string, std::shared_ptr<res::Model>>
{
    Q_OBJECT
public:
    explicit ModelManager(QObject *parent = nullptr) : QObject(parent) {}

public:

    static ModelManager *getInstance(QObject *parent = nullptr)
    {
        static ModelManager instance_am(parent);
        return &instance_am;
    }

signals:
};


//class ModelFileManager : public QObject, public IAssetManager<std::string, QString>
//{
//    Q_OBJECT
//public:
//    explicit ModelFileManager(QObject *parent = nullptr) : QObject(parent) {}

//public:

//    static ModelFileManager *getInstance(QObject *parent = nullptr)
//    {
//        static ModelFileManager instance_am(parent);
//        return &instance_am;
//    }

//signals:
//};



#endif // MX_MODEL_H
