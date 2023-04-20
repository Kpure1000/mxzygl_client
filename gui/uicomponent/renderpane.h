#ifndef MX_RENDERPANE_H
#define MX_RENDERPANE_H

#include <QOpenGLWidget>

class RenderPane : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit RenderPane(QWidget *parent = nullptr);

private:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

signals:

};

#endif // MX_RENDERPANE_H
