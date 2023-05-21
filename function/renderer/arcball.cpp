#include "arcball.h"

#include <QDebug>

constexpr float rad2angle = 180.f / 3.1415926f;

ArcBall::ArcBall(Transform *trans_model, int w, int h, float moveSpeed, float rotateSpeed, float drag)
    : m_trans_model(trans_model),
      m_isLeftTracking(false)
    , m_isMidTracking(false)
    , m_velocity(.0f)
    , m_axis(.0f, 1.f, .0f)
    , m_lastWorldPos_Left(.0f, .0f, .0f)
    , m_w(w), m_h(h)
    , m_moveSpeed(moveSpeed), m_rotateSpeed(rotateSpeed), m_drag(drag)
{
}

void ArcBall::resize(int w, int h)
{
    m_w = w;
    m_h = h;
    m_isLeftTracking = false;
    m_isMidTracking = false;
}

void ArcBall::tick(const InputData &input, const QVector3D &front, float dt)
{
    auto rot = QQuaternion::fromDirection(front, {.0f, 1.f, .0f});

    auto curWorldPos = screenToWorld(input.pos);

    if (input.mouse.move && (input.mouseButtons & InputData::MouseButton::LEFT)) {
        if (!m_isLeftTracking) {
            m_isLeftTracking = true;
            m_lastWorldPos_Left = curWorldPos;
        }

        QVector3D moveDir = (curWorldPos - m_lastWorldPos_Left);

        float theta_Angle = moveDir.length() * rad2angle;

        m_axis = QVector3D::crossProduct(rot.rotatedVector(moveDir), front).normalized();

        m_velocity = theta_Angle * m_rotateSpeed;

        m_lastWorldPos_Left = curWorldPos;
    }
    if (input.mouse.release && input.mouseButton == InputData::MouseButton::LEFT) {
        m_isLeftTracking = false;
    }

    if (input.mouse.move && (input.mouseButtons & InputData::MouseButton::MIDDLE)) {
        if (!m_isMidTracking) {
            m_isMidTracking = true;
            m_lastWorldPos_Mid = curWorldPos;
        }

        QVector3D moveDir = (curWorldPos - m_lastWorldPos_Mid);

        m_trans_model->position += rot.rotatedVector(moveDir) * m_moveSpeed * dt;

        m_lastWorldPos_Mid = curWorldPos;
    }
    if (input.mouse.release && input.mouseButton == InputData::MouseButton::MIDDLE) {
        m_isMidTracking = false;
    }

    m_velocity = std::max(0.0f, m_velocity - m_velocity * m_drag * dt);
    m_trans_model->rotation = QQuaternion::fromAxisAndAngle(m_axis, m_velocity) * m_trans_model->rotation;

}

void ArcBall::reset()
{
    m_velocity = 0.0f;
    m_axis = {.0f, 1.f, .0f};
}

QVector3D ArcBall::screenToWorld(const QPoint &p)
{
    return QVector3D{
        - 2.f * static_cast<float>(p.x()) / static_cast<float>(m_w) + 1.f,
        1.f - 2.f * static_cast<float>(p.y()) / static_cast<float>(m_h),
        .0f
    };
}
