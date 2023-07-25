#ifndef MX_RENDERER_H
#define MX_RENDERER_H

#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QVector3D>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <memory>
#include <functional>

#include "renderdata.h"

struct FPSCameraController;
struct ArcBall;

struct RenderConfig {
    QVector3D skyColor, groundColor;
    struct { float moveSpeed, rotateSpeed, drag; } arcball;
    struct { float moveSpeed, rotateSpeed; } fpscamera;
};

class Renderer : public QObject, public QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Renderer(int sw, int sh, QObject *parent = nullptr);

    ~Renderer();

    void initialize();
    bool is_initialized() const { return m_is_initialized; }

    void resize(QOpenGLContext *context, int w, int h);

    void push_input(const InputData &input);
    void logic_tick(float dt);
    void render_tick(QOpenGLContext *context);
    void pop_input();


    void setRenderData(std::shared_ptr<res::Model> model);
    void setRenderData(std::shared_ptr<res::BVH> bvh);
    void clearRenderData();
    bool hasRendererData() const { return nullptr != m_meshesData; }

    void resetAllTransform();
    Transform getModelTransform() const;
    Transform getCameraTransform() const;
    void setModelTransform(const Transform &trans);
    void setCameraTransform(const Transform &trans);

    float getBVHSampleFreq() const;
    void setBVHSampleFreq(float spp);
    int getBVHFrames() const;
    void setBVHFrames(int frames);

signals:
    void onRenderDataSet();
    void onRenderDataCleared();

private:
    std::shared_ptr<RenderData> m_meshesData;
    std::shared_ptr<SkyData> m_skyData;

    StaticModelLightShader *m_staticShader;
    AnimationModelLightShader *m_animationShader;
    SkyShader   *m_skyShader;

    Transform trans_camera;
    Transform trans_model;

    std::shared_ptr<ArcBall> m_arcBall;
    std::shared_ptr<FPSCameraController> m_fpsCamera;

    RenderConfig m_renderConfig;

    float m_rot=.0f;

    int m_sw, m_sh;

    InputData m_input;

    bool m_is_initialized;

    float m_ouput_time = .0f;
};

#endif // MX_RENDERER_H
