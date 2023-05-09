#include "renderapi.h"

#include <QDebug>
#include <QOpenGLFunctions>

#include "function/configer/configmanager.h"

RenderAPI::RenderAPI(QObject *parent) : QObject(parent)
{
    auto doRendererConfigModified = [this]() {
        auto val = ConfigManager::getInstance()->getConfig("Render/ClearColor");
        if (!val.canConvert<QVector3D>()) {
            qDebug() << "RenderAPI>>ConfigManager::onConfModified>> ClearColor" << val << "is not Vec3";
            val = QVector3D{.0f, .0f, .0f};
        }
        m_clearColor = val.value<QVector3D>();
    };
    connect(ConfigManager::getInstance(), &ConfigManager::onConfModified, this, doRendererConfigModified);
    doRendererConfigModified();
}

void RenderAPI::resize(QOpenGLContext *context, int w, int h)
{
    QOpenGLFunctions f(context);
    f.glViewport(0, 0, w, h);
}

void RenderAPI::clearAll(QOpenGLContext *context)
{
    QOpenGLFunctions f(context);
    f.glClearColor(m_clearColor.x(), m_clearColor.y(), m_clearColor.z(), 1.0f);
    f.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderAPI::clearColor(QOpenGLContext *context)
{
    QOpenGLFunctions f(context);
    f.glClearColor(m_clearColor.x(), m_clearColor.y(), m_clearColor.z(), 1.0f);
    f.glClear(GL_COLOR_BUFFER_BIT);
}

void RenderAPI::clearDepth(QOpenGLContext *context)
{
    QOpenGLFunctions f(context);
    f.glClear(GL_DEPTH_BUFFER_BIT);
}

void RenderAPI::enableDepth(QOpenGLContext *context, bool on)
{
    QOpenGLFunctions f(context);
    on ? f.glEnable(GL_DEPTH_TEST) : f.glDisable(GL_DEPTH_TEST);
}

void RenderAPI::enableBlend(QOpenGLContext *context, bool on)
{
    QOpenGLFunctions f(context);
    if (on) {
        f.glEnable(GL_BLEND);
        f.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        f.glDisable(GL_BLEND);
    }
}

void RenderAPI::drawTriangle(QOpenGLContext *context, int triangleNums)
{
    QOpenGLFunctions f(context);
    f.glDrawElements(GL_TRIANGLES, triangleNums * 3, GL_UNSIGNED_INT, 0);
}
