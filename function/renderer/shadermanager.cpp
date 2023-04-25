#include "shadermanager.h"

ShaderManager::ShaderManager(QObject *parent)
    : QObject(parent)
{
}

QOpenGLShaderProgram *ShaderManager::load(const QString &vShaderFileName,
                                          const QString &fShaderFileName)
{
    auto sprog = new QOpenGLShaderProgram(this);
    if (!sprog->addShaderFromSourceCode(QOpenGLShader::Vertex, vShaderFileName)) {
        qDebug() << "ShaderManager::load>> Vertex Shader" << vShaderFileName << "Load Failed";
        return nullptr;
    }
    if (!sprog->addShaderFromSourceCode(QOpenGLShader::Fragment, fShaderFileName)) {
        qDebug() << "ShaderManager::load>> Fragment Shader" << fShaderFileName << "Load Failed";
        return nullptr;
    }
    emit onShaderLoaded(sprog);
    return sprog;
}
