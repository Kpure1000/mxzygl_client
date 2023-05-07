#include "renderer.h"

#include <QDebug>
#include "renderapi.h"
#include "resource/model.h"
#include "resource/bvh.h"
#include "shadermanager.h"

TriangleData::TriangleData(std::shared_ptr<res::Mesh> mesh)
{
    triangle_nums = mesh->facesNum();
    vbo_v = std::make_shared<QOpenGLBuffer>(QOpenGLBuffer::Type::VertexBuffer);
    vbo_v->create();
    vbo_v->bind();
    vbo_v->setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    vbo_v->allocate(mesh->vertices.data(), mesh->verticesNum() * sizeof(QVector3D));

    vbo_n = std::make_shared<QOpenGLBuffer>(QOpenGLBuffer::Type::VertexBuffer);
    vbo_n->create();
    vbo_n->bind();
    vbo_n->setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    vbo_n->allocate(mesh->normals.data(), mesh->verticesNum() * sizeof(QVector3D));

//    vbo_u = std::make_shared<QOpenGLBuffer>(QOpenGLBuffer::Type::VertexBuffer);
//    vbo_u->create();
//    vbo_u->bind();
//    vbo_u->setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
//    vbo_u->allocate(mesh->uvs.data(), mesh->verticesNum() * sizeof(QVector2D));

    ibo = std::make_shared<QOpenGLBuffer>(QOpenGLBuffer::Type::IndexBuffer);
    ibo->create();
    ibo->bind();
    ibo->setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    ibo->allocate(mesh->indices.data(), mesh->facesNum() * 3 * sizeof(unsigned int));
}

void TriangleData::bind(QOpenGLShaderProgram *sprog)
{
    vbo_v->bind();
    sprog->enableAttributeArray("aPos");
    sprog->setAttributeBuffer("aPos", GL_FLOAT, 0, 3, 0);
    vbo_n->bind();
    sprog->enableAttributeArray("aNor");
    sprog->setAttributeBuffer("aNor", GL_FLOAT, 0, 3, 0);
//    vbo_u->bind();
//    sprog->enableAttributeArray("aTex");
//    sprog->setAttributeBuffer("aPos", GL_FLOAT, 0, 2, 0);
    ibo->bind();
}

TriangleData::~TriangleData()
{
    vbo_v->release(QOpenGLBuffer::Type::VertexBuffer);
    vbo_n->release(QOpenGLBuffer::Type::VertexBuffer);
    ibo->release(QOpenGLBuffer::Type::IndexBuffer);
    qDebug() << "TriangleData::~TriangleData";
}

RenderData::RenderData(std::shared_ptr<res::Model> model)
{
    for (auto mesh : model->meshes) {
        triangleDatas.emplace_back(std::make_shared<TriangleData>(mesh));
    }
}

RenderData::RenderData(std::shared_ptr<res::BVH> bvh)
{

}

RenderData::~RenderData()
{

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

void PhongShader::use(std::function<void(QOpenGLShaderProgram *)> func)
{
    this->sprog->bind();
    func(this->sprog.get());
}

Renderer::Renderer(QObject *parent)
    : QObject(parent), m_shader(std::make_shared<PhongShader>())
{

}

Renderer::~Renderer()
{

}

void Renderer::render(QOpenGLContext *context, float dt)
{
    RenderAPI::getInstance()->clear(context);
    if (nullptr == m_renderData)
        return;
    QOpenGLFunctions glf(context);
    m_rot += dt * 0.01f;
    for(auto tri_data : m_renderData->triangleDatas) {
        m_shader->use([tri_data, this](QOpenGLShaderProgram *sprog) {
            // 变换
            tri_data->o2w.setToIdentity();
            tri_data->o2w.translate(0, 0, -1.f);
            tri_data->o2w.rotate(m_rot, 0.0f, 1.0f, 0.0f);
            QMatrix4x4 projMat, viewMat;
            projMat.perspective(60.f, 1.f, 0.1f, 50.f);
            viewMat.lookAt({0.f, 0.f, 0.f}, {0.f, 0.f, -1.f}, {0.f, 1.f, 0.f});
            sprog->setUniformValue("_model", tri_data->o2w);
            sprog->setUniformValue("_view", viewMat);
            sprog->setUniformValue("_proj", projMat);
            sprog->setUniformValue("_normal", tri_data->o2w.normalMatrix());
            // 材质
            sprog->setUniformValue("_material.albedo", {0.7, 0.7, 0.7});
            sprog->setUniformValue("_material.diffuse", {1.0, 1.0, 1.0});
            sprog->setUniformValue("_material.specular", {0.7, 0.7, 0.7});
            sprog->setUniformValue("_material.shininess", 16.f);
            // 直射光
            sprog->setUniformValue("_light.intensity", 0.5f);
            sprog->setUniformValue("_light.ambient", {0.3f, 0.3f, 0.3f});
            sprog->setUniformValue("_light.diffuse", {1.0f, 1.0f, 1.0f});
            sprog->setUniformValue("_light.specular", {0.6f, 0.6f, 0.6f});
            sprog->setUniformValue("_light.direction", {0.5f, 0.5f, 0.5f});

            tri_data->bind(sprog);
        });
        RenderAPI::getInstance()->drawTriangle(context, tri_data->triangle_nums);
    }
}

void Renderer::setRenderData(std::shared_ptr<RenderData> renderData)
{
    m_renderData = renderData;
}

void Renderer::clearRenderData()
{
    m_renderData.reset();
}
