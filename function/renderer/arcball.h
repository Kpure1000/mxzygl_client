#ifndef ARCBALL_H
#define ARCBALL_H

#include <QPoint>
#include <QVector3D>
#include <QQuaternion>

#include "renderdata.h"

class ArcBall
{
public:
    ArcBall(Transform *trans_model, int w, int h, float moveSpeed, float rotateSpeed = 1.0f, float drag = 0.01f);

    void resize(int w, int h);
    void tick(const InputData &input, const QVector3D &front, float dt);
    void reset();

    void setSclar(float scalar) { m_rotateSpeed = scalar; }
    void setDrag(float drag) { m_drag = drag; }

private:
    QVector3D screenToWorld(const QPoint &p);

    Transform *m_trans_model;
    bool m_isLeftTracking, m_isMidTracking;
    float m_velocity;
    QVector3D m_axis;
    QVector3D m_lastWorldPos_Left, m_lastWorldPos_Mid;
    int m_w, m_h;
    float m_moveSpeed, m_rotateSpeed, m_drag;
};

#endif // ARCBALL_H
