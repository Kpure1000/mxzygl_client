#ifndef MX_RENDERWIDGET_H
#define MX_RENDERWIDGET_H

#include <QOpenGLWidget>
#include <QBasicTimer>

#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimerEvent>

#include <chrono>
#include <memory>

class Renderer;
namespace res {
struct Model;
struct BVH;
}

class RenderWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit RenderWidget(QWidget *parent = nullptr);
    ~RenderWidget();

    Renderer *getRenderer() const { return m_renderer; }

private:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void timerEvent(QTimerEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void leaveEvent(QEvent *event) override;

signals:
    void onFrameUpdate();

public slots:
    void doModelRendering(const QString &filePath);
    void doBVHRendering(const QString &filePath);
    void doModelRendering(std::shared_ptr<res::Model> model);
    void doBVHRendering(std::shared_ptr<res::BVH> bvh);
    void stopRendering();

private:
    std::chrono::steady_clock::time_point start_time;
    /**
     * @brief deltaTime as milliseconds
     */
    const float m_dt = 16.0f;
    QBasicTimer m_timer;

    Renderer *m_renderer = nullptr;
};

#endif // MX_RENDERWIDGET_H
