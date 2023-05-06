#ifndef MX_RENDERER_H
#define MX_RENDERER_H

#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <memory>


class Renderer : public QObject, public QOpenGLFunctions
{
    Q_OBJECT
public:
    Renderer(QObject *parent = nullptr);

    ~Renderer();

    void render(QOpenGLContext *context, float dt);
    void stopRendering();

private:
};

#endif // MX_RENDERER_H
