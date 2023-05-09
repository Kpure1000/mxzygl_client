#ifndef ARCBALL_H
#define ARCBALL_H

#include <QPoint>
#include <QVector3D>
#include <QQuaternion>

#include "renderdata.h"

class ArcBall
{
public:
    ArcBall(int w, int h, float scalar = 1.0f, float drag = 0.01f);

    void resize(int w, int h);
    void tick(const InputData &input, float dt);
    QQuaternion getRotation() const;

private:
    void startTrack(const QPoint &p);
    void track(const QPoint &p);
    void endTrack();

    QVector3D screenToWorld(const QPoint &p);

    bool m_isTracking;
    float m_velocity;
    QVector3D m_axis;
    QVector3D m_lastWorldPos;
    QQuaternion m_rotation;
    int m_w, m_h;
    const float m_scalar, m_drag;
//    float m_dt;
};

#endif // ARCBALL_H
