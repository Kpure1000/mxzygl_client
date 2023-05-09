#ifndef MX_TRANSFORM_H
#define MX_TRANSFORM_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>

struct Transform {
    QVector3D position;
    QQuaternion rotation;
    QVector3D scale;

    Transform() : position({.0f,.0f,.0f}), rotation(1.f, .0f,.0f,.0f), scale({1.f,1.f,1.f}) {}

    explicit Transform(const QVector3D &position, const QQuaternion &rotation, const QVector3D &scale)
        : position(position), rotation(rotation), scale(scale) {
    }

    QMatrix4x4 get_trans_mat() const {
        QMatrix4x4 trans;
        trans.translate(position);
        trans.rotate(rotation);
        trans.scale(scale);
        return trans;
    }

    QMatrix4x4 get_inv_trans() const { return get_trans_mat().inverted(); }
};

#endif // MX_TRANSFORM_H
