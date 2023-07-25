#include "modelloader.h"

#include <cmath>
#include <QMatrix4x4>
#include <QFileInfo>
#include <QDebug>

#include "utils/jobsystem.h"
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
            ModelManager::getInstance()->remove(filePath_std);
            this->removeCache(filePath);
        } else {
            // 尝试重新清理
            this->restartCache(filePath);
        }
    });
}

std::shared_ptr<Model> ModelLoader::loadFBX(const QString &filePath, bool doGlobalTransform, bool doNormalize)
{
    Assimp::Importer importer;
    if (doNormalize)
        importer.SetPropertyBool(AI_CONFIG_PP_PTV_NORMALIZE, true);
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
        }

        for (size_t i = 0; i < cur_mesh->mNumFaces; i++) {
            auto face = faces[i];
            mesh->indices[i * 3    ] = face.mIndices[0];
            mesh->indices[i * 3 + 1] = face.mIndices[1];
            mesh->indices[i * 3 + 2] = face.mIndices[2];
        }

        model->meshes.push_back(mesh);
    }

    return model;
}

std::shared_ptr<Model> ModelLoader::loadOBJ(const QString &filePath, bool doNormalize)
{
    return loadFBX(filePath, true, doNormalize);
}

std::shared_ptr<Model> ModelLoader::loadOFF(const QString &filePath, bool doNormalize)
{
    return loadFBX(filePath, true, doNormalize);
}

void ModelLoader::cachedAsyncLoad(const QString &filePath, std::function<void(bool)> loadCallBack)
{
    if (!ModelManager::getInstance()->has(filePath.toStdString())) {
        // load FBX file in async Job
        JobSystem::getInstance()->submit([filePath, loadCallBack, this]() {
            auto fileExt = filePath.split('.').back();
            std::shared_ptr<res::Model> model;
            if (fileExt == "fbx") {
                model = ModelLoader::getInstance()->loadFBX(filePath, true, true);
            } else if (fileExt == "obj") {
                model = ModelLoader::getInstance()->loadOBJ(filePath, true);
            } else if (fileExt == "off") {
                model = ModelLoader::getInstance()->loadOFF(filePath, true);
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
            ModelManager::getInstance()->add(filePath.toStdString(), model);
            int v_count = 0;
            for (auto &mesh : model->meshes) {
                v_count += mesh->verticesNum();
            }
            qDebug() << QString("模型 '%1' 顶点数: %2.").arg(filePath, v_count);
            emit onAssetLoaded(filePath);
            loadCallBack(true);
        });
    } else {
        int v_count = 0;
        auto model = ModelManager::getInstance()->get(filePath.toStdString());
        for (auto &mesh : model->meshes) {
            v_count += mesh->verticesNum();
        }
        qDebug() << QString("模型 '%1' 顶点数: %2.").arg(filePath, v_count);
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
            model = ModelLoader::getInstance()->loadFBX(filePath, true, false);
        } else if (fileExt == "obj") {
            model = ModelLoader::getInstance()->loadOBJ(filePath, false);
        } else if (fileExt == "off") {
            model = ModelLoader::getInstance()->loadOFF(filePath, false);
        } else {
            qDebug() << "ModelLoader::tempAsyncLoad>> Unsupported Model" << filePath << "(.fbx, .obj, .off is Legal)";
        }
        if (model == nullptr) {
            qDebug() << "ModelLoader::tempAsyncLoad>> Model" << filePath << "Load Failed";
        }
        if (model) {
            int v_count = 0;
            for (auto &mesh : model->meshes) {
                v_count += mesh->verticesNum();
            }
            qDebug() << QString(R"(模型 '%1', 顶点数: %2)").arg(filePath).arg(v_count);
        }
        loadCallBack(model);
    });
}
