#ifndef MX_CAMERA_H
#define MX_CAMERA_H

#include <QMatrix4x4>
#include <QVector3D>
#include "transform.h"

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

#endif // MX_CAMERA_H
