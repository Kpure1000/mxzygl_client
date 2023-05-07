#include "renderwidget.h"

#include <QOpenGLContext>
#include <QDebug>
#include <QTimerEvent>

#include "resource/model.h"
#include "resource/bvh.h"

#include "function/renderer/renderer.h"

RenderWidget::RenderWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

void RenderWidget::initializeGL()
{
//    qDebug() << "initializeGL";
    if (!m_renderer) {
        makeCurrent();
        m_renderer = new Renderer(this);
        m_timer.start(16, this);
        start_time = std::chrono::steady_clock::now();
        doneCurrent();
    }
}

void RenderWidget::resizeGL(int w, int h)
{

}

void RenderWidget::paintGL()
{
    auto currentContext = QOpenGLContext::currentContext();
    auto deltaTime = m_dt;
    if (m_renderer)
        m_renderer->render(currentContext, deltaTime);
}

void RenderWidget::timerEvent(QTimerEvent *event)
{
    *const_cast<float *>(&m_dt) = std::chrono::duration<float>(std::chrono::steady_clock::now() - start_time).count() * 1000.f;
    start_time = std::chrono::steady_clock::now();
    update();
}

void RenderWidget::doModelRendering(const QString &filePath)
{
    auto model = ModelManager::getInstance()->get(filePath.toStdString());
//    qDebug() << "RenderWidget::doModelRendering>> Render Model" << filePath;
    if (m_renderer) {
        m_renderer->setRenderData(std::make_shared<RenderData>(model));
    }
}

void RenderWidget::doBVHRendering(const QString &filePath)
{
    // TODO render BVH asset
}

void RenderWidget::stopRendering()
{
    if (m_renderer) {
        m_renderer->clearRenderData();
    }
}
