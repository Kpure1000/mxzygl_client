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

#include "arcball.h"

class Renderer : public QObject, public QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Renderer(int sw, int sh, QObject *parent = nullptr);

    ~Renderer();

    void resize(QOpenGLContext *context, int w, int h);

    void push_input(const InputData &input);
    void logic_tick(float dt);
    void render_tick(QOpenGLContext *context);
    void pop_input();


    void setRenderData(std::shared_ptr<res::Model> model);
    void setRenderData(std::shared_ptr<res::BVH> bvh);
    void clearRenderData();

private:
    std::shared_ptr<RenderData> m_meshesData;
    std::shared_ptr<SkyData> m_skyData;

    PhongShader *m_phongShader;
    SkyShader   *m_skyShader;

    std::shared_ptr<ArcBall> m_arcBall;

    float m_rot=.0f;

    int m_sw, m_sh;

    InputData m_input;
};

#endif // MX_RENDERER_H
