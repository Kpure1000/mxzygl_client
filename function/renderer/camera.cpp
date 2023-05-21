#include "camera.h"

#include <cmath>
#include "renderdata.h"
#include <QDebug>

constexpr float radiance = 3.1415926f / 180.f;

#undef far
#undef near

Camera::Camera(float aspect, float near, float far)
    : aspect(aspect), near(near), far(far)
{

}

QMatrix4x4 Camera::get_view_mat(const Transform &trans) const
{
    QMatrix4x4 ret;
    ret.lookAt(trans.position,
               trans.position + trans.rotation.rotatedVector(m_defaultFront),
               trans.rotation.rotatedVector(m_defaultUp));
    return ret;
}

PerspectiveCamera::PerspectiveCamera(float aspect, float near, float far, float fov)
    : Camera(aspect, near, far), fov(fov)
{

}

QMatrix4x4 PerspectiveCamera::get_projection_mat() const
{
    QMatrix4x4 ret;
    ret.perspective(fov, aspect, near, far);
    return ret;
}

FPSCameraController::FPSCameraController(Transform *trans_ca, float moveSpeed, float rotateSpeed)
    : m_trans_ca(trans_ca), m_moveSpeed(moveSpeed), m_rotateSpeed(rotateSpeed)
{
    reset();
}

void FPSCameraController::reset()
{
    m_yaw = m_trans_ca->rotation.toEulerAngles().y();
    m_pitch = m_trans_ca->rotation.toEulerAngles().x();
    m_row = m_trans_ca->rotation.toEulerAngles().z();

    float rad_yaw = radiance * m_yaw, rad_pitch = radiance * m_pitch;

    m_front = QVector3D{-std::sin(rad_yaw) * std::cos(rad_pitch), std::sin(rad_pitch), -std::cos(rad_yaw) * std::cos(rad_pitch)};
}

void FPSCameraController::tick(const InputData &input, float dt)
{
    if (input.mouse.move && (input.mouseButtons & InputData::MouseButton::RIGHT)) {
        if (!m_is_track) {
            m_is_track = true;
            m_last_cursor_pos = input.pos;
        }
            m_yaw -= (input.pos.x() - m_last_cursor_pos.x()) * m_rotateSpeed * dt;
            m_pitch = std::max(-89.9f, std::min(89.9f, m_pitch - (input.pos.y() - m_last_cursor_pos.y()) * m_rotateSpeed * dt));

            m_last_cursor_pos = input.pos;

            m_trans_ca->rotation = QQuaternion::fromEulerAngles(m_pitch, m_yaw, m_row);

            float rad_yaw = radiance * m_yaw, rad_pitch = radiance * m_pitch;
            m_front = QVector3D{-std::sin(rad_yaw) * std::cos(rad_pitch), std::sin(rad_pitch), -std::cos(rad_yaw) * std::cos(rad_pitch)};
            m_front.normalize();
    }
    if (input.mouse.release && input.mouseButton == InputData::MouseButton::RIGHT) {
        m_is_track = false;
        m_last_cursor_pos = input.pos;
    }

//    qDebug() << "yaw:" << m_yaw << ", pitch:" << m_pitch << ", front:" << m_front;

    QVector3D right = QVector3D::crossProduct(m_front, {0, 1.0f, 0}).normalized();

    if (input.key.press && input.keyCode == InputData::W) {
        m_trans_ca->position += m_front * m_moveSpeed * dt;
    }
    if (input.key.press && input.keyCode == InputData::S) {
        m_trans_ca->position -= m_front * m_moveSpeed * dt;
    }
    if (input.key.press && input.keyCode == InputData::A) {
        m_trans_ca->position -= right * m_moveSpeed * dt;
    }
    if (input.key.press && input.keyCode == InputData::D) {
        m_trans_ca->position += right * m_moveSpeed * dt;
    }
}

QVector3D FPSCameraController::getFront() const
{
    float rad_yaw = radiance * m_yaw, rad_pitch = radiance * m_pitch;

    return {-std::sin(rad_yaw) * std::cos(rad_pitch), std::sin(rad_pitch), -std::cos(rad_yaw) * std::cos(rad_pitch)};
}

