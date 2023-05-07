#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <QObject>
#include <QOpenGLShaderProgram>

#include "utils/assetmanager.h"

class ShaderManager : public QObject, public IAssetManager<std::string, QOpenGLShaderProgram*>
{
    Q_OBJECT
public:
    explicit ShaderManager(QObject *parent = nullptr);

    QOpenGLShaderProgram *load(const std::string &shaderName, const QString &vShaderFileName, const QString &fShaderFileName);

signals:
    void onShaderLoaded(const std::string &shaderName, QOpenGLShaderProgram *);

public:
    static ShaderManager* getInstance(QObject *parent = nullptr)
    {
        static ShaderManager sm(parent);
        return &sm;
    }

};

#endif // SHADERMANAGER_H
