#ifndef MX_RENDERAPI_H
#define MX_RENDERAPI_H

#include <QObject>
#include <QVector3D>
#include <QOpenGLContext>

class RenderAPI : public QObject
{
    Q_OBJECT
public:
    explicit RenderAPI(QObject *parent = nullptr);

    void clear(QOpenGLContext *context);

    void drawTriangle(QOpenGLContext *context, int triangleNums);

public:
    static RenderAPI* getInstance(QObject *parent = nullptr)
    {
        static RenderAPI instance(parent);
        return &instance;
    }

private:
    QVector3D m_clearColor;
};

#endif // MX_RENDERAPI_H
