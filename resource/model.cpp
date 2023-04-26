#include "model.h"

using namespace res;

std::shared_ptr<Mesh> Mesh::batchMesh(const std::vector<std::shared_ptr<Mesh> > &meshes)
{
    auto ret_mesh = std::make_shared<Mesh>();
    // TODO
    for (auto mesh : meshes) {
        qDebug() << "nvert:" << mesh->vertices.size();
    }
    return ret_mesh;
}
