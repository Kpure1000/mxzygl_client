#ifndef MODELCONVERTER_H
#define MODELCONVERTER_H

#include <QObject>
#include <QVector3D>
#include <QVector2D>

#include "resource/model.h"

class ModelConverter : public QObject
{
    Q_OBJECT
public:

    struct OBJFace {
        union {
            struct { unsigned int v, vt, vn; };
            unsigned int id[3];
        };
    };
    struct OBJModel {
    };

    explicit ModelConverter(QObject *parent = nullptr);

    static bool toOBJ(const QString &fileIn, const QString &fileOut);

signals:

};

#endif // MODELCONVERTER_H
