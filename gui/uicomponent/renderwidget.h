#ifndef MX_RENDERWIDGET_H
#define MX_RENDERWIDGET_H

#include <QOpenGLWidget>
#include <QBasicTimer>

#include <chrono>

class Renderer;

class RenderWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit RenderWidget(QWidget *parent = nullptr);

private:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void timerEvent(QTimerEvent *event) override;

signals:


public slots:
    void doModelRendering(const QString &filePath);
    void doBVHRendering(const QString &filePath);
    void stopRendering();

private:
    std::chrono::steady_clock::time_point start_time;
    /**
     * @brief deltaTime as milliseconds
     */
    const float m_dt = 16.0f;
    QBasicTimer m_timer;

    Renderer *m_renderer;
};

#endif // MX_RENDERWIDGET_H
