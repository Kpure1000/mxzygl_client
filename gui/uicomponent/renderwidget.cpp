#include "renderwidget.h"

#include <QOpenGLContext>
#include <QDebug>
#include <QTimerEvent>

#include "resource/model.h"
#include "resource/bvh.h"

#include "function/renderer/renderer.h"

RenderWidget::RenderWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    this->setFocusPolicy(Qt::WheelFocus);
}

RenderWidget::~RenderWidget()
{
//    if (!m_renderer) {
////        qDebug() << "RenderWidget::~RenderWidget";
//        delete m_renderer;
//        m_renderer = nullptr;
//    }
}

void RenderWidget::initializeGL()
{
//    qDebug() << "initializeGL";
    if (!m_renderer) {
        makeCurrent();
        m_renderer = new Renderer(size().width(), size().height(), this);
        doneCurrent();
        m_timer.start(16, this);
        start_time = std::chrono::steady_clock::now();
    }
}

void RenderWidget::resizeGL(int w, int h)
{
    if (m_renderer) {
        m_renderer->resize(QOpenGLContext::currentContext(), w, h);
    }
}

void RenderWidget::paintGL()
{
    auto currentContext = QOpenGLContext::currentContext();
    auto deltaTime = m_dt;
    if (m_renderer) {
        m_renderer->logic_tick(deltaTime);
        m_renderer->render_tick(currentContext);
        m_renderer->pop_input();
    }
}

void RenderWidget::timerEvent(QTimerEvent *event)
{
    *const_cast<float *>(&m_dt) = std::chrono::duration<float>(std::chrono::steady_clock::now() - start_time).count() * 1000.f;
    start_time = std::chrono::steady_clock::now();
    update();
}

void RenderWidget::keyPressEvent(QKeyEvent *event)
{
    if (m_renderer) {
        InputData input{InputData::InputType::KEY};
        switch (event->key()) {
        case Qt::Key_W: // front
        {
            input.front = true;
            m_renderer->push_input(input);
            break;
        }
        case Qt::Key_A: // left
        {
            input.left = true;
            m_renderer->push_input(input);
            break;
        }
        case Qt::Key_S: // back
        {
            input.back = true;
            m_renderer->push_input(input);
            break;
        }
        case Qt::Key_D: // right
        {
            input.right = true;
            m_renderer->push_input(input);
            break;
        }
        case Qt::Key_R: // reset
        {
            input.reset = true;
            m_renderer->push_input(input);
            break;
        }
        case Qt::Key_Escape: // reset
        {
            clearFocus();
            break;
        }
        }
    }
}

void RenderWidget::wheelEvent(QWheelEvent *event)
{
    if (m_renderer) {
        InputData input{InputData::InputType::WHEEL};
        input.scrollx = event->angleDelta().x();
        input.scrolly = event->angleDelta().y();
        m_renderer->push_input(input);
    }
}

void RenderWidget::mousePressEvent(QMouseEvent *event)
{
    if (m_renderer && event->button() == Qt::MouseButton::LeftButton) {
        InputData input{InputData::InputType::MOUSE};
        input.press = true;
        input.pos = event->pos();
        m_renderer->push_input(input);
    }
    event->ignore();
}

void RenderWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_renderer && event->button() == Qt::MouseButton::LeftButton) {
        InputData input{InputData::InputType::MOUSE};
        input.release = true;
        input.pos = event->pos();
        m_renderer->push_input(input);
    }
    event->ignore();
}

void RenderWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_renderer) {
        InputData input{InputData::InputType::MOUSE};
        input.move = true;
        input.pos = event->pos();
        m_renderer->push_input(input);
    }
}

void RenderWidget::leaveEvent(QEvent *event)
{
    this->clearFocus();
}

void RenderWidget::doModelRendering(const QString &filePath)
{
    auto model = ModelManager::getInstance()->get(filePath.toStdString());
//    qDebug() << "RenderWidget::doModelRendering>> Render Model" << filePath;
    if (m_renderer) {
        m_renderer->setRenderData(model);
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
