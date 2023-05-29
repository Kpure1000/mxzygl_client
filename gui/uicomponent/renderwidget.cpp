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
    m_renderer = new Renderer(size().width(), size().height(), this);
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
    if (!m_renderer->is_initialized()) {
        //    qDebug() << "initializeGL";
        makeCurrent();
        m_renderer->initialize();
        doneCurrent();

        m_timer.start(16, this);
        start_time = std::chrono::steady_clock::now();
    }
}

void RenderWidget::resizeGL(int w, int h)
{
    if (m_renderer->is_initialized()) {
        m_renderer->resize(QOpenGLContext::currentContext(), w, h);
    }
}

void RenderWidget::paintGL()
{
    auto currentContext = QOpenGLContext::currentContext();
    auto deltaTime = m_dt;
    if (m_renderer->is_initialized()) {
        m_renderer->logic_tick(deltaTime);
        m_renderer->render_tick(currentContext);
        m_renderer->pop_input();
        if (m_renderer->hasRendererData())
            emit onFrameUpdate();
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
    if (m_renderer->is_initialized()) {
        InputData input{InputData::InputType::KEY};
        input.key.press = true;
        switch (event->key()) {
        case Qt::Key_W: // front
        {
            input.keyCode = InputData::W;
            m_renderer->push_input(input);
            break;
        }
        case Qt::Key_A: // left
        {
            input.keyCode = InputData::A;
            m_renderer->push_input(input);
            break;
        }
        case Qt::Key_S: // back
        {
            input.keyCode = InputData::S;
            m_renderer->push_input(input);
            break;
        }
        case Qt::Key_D: // right
        {
            input.keyCode = InputData::D;
            m_renderer->push_input(input);
            break;
        }
        case Qt::Key_R: // reset
        {
            input.keyCode = InputData::R;
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

void RenderWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (m_renderer->is_initialized()) {
        InputData input{InputData::InputType::KEY};
        input.key.release = true;
        switch (event->key()) {
        case Qt::Key_W: // front
        {
            input.keyCode = InputData::W;
            m_renderer->push_input(input);
            break;
        }
        case Qt::Key_A: // left
        {
            input.keyCode = InputData::A;
            m_renderer->push_input(input);
            break;
        }
        case Qt::Key_S: // back
        {
            input.keyCode = InputData::S;
            m_renderer->push_input(input);
            break;
        }
        case Qt::Key_D: // right
        {
            input.keyCode = InputData::D;
            m_renderer->push_input(input);
            break;
        }
        case Qt::Key_R: // reset
        {
            input.keyCode = InputData::R;
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
    if (m_renderer->is_initialized()) {
        InputData input{InputData::InputType::WHEEL};
        input.scrollx = event->angleDelta().x();
        input.scrolly = event->angleDelta().y();
        m_renderer->push_input(input);
    }
}

void RenderWidget::mousePressEvent(QMouseEvent *event)
{
    if (m_renderer->is_initialized()) {
        if (event->button() == Qt::MouseButton::LeftButton) {
            InputData input{InputData::InputType::MOUSE};
            input.mouse.press = true;
            input.mouseButton = InputData::MouseButton::LEFT;
            input.pos = event->pos();
            m_renderer->push_input(input);
        } else if (event->button() == Qt::MouseButton::RightButton) {
            InputData input{InputData::InputType::MOUSE};
            input.mouse.press = true;
            input.mouseButton = InputData::MouseButton::RIGHT;
            input.pos = event->pos();
            m_renderer->push_input(input);
        } else if (event->button() == Qt::MouseButton::MidButton) {
            InputData input{InputData::InputType::MOUSE};
            input.mouse.press = true;
            input.mouseButton = InputData::MouseButton::MIDDLE;
            input.pos = event->pos();
            m_renderer->push_input(input);
        }
    }
    event->ignore();
}

void RenderWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_renderer->is_initialized()) {
        if (event->button() == Qt::MouseButton::LeftButton) {
            InputData input{InputData::InputType::MOUSE};
            input.mouse.release = true;
            input.mouseButton = InputData::MouseButton::LEFT;
            input.pos = event->pos();
            m_renderer->push_input(input);
        } else if (event->button() == Qt::MouseButton::RightButton) {
            InputData input{InputData::InputType::MOUSE};
            input.mouse.release = true;
            input.mouseButton = InputData::MouseButton::RIGHT;
            input.pos = event->pos();
            m_renderer->push_input(input);
        } else if (event->button() == Qt::MouseButton::MidButton) {
            InputData input{InputData::InputType::MOUSE};
            input.mouse.release = true;
            input.mouseButton = InputData::MouseButton::MIDDLE;
            input.pos = event->pos();
            m_renderer->push_input(input);
        }
    }
    event->ignore();
}

void RenderWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_renderer->is_initialized()) {
        InputData input{InputData::InputType::MOUSE};
        if (event->buttons() & Qt::MouseButton::LeftButton) {
            input.mouse.move = true;
            input.mouseButtons = InputData::MouseButton::LEFT;
        }
        if (event->buttons() & Qt::MouseButton::RightButton) {
            input.mouse.move = true;
            input.mouseButtons |= InputData::MouseButton::RIGHT;
        }
        if (event->buttons() & Qt::MouseButton::MidButton) {
            input.mouse.move = true;
            input.mouseButtons |= InputData::MouseButton::MIDDLE;
        }
        input.pos = event->pos();
        if (input.mouse.move) {
            m_renderer->push_input(input);
        }
    }
}

void RenderWidget::leaveEvent(QEvent *event)
{
    this->clearFocus();
}

void RenderWidget::doModelRendering(const QString &filePath)
{
    if (ModelManager::getInstance()->has(filePath.toStdString())) {
//        qDebug() << "RenderWidget::doModelRendering>> Render Model" << filePath;
        auto model = ModelManager::getInstance()->get(filePath.toStdString());
        if (m_renderer->is_initialized()) {
            m_renderer->setRenderData(model);
        }
    } else {
        qDebug() << "RenderWidget::doModelRendering>> Render Model Not Found" << filePath;
    }
}

void RenderWidget::doBVHRendering(const QString &filePath)
{
    if (BVHManager::getInstance()->has(filePath.toStdString())) {
        auto bvh = BVHManager::getInstance()->get(filePath.toStdString());
        if (m_renderer->is_initialized()) {
            m_renderer->setRenderData(bvh);
        }
    } else {
        qDebug() << "RenderWidget::doModelRendering>> Render Model Not Found" << filePath;
    }
}

void RenderWidget::doModelRendering(std::shared_ptr<res::Model> model)
{
    if (m_renderer->is_initialized()) {
        m_renderer->setRenderData(model);
    }
}

void RenderWidget::doBVHRendering(std::shared_ptr<res::BVH> bvh)
{
    if (m_renderer->is_initialized()) {
        m_renderer->setRenderData(bvh);
    }
}

void RenderWidget::stopRendering()
{
    if (m_renderer->is_initialized()) {
        m_renderer->clearRenderData();
    }
}
