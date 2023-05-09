#include "arcball.h"

#include <QDebug>

constexpr float rad2angle = 180.f / 3.1415926f;

ArcBall::ArcBall(int w, int h, float scalar, float drag)
    : m_isTracking(false)
    , m_velocity(.0f)
    , m_axis(.0f, 1.f, .0f)
    , m_lastWorldPos(.0f, .0f, .0f)
    , m_rotation(QQuaternion::fromEulerAngles({.0f,.0f,.0f}))
    , m_w(w), m_h(h)
    , m_scalar(scalar), m_drag(drag)
{
}

void ArcBall::resize(int w, int h)
{
    m_w = w;
    m_h = h;
    m_isTracking = false;
}

void ArcBall::tick(const InputData &input, float dt)
{
    if (input.press) {
        startTrack(input.pos);
    }
    if (input.move) {
        track(input.pos);
    }
    if (input.release) {
        endTrack();
    }
    m_velocity = std::max(0.0f, m_velocity - m_velocity * m_drag * dt);
    m_rotation = QQuaternion::fromAxisAndAngle(m_axis, m_velocity) * m_rotation;
}

void ArcBall::startTrack(const QPoint &p)
{
    m_isTracking = true;
    m_lastWorldPos = screenToWorld(p);
}

void ArcBall::track(const QPoint &p)
{
    if (m_isTracking) {
        QVector3D curWorldPos = screenToWorld(p);

        QVector3D moveDir = (curWorldPos - m_lastWorldPos);
        float theta_Angle = moveDir.length() * rad2angle;

        QVector3D lastDir(.0f, .0f, -1.f);

        m_axis = QVector3D::crossProduct(moveDir, lastDir).normalized();

        m_velocity = theta_Angle * m_scalar;

        m_lastWorldPos = curWorldPos;
    }
}

void ArcBall::endTrack()
{
    m_isTracking = false;
}

QQuaternion ArcBall::getRotation() const
{
    return m_rotation;
}

QVector3D ArcBall::screenToWorld(const QPoint &p)
{
    return QVector3D{
        2.f * static_cast<float>(p.x()) / static_cast<float>(m_w) - 1.f,
        1.f - 2.f * static_cast<float>(p.y()) / static_cast<float>(m_h),
        .0f
    };
}
