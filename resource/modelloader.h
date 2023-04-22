#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <QObject>

#include "model.h"

class ModelLoader : public QObject
{
    Q_OBJECT
public:
    explicit ModelLoader(QObject *parent = nullptr);

    std::shared_ptr<res::Model> loadFBX(const QString& filePath) const;
    std::shared_ptr<res::Model> loadOBJ(const QString& filePath) const;
    std::shared_ptr<res::Model> loadOFF(const QString& filePath) const;

public:

    static ModelLoader* getInstance(QObject *parent = nullptr)
    {
        static ModelLoader ml(parent);
        return &ml;
    }

signals:

};

#endif // MODELLOADER_H
