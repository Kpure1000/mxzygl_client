#include "renderer.h"

#include <QDebug>

#include "renderapi.h"
#include "renderdata.h"
#include "resource/model.h"
#include "resource/bvh.h"
#include "arcball.h"
#include "camera.h"
#include "function/configer/configmanager.h"

const auto defaultCameraTrans = Transform{QVector3D{.0f, .0f, 1.0f},
                                         QQuaternion{1.f, QVector3D{.0f, .0f, .0f}},
                                         QVector3D{.0f, .0f, .0f}};

const auto defaultModelTrans = Transform({.0f, .0f, -1.2f}, QQuaternion{1.f, {.0f, .0f, .0f}}, {1.f, 1.f, 1.f});

Renderer::Renderer(int sw, int sh, QObject *parent)
    : QObject(parent)
    , m_sw(sw)
    , m_sh(sh)
    , m_is_initialized(false)
{
    trans_camera = defaultCameraTrans;
    trans_model = defaultModelTrans;

    m_renderConfig.skyColor = QVector3D{0.2f, 0.3f, 0.48f};
    m_renderConfig.groundColor = QVector3D{0.4f, 0.3f, 0.2f};
    m_renderConfig.arcball.moveSpeed = 0.005f;
    m_renderConfig.arcball.rotateSpeed = 1.0f;
    m_renderConfig.arcball.drag = 0.01f;
    m_renderConfig.fpscamera.moveSpeed = 0.002f;
    m_renderConfig.fpscamera.rotateSpeed = 0.002f;

    m_arcBall = std::make_shared<ArcBall>(&trans_model, m_sw, m_sh, m_renderConfig.arcball.moveSpeed, m_renderConfig.arcball.rotateSpeed, m_renderConfig.arcball.drag);

    m_fpsCamera = std::make_shared<FPSCameraController>(&trans_camera, m_renderConfig.fpscamera.moveSpeed, m_renderConfig.fpscamera.rotateSpeed);

    auto doRendererConfigModified = [this]() {
        auto configM = ConfigManager::getInstance();
        auto val = configM->getConfig("Render/SkyColor", m_renderConfig.skyColor);
        if (!val.canConvert<QVector3D>()) {
            qDebug() << "Renderer::Renderer>> SkyColor" << val << "is not Vec3";
            val = m_renderConfig.skyColor;
        }
        m_renderConfig.skyColor = val.value<QVector3D>();
        val = configM->getConfig("Render/GroundColor", m_renderConfig.groundColor);
        if (!val.canConvert<QVector3D>()) {
            qDebug() << "Renderer::Renderer>> GroundColor" << val << "is not Vec3";
            val = m_renderConfig.groundColor;
        }
        m_renderConfig.groundColor = val.value<QVector3D>();

        m_renderConfig.arcball.moveSpeed = configM->getConfig("Render/ArcBall_moveSpeed", m_renderConfig.arcball.moveSpeed).toFloat();
        m_renderConfig.arcball.rotateSpeed = configM->getConfig("Render/ArcBall_rotateSpeed", m_renderConfig.arcball.rotateSpeed).toFloat();
        m_renderConfig.arcball.drag = configM->getConfig("Render/ArcBall_drag", m_renderConfig.arcball.drag).toFloat();
        m_renderConfig.fpscamera.moveSpeed = configM->getConfig("Render/FPSCamera_moveSpeed", m_renderConfig.fpscamera.moveSpeed).toFloat();
        m_renderConfig.fpscamera.rotateSpeed = configM->getConfig("Render/FPSCamera_rotateSpeed", m_renderConfig.fpscamera.rotateSpeed).toFloat();

        m_arcBall->setSclar(m_renderConfig.arcball.rotateSpeed);
        m_arcBall->setDrag(m_renderConfig.arcball.drag);

        m_fpsCamera->setMoveSpeed(m_renderConfig.fpscamera.moveSpeed);
        m_fpsCamera->setRotateSpeed(m_renderConfig.fpscamera.rotateSpeed);

        if (this->is_initialized()) {
            m_skyData->sky = m_renderConfig.skyColor;
            m_skyData->ground = m_renderConfig.groundColor;
        }
    };
    connect(ConfigManager::getInstance(), &ConfigManager::onConfModified, this, doRendererConfigModified);
    doRendererConfigModified();
}

Renderer::~Renderer()
{
    //    qDebug() << "Renderer::~Renderer";
}

void Renderer::initialize()
{
    m_is_initialized = true;
    m_phongShader   = new PhongShader("Phong", this);
    m_skyShader     = new SkyShader("Skybox", this);
    m_skyData = std::make_shared<SkyData>(m_renderConfig.skyColor, m_renderConfig.groundColor);
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
        m_input.keyCode = input.keyCode;
        for (int i = 0; i < m_input.nKeyEvents; i++) {
            m_input.key.key_event[i] |= input.key.key_event[i];
        }
        break;
    }
    case InputData::InputType::MOUSE: {
        m_input.pos = input.pos;
        m_input.mouseButton = input.mouseButton;
        m_input.mouseButtons |= input.mouseButtons;
        for (int i = 0; i < m_input.nMouseEvents; i++) {
            m_input.mouse.mouse_event[i] |= input.mouse.mouse_event[i];
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
        auto zoom = m_input.scrolly;
        auto scale = &trans_model.scale;
        scale->setX(std::max(.5f, std::min(6.f, scale->x() + zoom * dt * 0.0003f)));
        scale->setY(std::max(.5f, std::min(6.f, scale->y() + zoom * dt * 0.0003f)));
        scale->setZ(std::max(.5f, std::min(6.f, scale->z() + zoom * dt * 0.0003f)));
        // FPSCameraController 处理
        m_fpsCamera->tick(m_input, dt);
        // ArcBall处理
        m_arcBall->tick(m_input, m_fpsCamera->getFront(), dt);
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
    shSky->setUniformValue("_fov_rad", m_meshesData->camera->fov * 3.1415926f / 180.f);
    shSky->setUniformValue("_sky", m_skyData->sky);
    shSky->setUniformValue("_ground", m_skyData->ground);
    shSky->setUniformValue("_pitch", trans_camera.rotation.toEulerAngles().x() * 3.1415926f / 180.f);
    m_skyData->triangleData->bind(shSky.get());
    RenderAPI::getInstance()->drawTriangle(context, m_skyData->triangleData->triangle_nums);

    RenderAPI::getInstance()->clearDepth(context);
    auto shPhong = m_phongShader->sprog;
    shPhong->bind();
    // 模型变换
    auto modelMat = trans_model.get_trans_mat();
    shPhong->setUniformValue("_model", modelMat);
    shPhong->setUniformValue("_normal", modelMat.normalMatrix());
    // 相机变换
    auto camera = m_meshesData->camera;
    shPhong->setUniformValue("_view", camera->get_view_mat(trans_camera));
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
    m_meshesData.reset();
    m_meshesData = std::make_shared<RenderData>(model, m_sw, m_sh);
    emit onRenderDataSet();
}

void Renderer::setRenderData(std::shared_ptr<res::BVH> bvh)
{
    m_meshesData.reset();
    m_meshesData = std::make_shared<RenderData>(bvh, m_sw, m_sh);
    emit onRenderDataSet();
}

void Renderer::clearRenderData()
{
    m_meshesData.reset();
    m_meshesData = nullptr;
    emit onRenderDataCleared();
}

void Renderer::resetAllTransform()
{
    trans_camera = defaultCameraTrans;
    trans_model = defaultModelTrans;
    m_arcBall->reset();
}

Transform Renderer::getModelTransform() const
{
    return trans_model;
}

Transform Renderer::getCameraTransform() const
{
    return trans_camera;
}

void Renderer::setModelTransform(const Transform &trans)
{
    trans_model = trans;
    m_arcBall->reset();
}

void Renderer::setCameraTransform(const Transform &trans)
{
    trans_camera = trans;
    m_fpsCamera->reset();
}
