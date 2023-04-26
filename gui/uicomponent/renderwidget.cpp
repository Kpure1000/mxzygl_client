#include "renderwidget.h"

#include "mxzygl.h"
#include <QOpenGLContext>
#include <QDebug>
#include <QTimerEvent>

#include "resource/model.h"
#include "resource/bvh.h"

RenderWidget::RenderWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    m_timer.start(16, this);
    start_time = std::chrono::steady_clock::now();
}

void RenderWidget::initializeGL()
{
}

void RenderWidget::resizeGL(int w, int h)
{

}

void RenderWidget::paintGL()
{
    makeCurrent();
    auto currentContext = QOpenGLContext::currentContext();
    auto deltaTime = m_dt;
    // TODO: render api
//    f.glClearColor(.18f, .18f, .18f, 1.f);
//    f.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderWidget::timerEvent(QTimerEvent *event)
{
    *const_cast<float *>(&m_dt) = std::chrono::duration<float>(std::chrono::steady_clock::now() - start_time).count() * 1000.f;
    start_time = std::chrono::steady_clock::now();
    update();
}
std::shared_ptr<res::Model> g_model;
void RenderWidget::doModelRendering(const QString &assetName)
{
    g_model = ModelManager::getInstance()->get(assetName.toStdString());
//    qDebug() << "RenderWidget::doModelRendering>> Render Model" << model->name;
    // TODO render model
}

void RenderWidget::doBVHRendering(const QString &assetName)
{
    // TODO render BVH asset
}

void RenderWidget::stopRendering()
{
    // TODO clear rendering context
}
