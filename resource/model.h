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

    static std::shared_ptr<Mesh> batchMesh(const std::vector<std::shared_ptr<Mesh>> &meshes)
    {
        auto ret_mesh = std::make_shared<Mesh>();
        // TODO
        for (auto mesh : meshes) {
            qDebug() << "nvert:" << mesh->vertices.size();
        }
        return ret_mesh;
    }
};

struct Model
{
    Model();
};

} // namespace res

class ModelManager : public QObject, public IAssetManager<std::string, std::shared_ptr<res::Model>>
{
    Q_OBJECT
public:
    explicit ModelManager(QObject *parent = nullptr) : QObject(parent) {}

    std::shared_ptr<res::Model> loadFBX(const QString& filePath) const;
    std::shared_ptr<res::Model> loadOBJ(const QString& filePath) const;
    std::shared_ptr<res::Model> loadOFF(const QString& filePath) const;

public:

    static ModelManager *getInstance(QObject *parent = nullptr)
    {
        static ModelManager instance_am(parent);
        return &instance_am;
    }

signals:
};

#endif // MX_MODEL_H
