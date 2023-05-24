#include "modelloader.h"

#include <cmath>
#include <QMatrix4x4>
#include <QFileInfo>
#include <QDebug>

#include "utils/jobsystem.h"
//#include "3rd_part/openfbx/ofbx.h"

//#define TINYOBJLOADER_IMPLEMENTATION
//#include "3rd_part/tiny_obj_loader/tiny_obj_loader.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

using namespace res;

ModelLoader::ModelLoader(QObject *parent) : AssetCache(parent)
{
    connect(this, &AssetCache::onCacheTimeout, this, [this](const QString& filePath) {
        auto filePath_std = filePath.toStdString();
        auto model_asset = ModelManager::getInstance()->get(filePath_std);
        if (model_asset.use_count() <= 2) {
//            qDebug() << "ModelLoader::cacheStart>> Timeout and Not Referenced, Remove Model Cache" << filePath_std.c_str();
            ModelManager::getInstance()->remove(filePath_std);
            this->removeCache(filePath);
        } else {
            // 尝试重新清理
//            qDebug() << "ModelLoader::cacheStart>> Model" << filePath << "use_count=" << model_asset.use_count() << ", Try To Remove Again...";
            this->restartCache(filePath);
        }
    });
}

aiNode *findMeshTransform(const aiScene *scene, aiNode *node, int depth, aiMesh *target)
{
//    qDebug() << "node:" << node->mName.C_Str() << ", nMeshes:" << node->mNumMeshes;
//    auto mat = node->mTransformation;
//    for (int i = 0; i < 4; i++) {
//        qDebug() << mat[i][0] << "\t\t" << mat[i][1] << "\t\t" << mat[i][2] << "\t\t" << mat[i][3];
//    }
    for (size_t j = 0; j < node->mNumMeshes; j++) {
        if (target == scene->mMeshes[node->mMeshes[j]]) {
            return node;
        }
    }
    for (size_t i = 0; i < node->mNumChildren; i++) {
        auto result = findMeshTransform(scene, node->mChildren[i], depth + 1, target);
        if (nullptr != result)
            return result;
    }
    return nullptr;
}

std::shared_ptr<Model> ModelLoader::loadFBX(const QString &filePath, bool doGlobalTransform)
{
    Assimp::Importer importer;
    auto importFlag = aiProcess_CalcTangentSpace |
                      aiProcess_GenNormals |
                      aiProcess_Triangulate |
                      aiProcess_JoinIdenticalVertices |
                      aiProcess_SortByPType;
    if (doGlobalTransform)
        importFlag |= aiProcess_PreTransformVertices;
    auto scene = importer.ReadFile(filePath.toStdString(), importFlag);

    if (!scene) {
        qDebug() << "ModelLoader::loadFBX>> Assimp Scene" << filePath << "Load Failed";
        return nullptr;
    }

    if (!scene->HasMeshes()) {
        qDebug() << "ModelLoader::loadFBX>> Assimp Scene" << filePath << "DONNT Have Any Meshes";
        return nullptr;
    }

    auto model = std::make_shared<Model>();

    auto nMeshes = scene->mNumMeshes;

    float minLim = std::numeric_limits<float>::min();
    float maxLim = std::numeric_limits<float>::max();
    QVector3D max(minLim, minLim, minLim);
    QVector3D min(maxLim, maxLim, maxLim);

    for (unsigned int k = 0; k < nMeshes; k++) {
        auto mesh = std::make_shared<Mesh>();

        auto cur_mesh = scene->mMeshes[k];

        auto nVert = cur_mesh->mNumVertices;
        auto nIndi = cur_mesh->mNumFaces * cur_mesh->mFaces->mNumIndices;

        mesh->vertices = std::vector<QVector3D>(nVert);
        mesh->normals = std::vector<QVector3D>(nVert);
        mesh->uvs = std::vector<QVector2D>(nVert);
        mesh->indices = std::vector<unsigned int>(nIndi);

        auto faces = cur_mesh->mFaces;

        for (unsigned int i = 0; i < cur_mesh->mNumVertices; i++) {
            mesh->vertices[i].setX(cur_mesh->mVertices[i].x);
            mesh->vertices[i].setY(cur_mesh->mVertices[i].y);
            mesh->vertices[i].setZ(cur_mesh->mVertices[i].z);
            mesh->normals [i].setX(cur_mesh->mNormals [i].x);
            mesh->normals [i].setY(cur_mesh->mNormals [i].y);
            mesh->normals [i].setZ(cur_mesh->mNormals [i].z);

            max.setX(qMax(max.x(), mesh->vertices[i].x()));
            max.setY(qMax(max.y(), mesh->vertices[i].y()));
            max.setZ(qMax(max.z(), mesh->vertices[i].z()));
            min.setX(qMin(min.x(), mesh->vertices[i].x()));
            min.setY(qMin(min.y(), mesh->vertices[i].y()));
            min.setZ(qMin(min.z(), mesh->vertices[i].z()));
        }

        for (size_t i = 0; i < cur_mesh->mNumFaces; i++) {
            auto face = faces[i];
            mesh->indices[i * 3    ] = face.mIndices[0];
            mesh->indices[i * 3 + 1] = face.mIndices[1];
            mesh->indices[i * 3 + 2] = face.mIndices[2];
        }

        model->meshes.push_back(mesh);
    }

    model->centroid = (max + min) * .5f;
    model->diagonal = (max - min).length();

    return model;
}

std::shared_ptr<Model> ModelLoader::loadOBJ(const QString &filePath)
{
    return loadFBX(filePath);
}

std::shared_ptr<Model> ModelLoader::loadOFF(const QString &filePath)
{
    return loadFBX(filePath);
}

void ModelLoader::cachedAsyncLoad(const QString &filePath, std::function<void(bool)> loadCallBack)
{
    if (!ModelManager::getInstance()->has(filePath.toStdString())) {
        // load FBX file in async Job
        JobSystem::getInstance()->submit([filePath, loadCallBack, this]() {
            auto fileExt = filePath.split('.').back();
            std::shared_ptr<res::Model> model;
            if (fileExt == "fbx") {
                model = ModelLoader::getInstance()->loadFBX(filePath, true);
            } else if (fileExt == "obj") {
                model = ModelLoader::getInstance()->loadOBJ(filePath);
            } else if (fileExt == "off") {
                model = ModelLoader::getInstance()->loadOFF(filePath);
            } else {
                qDebug() << "ModelLoader::asyncLoad>> Unsupported Model" << filePath << "(.fbx, .obj, .off is Legal)";
                loadCallBack(false);
                return;
            }
            if (model == nullptr) {
                qDebug() << "ModelLoader::asyncLoad>> Model" << filePath << "Load Failed";
                loadCallBack(false);
                return;
            }
//            qDebug() << "ModelLoader::asyncLoad>> Model" << filePath << "Load Successed";
//            model->normalize();
            ModelManager::getInstance()->add(filePath.toStdString(), model);
            emit onAssetLoaded(filePath);
            loadCallBack(true);
        });
    } else {
        emit onAssetLoaded(filePath);
        loadCallBack(true);
    }
}

void ModelLoader::tempAsyncLoad(const QString &filePath, std::function<void (std::shared_ptr<res::Model>)> loadCallBack)
{
    JobSystem::getInstance()->submit([filePath, loadCallBack]() {
        auto fileExt = filePath.split('.').back();
        std::shared_ptr<res::Model> model = nullptr;
        if (fileExt == "fbx") {
            model = ModelLoader::getInstance()->loadFBX(filePath, true);
        } else if (fileExt == "obj") {
            model = ModelLoader::getInstance()->loadOBJ(filePath);
        } else if (fileExt == "off") {
            model = ModelLoader::getInstance()->loadOFF(filePath);
        } else {
            qDebug() << "ModelLoader::tempAsyncLoad>> Unsupported Model" << filePath << "(.fbx, .obj, .off is Legal)";
        }
        if (model == nullptr) {
            qDebug() << "ModelLoader::tempAsyncLoad>> Model" << filePath << "Load Failed";
        }
//        qDebug() << "ModelLoader::tempLoad>> Model" << filePath << "Load Successed";
//        model->normalize();
        loadCallBack(model);
    });
}
