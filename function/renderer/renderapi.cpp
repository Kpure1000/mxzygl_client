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
            return;
        }
        m_clearColor = val.value<QVector3D>();
    };
    connect(ConfigManager::getInstance(), &ConfigManager::onConfModified, this, doRendererConfigModified);
    doRendererConfigModified();
}

void RenderAPI::clear(QOpenGLContext *context)
{
    QOpenGLFunctions f(context);
    f.glClearColor(m_clearColor.x(), m_clearColor.y(), m_clearColor.z(), 1.0f);
    f.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
