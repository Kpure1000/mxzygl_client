#ifndef MX_RENDERWIDGET_H
#define MX_RENDERWIDGET_H

#include <QOpenGLWidget>

class RenderWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit RenderWidget(QWidget *parent = nullptr);

private:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

signals:

};

#endif // MX_RENDERWIDGET_H
