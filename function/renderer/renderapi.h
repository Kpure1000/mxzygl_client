#ifndef MX_RENDERAPI_H
#define MX_RENDERAPI_H

#include <QObject>
#include <QVector3D>
#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>

class RenderAPI : public QObject
{
    Q_OBJECT
public:
    explicit RenderAPI(QObject *parent = nullptr);

    void init();

    void resize(QOpenGLContext *context, int w, int h);

    void clearAll(QOpenGLContext *context);
    void clearColor(QOpenGLContext *context);
    void clearDepth(QOpenGLContext *context);
    void enableDepth(QOpenGLContext *context, bool on = true);
    void enableBlend(QOpenGLContext *context, bool on = true);

    void drawTriangle(QOpenGLContext *context, int triangleNums);

public:
    static RenderAPI* getInstance(QObject *parent = nullptr)
    {
        static RenderAPI instance(parent);
        return &instance;
    }

    QOpenGLFunctions_3_3_Core f_330C;

private:
    QVector3D m_clearColor;
};

#endif // MX_RENDERAPI_H
