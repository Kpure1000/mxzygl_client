#ifndef MX_TRANSFORM_H
#define MX_TRANSFORM_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QJsonArray>

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

    static QJsonArray toJson(const Transform &trans) {
        return QJsonArray{{
            trans.position.x(),      trans.position.y(), trans.position.z(),
            trans.rotation.scalar(), trans.rotation.x(), trans.rotation.y(), trans.rotation.z(),
            trans.scale.x(),         trans.scale.y(),    trans.scale.z()
        }};
    }

    static Transform fromJson(const QJsonArray &json) {
        if (json.size() != 10) {
            qDebug() << "Transform::fromJson>> JsonArray size != 10";
            return Transform();
        }
        return Transform{
            QVector3D   { json[0].toVariant().toFloat(), json[1].toVariant().toFloat(), json[2].toVariant().toFloat()  },
            QQuaternion { json[3].toVariant().toFloat(), {json[4].toVariant().toFloat(), json[5].toVariant().toFloat(), json[6].toVariant().toFloat()} },
            QVector3D   { json[7].toVariant().toFloat(), json[8].toVariant().toFloat(), json[9].toVariant().toFloat()  },
        };
    }
};

#endif // MX_TRANSFORM_H
