#include "renderdata.h"

#include <QDebug>

#include "resource/model.h"
#include "resource/bvh.h"
#include "shadermanager.h"
#include "camera.h"

TriangleData::TriangleData(std::shared_ptr<res::Mesh> mesh)
{
    triangle_nums = mesh->facesNum();
    vertices_nums = mesh->verticesNum();

    vbo_v = std::make_shared<QOpenGLBuffer>(QOpenGLBuffer::Type::VertexBuffer);
    if (!vbo_v->create()) {
        qDebug() << "TriangleData::TriangleData>> vbo_v create failed";
    }
    vbo_v->bind();
    vbo_v->setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    vbo_v->allocate(mesh->vertices.data(), mesh->verticesNum() * sizeof(QVector3D));

    vbo_n = std::make_shared<QOpenGLBuffer>(QOpenGLBuffer::Type::VertexBuffer);
    if (!vbo_n->create()) {
        qDebug() << "TriangleData::TriangleData>> vbo_n create failed";
    }
    vbo_n->bind();
    vbo_n->setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    vbo_n->allocate(mesh->normals.data(), mesh->verticesNum() * sizeof(QVector3D));

    ibo = std::make_shared<QOpenGLBuffer>(QOpenGLBuffer::Type::IndexBuffer);
    if (!ibo->create()) {
        qDebug() << "TriangleData::TriangleData>> ibo create failed";
    }
    ibo->bind();
    ibo->setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    ibo->allocate(mesh->indices.data(), mesh->facesNum() * 3 * sizeof(unsigned int));
}

void TriangleData::bind(QOpenGLShaderProgram *sprog)
{
    vbo_v->bind();
    sprog->enableAttributeArray(0);
    sprog->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    vbo_n->bind();
    sprog->enableAttributeArray(1);
    sprog->setAttributeBuffer(1, GL_FLOAT, 0, 3, 0);

    ibo->bind();
}

TriangleData::~TriangleData()
{
    vbo_v->release(QOpenGLBuffer::Type::VertexBuffer);
    vbo_n->release(QOpenGLBuffer::Type::VertexBuffer);
    ibo->release(QOpenGLBuffer::Type::IndexBuffer);
}

BoneData::BoneData(std::shared_ptr<res::BVH> bvh)
    : bvh(bvh)
{
    if (bvh->boneMeshes.empty()) {
        qDebug("BoneData::BoneData>> BVH is empty");
        return;
    }
    triangle_nums = bvh->boneMeshes[0]->indices.size() / 3;
    vertices_nums = bvh->boneMeshes[0]->vertices.size();
    vbo_v = std::make_shared<QOpenGLBuffer>(QOpenGLBuffer::Type::VertexBuffer);
    if (!vbo_v->create()) {
        qDebug() << "BoneData::BoneData>> vbo_v create failed";
    }
    vbo_v->bind();
    vbo_v->setUsagePattern(QOpenGLBuffer::UsagePattern::DynamicDraw);
    vbo_v->allocate(vertices_nums * sizeof(QVector3D));

    vbo_n = std::make_shared<QOpenGLBuffer>(QOpenGLBuffer::Type::VertexBuffer);
    if (!vbo_n->create()) {
        qDebug() << "BoneData::BoneData>> vbo_n create failed";
    }
    vbo_n->bind();
    vbo_n->setUsagePattern(QOpenGLBuffer::UsagePattern::DynamicDraw);
    vbo_n->allocate(vertices_nums * sizeof(QVector3D));

    vbo_t = std::make_shared<QOpenGLBuffer>(QOpenGLBuffer::Type::VertexBuffer);
    if (!vbo_t->create()) {
        qDebug() << "BoneData::BoneData>> vbo_t create failed";
    }
    vbo_t->bind();
    vbo_t->setUsagePattern(QOpenGLBuffer::UsagePattern::DynamicDraw);
    vbo_t->allocate(vertices_nums * sizeof(QMatrix4x4));

    vbo_tn = std::make_shared<QOpenGLBuffer>(QOpenGLBuffer::Type::VertexBuffer);
    if (!vbo_tn->create()) {
        qDebug() << "BoneData::BoneData>> vbo_tn create failed";
    }
    vbo_tn->bind();
    vbo_tn->setUsagePattern(QOpenGLBuffer::UsagePattern::DynamicDraw);
    vbo_tn->allocate(vertices_nums * sizeof(QMatrix3x3));

    ibo = std::make_shared<QOpenGLBuffer>(QOpenGLBuffer::Type::IndexBuffer);
    if (!ibo->create()) {
        qDebug() << "BoneData::BoneData>> ibo create failed";
    }
    ibo->bind();
    ibo->setUsagePattern(QOpenGLBuffer::UsagePattern::DynamicDraw);
    ibo->allocate(triangle_nums * 3 * sizeof(unsigned int));

    resetBuffer(bvh->boneMeshes[0]);
}

void BoneData::resetBuffer(std::shared_ptr<res::BoneMesh> boneMesh)
{
    vbo_v->bind();
    vbo_v->write(0, boneMesh->vertices.data(), boneMesh->vertices.size() * sizeof(QVector3D));

    vbo_n->bind();
    vbo_n->write(0, boneMesh->normals.data(), boneMesh->normals.size() * sizeof(QVector3D));

    vbo_t->bind();
    auto trans_size = boneMesh->trans_r0.size() * sizeof(QVector4D);
    vbo_t->write(0             , boneMesh->trans_r0.data(), trans_size);
    vbo_t->write(trans_size    , boneMesh->trans_r1.data(), trans_size);
    vbo_t->write(trans_size * 2, boneMesh->trans_r2.data(), trans_size);
    vbo_t->write(trans_size * 3, boneMesh->trans_r3.data(), trans_size);

    vbo_tn->bind();
    auto trans_nor_size = boneMesh->trans_nor_r0.size() * sizeof(QVector3D);
    vbo_tn->write(0                 , boneMesh->trans_nor_r0.data(), trans_nor_size);
    vbo_tn->write(trans_nor_size    , boneMesh->trans_nor_r1.data(), trans_nor_size);
    vbo_tn->write(trans_nor_size * 2, boneMesh->trans_nor_r2.data(), trans_nor_size);

    ibo->bind();
    ibo->write(0, boneMesh->indices.data(), boneMesh->indices.size() * sizeof(unsigned int));
}

void BoneData::bind(QOpenGLShaderProgram *sprog)
{
    vbo_v->bind();
    sprog->enableAttributeArray(0);
    sprog->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    vbo_n->bind();
    sprog->enableAttributeArray(1);
    sprog->setAttributeBuffer(1, GL_FLOAT, 0, 3, 0);

    unsigned int offset = 0;
    vbo_t->bind();
    sprog->enableAttributeArray(2);
    sprog->setAttributeBuffer(2, GL_FLOAT, offset, 4, 0);
    offset += sizeof(float) * 4 * vertices_nums;

    sprog->enableAttributeArray(3);
    sprog->setAttributeBuffer(3, GL_FLOAT, offset, 4, 0);
    offset += sizeof(float) * 4 * vertices_nums;

    sprog->enableAttributeArray(4);
    sprog->setAttributeBuffer(4, GL_FLOAT, offset, 4, 0);
    offset += sizeof(float) * 4 * vertices_nums;

    sprog->enableAttributeArray(5);
    sprog->setAttributeBuffer(5, GL_FLOAT, offset, 4, 0);

    offset = 0;
    vbo_tn->bind();
    sprog->enableAttributeArray(6);
    sprog->setAttributeBuffer(6, GL_FLOAT, offset, 3, 0);
    offset += sizeof(float) * 3 * vertices_nums;

    sprog->enableAttributeArray(7);
    sprog->setAttributeBuffer(7, GL_FLOAT, offset, 3, 0);
    offset += sizeof(float) * 3 * vertices_nums;

    sprog->enableAttributeArray(8);
    sprog->setAttributeBuffer(8, GL_FLOAT, offset, 3, 0);

    ibo->bind();
}

BoneData::~BoneData()
{
    vbo_v->release(QOpenGLBuffer::Type::VertexBuffer);
    vbo_n->release(QOpenGLBuffer::Type::VertexBuffer);
    vbo_t->release(QOpenGLBuffer::Type::VertexBuffer);
    vbo_tn->release(QOpenGLBuffer::Type::VertexBuffer);
    ibo->release(QOpenGLBuffer::Type::IndexBuffer);
}

RenderData::RenderData(std::shared_ptr<res::Model> model, int sw, int sh)
    : camera(std::make_shared<PerspectiveCamera>(static_cast<float>(sw) / sh, .1f, 100.f, 60.f))
{
    for (auto mesh : model->meshes) {
        triangleDatas.push_back(std::make_shared<TriangleData>(mesh));
    }
}

RenderData::RenderData(std::shared_ptr<res::BVH> bvh, int sw, int sh)
    : camera(std::make_shared<PerspectiveCamera>(static_cast<float>(sw) / sh, .1f, 100.f, 60.f)), boneData(std::make_shared<BoneData>(bvh))
{
}

void RenderData::tick(float dt)
{
    if (boneData && triangleDatas.size() == 0) { // render bvh
        qDebug("ctime=%f , cframe=%d", boneData->current_time, boneData->current_frame);
        boneData->current_time += dt;
        if (boneData->current_time >= 1000.0f / boneData->bvh->ssp) {
            boneData->current_time = 0;
            boneData->current_frame++;
            boneData->current_frame %= boneData->bvh->nFrames;
            boneData->resetBuffer(boneData->bvh->boneMeshes[boneData->current_frame]);
        }
    }
}

RenderData::~RenderData()
{
//    qDebug() << "RenderData::~RenderData";
}

IShader::IShader(const std::string &name, QObject *parent) : QObject(parent), name(name)
{
}

IShader::~IShader()
{
//    qDebug() << "IShader::~IShader";
    sprog->release();
    sprog.reset();
}

StaticModelLightShader::StaticModelLightShader(const std::string &name, QObject *parent): IShader(name, parent)
{
    sprog.reset(ShaderManager::getInstance()->load(this->name,
                                                   ":/assets/assets/shader/static.vert",
                                                   ":/assets/assets/shader/phong.frag"));
}

StaticModelLightShader::~StaticModelLightShader()
{
//    qDebug() << "PhongShader::~PhongShader";
}

//void PhongShader::use(std::function<void(QOpenGLShaderProgram *)> func)
//{
//    this->sprog->bind();
//    func(this->sprog.get());
//}

AnimationModelLightShader::AnimationModelLightShader(const std::string &name, QObject *parent): IShader(name, parent)
{
    sprog.reset(ShaderManager::getInstance()->load(this->name,
                                                   ":/assets/assets/shader/animation.vert",
                                                   ":/assets/assets/shader/phong.frag"));
}

AnimationModelLightShader::~AnimationModelLightShader()
{

}

SkyShader::SkyShader(const std::string &name, QObject *parent) : IShader(name, parent)
{
    sprog.reset(ShaderManager::getInstance()->load(this->name,
                                                   ":/assets/assets/shader/skybox.vert",
                                                   ":/assets/assets/shader/skybox.frag"));
}

SkyShader::~SkyShader()
{
//    qDebug() << "SkyShader::~SkyShader";
}

SkyData::SkyData(const QVector3D &sky, const QVector3D &ground)
    : sky(sky), ground(ground)
{
    auto mesh = std::make_shared<res::Mesh>();
    mesh->vertices = {
        {-1.f, -1.f, .0f},
        { 1.f, -1.f, .0f},
        {-1.f,  1.f, .0f},
        { 1.f,  1.f, .0f},
    };
    mesh->indices = {
        0, 1, 2,
        2, 1, 3
    };
    triangleData = std::make_shared<TriangleData>(mesh);
}
