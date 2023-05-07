#include "shadermanager.h"
#include <QOpenGLShader>

ShaderManager::ShaderManager(QObject *parent)
    : QObject(parent)
{
}

QOpenGLShaderProgram *ShaderManager::load(const std::string &shaderName, const QString &vShaderFileName,
                                          const QString &fShaderFileName)
{
    auto vShader = new QOpenGLShader(QOpenGLShader::Vertex);
    if (!vShader->compileSourceFile(vShaderFileName)) {
        qDebug() << "ShaderManager::load>> Vertex Shader" << vShaderFileName << "Load Failed,"
                 << vShader->log();
        return nullptr;
    }
    auto fShader = new QOpenGLShader(QOpenGLShader::Fragment);
    if (!fShader->compileSourceFile(fShaderFileName)) {
        qDebug() << "ShaderManager::load>> Fragment Shader" << fShaderFileName << "Load Failed,"
                 << fShader->log();
        return nullptr;
    }
    auto sprog = new QOpenGLShaderProgram();
    sprog->addShader(vShader);
    sprog->addShader(fShader);
    if (!sprog->link()) {
        qDebug() << "ShaderManager::load>> QOpenGLShaderProgram Link Failed," << sprog->log();
        return nullptr;
    }
    emit onShaderLoaded(shaderName, sprog);
    return sprog;
}
