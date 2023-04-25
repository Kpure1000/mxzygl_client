#ifndef MX_RENDERWIDGET_H
#define MX_RENDERWIDGET_H

#include <QOpenGLWidget>
#include <QBasicTimer>

#include <chrono>

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
    void doModelRendering(const QString &assetName);
    void doBVHRendering(const QString &assetName);
    void stopRendering();

private:
    std::chrono::steady_clock::time_point start_time;
    /**
     * @brief deltaTime as milliseconds
     */
    const float m_dt = 16.0f;
    QBasicTimer m_timer;

};

#endif // MX_RENDERWIDGET_H
