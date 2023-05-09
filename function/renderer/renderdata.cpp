#include "renderdata.h"

#include <QDebug>

#include "resource/model.h"
#include "resource/bvh.h"
#include "shadermanager.h"

TriangleData::TriangleData(std::shared_ptr<res::Mesh> mesh)
{
    triangle_nums = mesh->facesNum();
//    qDebug() << "TriangleData::TriangleData>> start" << mesh->facesNum();
    vbo_v = std::make_shared<QOpenGLBuffer>(QOpenGLBuffer::Type::VertexBuffer);
    if (!vbo_v->create()) {
        qDebug() << "TriangleData::TriangleData>> vbo_v create failed";
    }
    vbo_v->bind();
    vbo_v->setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    vbo_v->allocate(mesh->vertices.data(), mesh->verticesNum() * sizeof(QVector3D));

    if (!mesh->normals.empty()) {
        vbo_n = std::make_shared<QOpenGLBuffer>(QOpenGLBuffer::Type::VertexBuffer);
        if (!vbo_n->create()) {
            qDebug() << "TriangleData::TriangleData>> vbo_n create failed";
        }
        vbo_n->bind();
        vbo_n->setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
        vbo_n->allocate(mesh->normals.data(), mesh->verticesNum() * sizeof(QVector3D));
    }

    //    vbo_u = std::make_shared<QOpenGLBuffer>(QOpenGLBuffer::Type::VertexBuffer);
    //    if (!vbo_u->create()){
    //        qDebug() << "TriangleData::TriangleData>> vbo_u create failed";
    //    }
    //    vbo_u->bind();
    //    vbo_u->setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    //    vbo_u->allocate(mesh->uvs.data(), mesh->verticesNum() * sizeof(QVector2D));

    ibo = std::make_shared<QOpenGLBuffer>(QOpenGLBuffer::Type::IndexBuffer);
    if (!ibo->create()) {
        qDebug() << "TriangleData::TriangleData>> ibo create failed";
    }
    ibo->bind();
    ibo->setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    ibo->allocate(mesh->indices.data(), mesh->facesNum() * 3 * sizeof(unsigned int));
//    qDebug() << "TriangleData::TriangleData>> end" << mesh->facesNum();
}

void TriangleData::bind(QOpenGLShaderProgram *sprog)
{
    vbo_v->bind();
    sprog->enableAttributeArray("aPos");
    sprog->setAttributeBuffer("aPos", GL_FLOAT, 0, 3, 0);
    if (vbo_n) {
        vbo_n->bind();
        sprog->enableAttributeArray("aNor");
        sprog->setAttributeBuffer("aNor", GL_FLOAT, 0, 3, 0);
    }
    if (vbo_u) {
        vbo_u->bind();
        sprog->enableAttributeArray("aTex");
        sprog->setAttributeBuffer("aTex", GL_FLOAT, 0, 2, 0);
    }
    ibo->bind();
}

TriangleData::~TriangleData()
{
    vbo_v->release(QOpenGLBuffer::Type::VertexBuffer);
    if (vbo_n)
        vbo_n->release(QOpenGLBuffer::Type::VertexBuffer);
    if (vbo_u)
        vbo_u->release(QOpenGLBuffer::Type::VertexBuffer);
    ibo->release(QOpenGLBuffer::Type::IndexBuffer);
//    qDebug() << "TriangleData::~TriangleData";
}

RenderData::RenderData(std::shared_ptr<res::Model> model, int sw, int sh)
    : camera(std::make_shared<PerspectiveCamera>(static_cast<float>(sw) / sh, .1f, 100.f, 60.f))
{
    trans_ca = Transform{QVector3D{.0f, .0f, .0f},
                         QQuaternion{1.f, QVector3D{.0f, .0f, .0f}},
                         QVector3D{.0f, .0f, .0f}};
    trans_model = Transform({.0f, .0f, -1.2f}, QQuaternion{1.f, {.0f, .0f, .0f}}, {1.f, 1.f, 1.f});
    for (auto mesh : model->meshes) {
        triangleDatas.push_back(std::make_shared<TriangleData>(mesh));
    }
}

RenderData::RenderData(std::shared_ptr<res::BVH> bvh, int sw, int sh)
    : camera(std::make_shared<PerspectiveCamera>(static_cast<float>(sw) / sh, .1f, 100.f, 60.f))
{
    trans_ca = Transform{QVector3D{.0f, .0f, .0f},
                         QQuaternion{1.f, QVector3D{.0f, .0f, .0f}},
                         QVector3D{.0f, .0f, .0f}};
    trans_model = Transform({.0f, .0f, -1.2f}, QQuaternion{1.f, {.0f, .0f, .0f}}, {1.f, 1.f, 1.f});
}

RenderData::~RenderData()
{
//    qDebug() << "RenderData::~RenderData";
}

IShader::IShader(const std::string &name) : name(name)
{
}

IShader::~IShader()
{
    sprog->release();
    sprog.reset();
}

PhongShader::PhongShader(const std::string &name) : IShader(name)
{
    sprog.reset(ShaderManager::getInstance()->load(this->name,
                                                   ":/assets/assets/shader/phong.vert",
                                                   ":/assets/assets/shader/phong.frag"));
}

PhongShader::~PhongShader()
{

}

//void PhongShader::use(std::function<void(QOpenGLShaderProgram *)> func)
//{
//    this->sprog->bind();
//    func(this->sprog.get());
//}

SkyShader::SkyShader(const std::string &name) : IShader(name)
{
    sprog.reset(ShaderManager::getInstance()->load(this->name,
                                                   ":/assets/assets/shader/skybox.vert",
                                                   ":/assets/assets/shader/skybox.frag"));
}

SkyShader::~SkyShader() {}

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
