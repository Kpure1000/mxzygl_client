#include "renderer.h"

#include <QDebug>

#include "renderapi.h"
#include "renderdata.h"
#include "resource/model.h"
#include "resource/bvh.h"

Renderer::Renderer(int sw, int sh, QObject *parent)
    : QObject(parent)
    , m_sw(sw)
    , m_sh(sh)
{
    m_phongShader = std::make_shared<PhongShader>();
    m_skyShader = std::make_shared<SkyShader>();
    m_skyData = std::make_shared<SkyData>(QVector3D{0.2f, 0.3f, 0.48f}, QVector3D{0.4f, 0.3f, 0.2f});
    m_arcBall = std::make_shared<ArcBall>(sw, sh);
}

Renderer::~Renderer()
{

}

void Renderer::resize(QOpenGLContext *context, int w, int h)
{
    m_sw = w;
    m_sh = h;

    if (m_meshesData)
        m_meshesData->camera->aspect = static_cast<float>(w) / h;
    m_arcBall->resize(w, h);

    RenderAPI::getInstance()->resize(context, w, h);
}

void Renderer::push_input(const InputData &input)
{
    switch (input.type) {
    case InputData::InputType::KEY: {
        for (int i = 0; i < m_input.nKeyEvents; i++) {
            m_input.key_event[i] |= input.key_event[i];
        }
        break;
    }
    case InputData::InputType::MOUSE: {
        m_input.pos = input.pos;
        for (int i = 0; i < m_input.nMouseEvents; i++) {
            m_input.mouse_event[i] |= input.mouse_event[i];
        }
        break;
    }
    case InputData::InputType::WHEEL: {
        m_input.scrollx = input.scrollx;
        m_input.scrolly = input.scrolly;
        break;
    }
    }
}

void Renderer::logic_tick(float dt)
{
    if (m_meshesData) {
        // 缩放处理
        auto zoom = -m_input.scrolly;
        auto scale = &m_meshesData->trans_model.scale;
        scale->setX(std::max(.5f, std::min(6.f, scale->x() + zoom * dt * 0.0003f)));
        scale->setY(std::max(.5f, std::min(6.f, scale->y() + zoom * dt * 0.0003f)));
        scale->setZ(std::max(.5f, std::min(6.f, scale->z() + zoom * dt * 0.0003f)));
        // ArcBall处理
        m_arcBall->tick(m_input, dt);
        m_meshesData->trans_model.rotation = m_arcBall->getRotation();
    }
}

void Renderer::render_tick(QOpenGLContext *context)
{
    context = QOpenGLContext::currentContext();

    RenderAPI::getInstance()->enableDepth(context, true);
    RenderAPI::getInstance()->clearAll(context);
    if (nullptr == m_meshesData)
        return;

    // 渲染天空
    auto shSky = m_skyShader->sprog;
    shSky->bind();
    shSky->setUniformValue("_sky", m_skyData->sky);
    shSky->setUniformValue("_ground", m_skyData->ground);
    shSky->setUniformValue("_pitch", 0.0f);
    m_skyData->triangleData->bind(shSky.get());
    RenderAPI::getInstance()->drawTriangle(context, m_skyData->triangleData->triangle_nums);

    RenderAPI::getInstance()->clearDepth(context);
    auto shPhong = m_phongShader->sprog;
    shPhong->bind();
    // 模型变换
    auto modelMat = m_meshesData->trans_model.get_trans_mat();
    shPhong->setUniformValue("_model", modelMat);
    shPhong->setUniformValue("_normal", modelMat.normalMatrix());
    // 相机变换
    auto camera = m_meshesData->camera;
    shPhong->setUniformValue("_view", camera->get_view_mat(m_meshesData->trans_ca));
    shPhong->setUniformValue("_proj", camera->get_projection_mat());
    // 材质
    shPhong->setUniformValue("_material.albedo", {0.8f, 0.8f, 0.8f});
    shPhong->setUniformValue("_material.diffuse", {1.0f, 1.0f, 1.0f});
    shPhong->setUniformValue("_material.specular", {1.0f, 1.0f, 1.0f});
    shPhong->setUniformValue("_material.shininess", 16.f);
    // 直射光1
    shPhong->setUniformValue("_light_1.intensity", 1.0f);
    shPhong->setUniformValue("_light_1.ambient", {0.4f, 0.4f, 0.4f});
    shPhong->setUniformValue("_light_1.diffuse", {0.8f, 0.8f, 0.8f});
    shPhong->setUniformValue("_light_1.specular", {0.9f, 0.9f, 0.9f});
    shPhong->setUniformValue("_light_1.position", {0.1f, 0.5f, 1.5f});
    shPhong->setUniformValue("_light_1._constant",  1.0f);
    shPhong->setUniformValue("_light_1._linear",    0.22f);
    shPhong->setUniformValue("_light_1._quadratic", 0.2f);
    // 直射光2
    shPhong->setUniformValue("_light_2.intensity", 1.0f);
    shPhong->setUniformValue("_light_2.ambient", {0.1f, 0.1f, 0.1f});
    shPhong->setUniformValue("_light_2.diffuse", {0.2f, 0.3f, 0.48f});
    shPhong->setUniformValue("_light_2.specular", {0.2f, 0.3f, 0.48f});
    shPhong->setUniformValue("_light_2.position", {-0.2f, 2.3f, 0.5f});
    shPhong->setUniformValue("_light_2._constant",  1.0f);
    shPhong->setUniformValue("_light_2._linear",    0.22f);
    shPhong->setUniformValue("_light_2._quadratic", 0.2f);
    // 直射光3
    shPhong->setUniformValue("_light_3.intensity", 1.0f);
    shPhong->setUniformValue("_light_3.ambient", {0.1f, 0.1f, 0.1f});
    shPhong->setUniformValue("_light_3.diffuse", {0.4f, 0.3f, 0.2f});
    shPhong->setUniformValue("_light_3.specular", {0.4f, 0.3f, 0.2f});
    shPhong->setUniformValue("_light_3.position", {0.2f, -2.3f, 0.5f});
    shPhong->setUniformValue("_light_3._constant",  1.0f);
    shPhong->setUniformValue("_light_3._linear",    0.22f);
    shPhong->setUniformValue("_light_3._quadratic", 0.2f);

    for (auto tri_data : m_meshesData->triangleDatas) {
        tri_data->bind(shPhong.get());
        RenderAPI::getInstance()->drawTriangle(context, tri_data->triangle_nums);
    }
}

void Renderer::pop_input()
{
    m_input = InputData{};
}

void Renderer::setRenderData(std::shared_ptr<res::Model> model)
{
    m_meshesData = std::make_shared<RenderData>(model, m_sw, m_sh);
}

void Renderer::setRenderData(std::shared_ptr<res::BVH> bvh)
{
    m_meshesData = std::make_shared<RenderData>(bvh, m_sw, m_sh);
}

void Renderer::clearRenderData()
{
    m_meshesData.reset();
    m_meshesData = nullptr;
}
