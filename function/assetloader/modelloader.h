#ifndef MX_MODELLOADER_H
#define MX_MODELLOADER_H

#include <QObject>

#include "resource/model.h"

class ModelLoader : public QObject
{
    Q_OBJECT
public:
    explicit ModelLoader(QObject *parent = nullptr);

    std::shared_ptr<res::Model> loadFBX(const QString& filePath, const QString& modelName, bool doGlobalTransform = true) const;
    std::shared_ptr<res::Model> loadOBJ(const QString& filePath, const QString& modelName) const;
    std::shared_ptr<res::Model> loadOFF(const QString& filePath, const QString& modelName) const;

public:

    static ModelLoader* getInstance(QObject *parent = nullptr)
    {
        static ModelLoader ml(parent);
        return &ml;
    }

signals:

};

#endif // MX_MODELLOADER_H
