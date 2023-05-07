#include "modelloader.h"

#include <QRunnable>
#include <QThreadPool>
#include <QMatrix4x4>
#include <QFileInfo>

#include <fstream>

#include "utils/jobsystem.h"
#include "3rd_part/openfbx/ofbx.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "3rd_part/tiny_obj_loader/tiny_obj_loader.h"

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

std::shared_ptr<Model> ModelLoader::loadFBX(const QString &filePath, bool doGlobalTransform)
{
    FILE *fp = fopen(filePath.toStdString().c_str(), "rb");

    if (!fp) {
        qDebug() << "ModelLoader::loadFBX>> cannot open file" << filePath;
        return nullptr;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    auto *content = new ofbx::u8[file_size];
    fread(content, 1, file_size, fp);

    // Ignoring certain nodes will only stop them from being processed not tokenised (i.e. they will still be in the tree)
    ofbx::LoadFlags flags =
        ofbx::LoadFlags::TRIANGULATE |
//    	ofbx::LoadFlags::IGNORE_MODELS |
        ofbx::LoadFlags::IGNORE_BLEND_SHAPES |
        ofbx::LoadFlags::IGNORE_CAMERAS |
        ofbx::LoadFlags::IGNORE_LIGHTS |
        ofbx::LoadFlags::IGNORE_TEXTURES |
        ofbx::LoadFlags::IGNORE_SKIN |
        ofbx::LoadFlags::IGNORE_BONES |
        ofbx::LoadFlags::IGNORE_PIVOTS |
        ofbx::LoadFlags::IGNORE_MATERIALS |
        ofbx::LoadFlags::IGNORE_POSES |
        ofbx::LoadFlags::IGNORE_VIDEOS |
        ofbx::LoadFlags::IGNORE_LIMBS |
//		ofbx::LoadFlags::IGNORE_MESHES |
        ofbx::LoadFlags::IGNORE_ANIMATIONS;

//    auto jobProcessor = [](ofbx::JobFunction jfn, void *, void *data, ofbx::u32 size, ofbx::u32 count) {
//        std::vector<std::future<void>> vresult;
//        qDebug() << "count: " << count;
//        for (ofbx::u32 i = 0; i < count; i++) {
//            vresult.push_back(JobSystem::getInstance()->submit([i, jfn, data, size]() {
//                ofbx::u8 *ptr = (ofbx::u8 *) data;
//                jfn(ptr + i * size);
//                return;
//            }));
//        }
//        for (auto &res : vresult) {
//            res.wait();
//        }
//    };

    ofbx::IScene *fbxscene = ofbx::load((ofbx::u8 *) content, file_size, (ofbx::u16) flags);

    if (nullptr == fbxscene) {
        qDebug() << "ModelLoader::loadFBX>> FBX Scene Load Failed";
        return nullptr;
    }

    auto model = std::make_shared<Model>();

    auto nMeshes = fbxscene->getMeshCount();

    float minLim = std::numeric_limits<float>::min();
    float maxLim = std::numeric_limits<float>::max();
    QVector3D max(minLim, minLim, minLim);
    QVector3D min(maxLim, maxLim, maxLim);

    for (int i = 0; i < nMeshes; i++) {
        auto mesh = std::make_shared<Mesh>();

        auto geom = fbxscene->getMesh(i)->getGeometry();

        bool has_normal = !(nullptr == geom->getNormals());
        bool has_uvs = !(nullptr == geom->getUVs());

        auto nVert = geom->getVertexCount();
        auto nIndi = geom->getIndexCount();

        mesh->vertices = std::vector<QVector3D>(nVert);
        mesh->normals = std::vector<QVector3D>(nVert);
        mesh->uvs = std::vector<QVector2D>(nVert);
        mesh->indices = std::vector<unsigned int>(nIndi);

        QMatrix4x4 localMatV, localMatN;
        if (doGlobalTransform) {
            auto localTrans = geom->getGlobalTransform();
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    float mv = static_cast<float>(localTrans.m[i * 4 + j]);
                    localMatV.data()[i * 4 + j] = mv;
                    if (i < 3 && j < 3)
                        localMatN.data()[i * 4 + j] = mv;
                }
            }
        }

        auto indi =  geom->getFaceIndices();
        auto vert = geom->getVertices();
        auto norm = geom->getNormals();
        auto uvs = geom->getUVs();

        for (int index = 0; index < nIndi; index++) {
            auto vid_current = indi[index] >= 0 ? indi[index] : -indi[index] - 1;

            // vertices
            auto v = vert[vid_current];
            float vx = static_cast<float>(v.x);
            float vy = static_cast<float>(v.y);
            float vz = static_cast<float>(v.z);
            mesh->vertices[vid_current] = localMatV * QVector3D{vx,vy,vz};
            auto rv = mesh->vertices[vid_current];
            vx=rv.x();
            vy=rv.y();
            vz=rv.z();

            max.setX(qMax(max.x(), vx));
            max.setY(qMax(max.y(), vy));
            max.setZ(qMax(max.z(), vz));
            min.setX(qMin(min.x(), vx));
            min.setY(qMin(min.y(), vy));
            min.setZ(qMin(min.z(), vz));

            // normals
            if (has_normal) {
                if (nullptr != norm){
                    auto vn = norm[vid_current];
                    mesh->normals[vid_current] += localMatN * QVector3D{
                                                      static_cast<float>(vn.x),
                                                      static_cast<float>(vn.y),
                                                      static_cast<float>(vn.z)
                                                  };
                }
            }

            // texCoords
            if (has_uvs) {
                if (nullptr != uvs) {
                    auto vt = uvs[vid_current];
                    mesh->uvs[vid_current] = {
                        static_cast<float>(vt.x),
                        static_cast<float>(vt.y)
                    };
                }
            }
            mesh->indices[index] = vid_current;
        }

        if (has_normal) {
            for (int i = 0; i < nVert; i++) {
                mesh->normals[i].normalize();
            }
        }

        model->meshes.push_back(mesh);
    }

    model->centroid = (max + min) * .5f;
    model->diagonal = (max - min).length();

    return model;
}

std::shared_ptr<Model> ModelLoader::loadOBJ(const QString &filePath)
{
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./"; // Path to material files

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(filePath.toStdString(), reader_config)) {
        if (!reader.Error().empty()) {
            qDebug() << "ModelLoader::loadOBJ>> " << reader.Error().c_str();
        }
        return nullptr;
    }

    if (!reader.Warning().empty()) {
        qDebug() << "ModelLoader::loadOBJ>> " << reader.Warning().c_str();
    }

    auto mesh = std::make_shared<Mesh>();

    auto &attrib = reader.GetAttrib();
    auto &shapes = reader.GetShapes();
    int nFaces = 0;
    for (auto &shape : shapes) {
        nFaces += shape.mesh.num_face_vertices.size();
    }
    int nVert = attrib.vertices.size();
    mesh->vertices = std::vector<QVector3D>(nVert);
    mesh->normals = std::vector<QVector3D>(nVert);
    mesh->uvs = std::vector<QVector2D>(nVert);
    mesh->indices = std::vector<unsigned int>(nFaces * 3);

    float minLim = std::numeric_limits<float>::min();
    float maxLim = std::numeric_limits<float>::max();
    QVector3D max(minLim, minLim, minLim);
    QVector3D min(maxLim, maxLim, maxLim);

    int index_offset_shape = 0;
    for (auto &shape : shapes) {

        int nfaces = shape.mesh.num_face_vertices.size();
        size_t index_offset = 0;
        for (int f = 0; f < nfaces; f++) {
            size_t fv = size_t(shape.mesh.num_face_vertices[f]);
            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                auto v_indi = idx.vertex_index;

                mesh->indices[index_offset_shape + index_offset + v] = v_indi;

                float vx = attrib.vertices[3 * size_t(v_indi) + 0];
                float vy = attrib.vertices[3 * size_t(v_indi) + 1];
                float vz = attrib.vertices[3 * size_t(v_indi) + 2];

                mesh->vertices[v_indi] = {vx, vy, vz};

                max.setX(qMax(max.x(), vx));
                max.setY(qMax(max.y(), vy));
                max.setZ(qMax(max.z(), vz));
                min.setX(qMin(min.x(), vx));
                min.setY(qMin(min.y(), vy));
                min.setZ(qMin(min.z(), vz));

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    float nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    float ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    float nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                    mesh->normals[v_indi] += {nx, ny, nz};
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    float tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    float ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    mesh->uvs[v_indi] = {tx, ty};
                }
            }
            index_offset += fv;
        }
        index_offset_shape += index_offset;
    }

    auto model = std::make_shared<Model>();
    model->meshes.push_back(mesh);

    model->centroid = (max + min) * .5f;
    model->diagonal = (max - min).length();

    return model;
}

std::shared_ptr<Model> ModelLoader::loadOFF(const QString &filePath)
{
    return nullptr;
}

void ModelLoader::asyncLoad(const QString &filePath, std::function<void()> loadCallBack)
{
    if (!ModelManager::getInstance()->has(filePath.toStdString())) {
        // load FBX file in async Job
        JobSystem::getInstance()->submit([filePath, loadCallBack, this]() {
            auto fileExt = filePath.split('.').back();
            std::shared_ptr<res::Model> model;
            if (fileExt == "fbx") {
                model = ModelLoader::getInstance()->loadFBX(filePath);
            } else if (fileExt == "obj") {
                model = ModelLoader::getInstance()->loadOBJ(filePath);
            } else if (fileExt == "off") {
                model = ModelLoader::getInstance()->loadOFF(filePath);
            } else {
                qDebug() << "ModelLoader::asyncLoad>> Unsupported Model" << filePath << "(.fbx, .obj, .off is Legal)";
                return;
            }
            if (model == nullptr) {
                qDebug() << "ModelLoader::asyncLoad>> Model" << filePath << "Load Failed";
                return;
            }
            model->normalize();
            ModelManager::getInstance()->add(filePath.toStdString(), model);
            emit onAssetLoaded(filePath);
            loadCallBack();
        });
    } else {
        emit onAssetLoaded(filePath);
        loadCallBack();
    }
}
