#include "renderer.h"

#include <QDebug>

#include "renderapi.h"

Renderer::Renderer(QObject *parent)
    : QObject(parent)
{

}

Renderer::~Renderer()
{

}

void Renderer::render(QOpenGLContext *context, float dt)
{
    RenderAPI::getInstance()->clear(context);
}

void Renderer::stopRendering()
{

}
