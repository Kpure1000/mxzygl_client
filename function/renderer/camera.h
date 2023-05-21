#ifndef MX_CAMERA_H
#define MX_CAMERA_H

#include <QMatrix4x4>
#include <QVector3D>
#include "transform.h"

#undef far
#undef near

struct Camera {
    explicit Camera(float aspect, float near, float far);
    QMatrix4x4 get_view_mat(const Transform &trans) const;
    virtual QMatrix4x4 get_projection_mat() const = 0;

    float aspect, near, far;
    const QVector3D m_defaultFront = {.0f, .0f, -1.f}, m_defaultUp = {.0f, 1.f, .0f};
};

struct PerspectiveCamera : public Camera
{
    explicit PerspectiveCamera(float aspect, float near, float far, float fov);
    QMatrix4x4 get_projection_mat() const override;

    float fov;
};

struct InputData;

struct FPSCameraController {
    FPSCameraController(Transform *trans_ca, float moveSpeed = 1.0f, float rotateSpeed = 1.0f);

    void reset();

    void tick(const InputData &input, float dt);

    QVector3D getFront() const;

    void setMoveSpeed(float moveSpeed) { m_moveSpeed = moveSpeed; }
    void setRotateSpeed(float rotateSpeed) { m_rotateSpeed = rotateSpeed; }

private:
    Transform *m_trans_ca;
    QVector3D m_front;
    float m_yaw, m_pitch, m_row;
    float m_moveSpeed, m_rotateSpeed;

    QPoint m_last_cursor_pos;

    bool m_is_track = false;
};

#endif // MX_CAMERA_H
