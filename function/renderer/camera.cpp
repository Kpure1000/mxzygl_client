#include "camera.h"

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
