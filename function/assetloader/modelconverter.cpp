#include "modelconverter.h"

#include <QDebug>
#include <QMatrix4x4>
#include <QFileInfo>

#include "assimp/Importer.hpp"
#include "assimp/Exporter.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

ModelConverter::ModelConverter(QObject *parent) : QObject(parent)
{

}

bool ModelConverter::toOBJ(const QString &fileIn, const QString &fileOut)
{
    Assimp::Importer importer;
    importer.SetPropertyBool(AI_CONFIG_PP_PTV_NORMALIZE, true);
    auto scene = importer.ReadFile(fileIn.toStdString(), aiProcess_PreTransformVertices | aiProcess_GenNormals);
    if (nullptr == scene) {
        qDebug() << "ModelConverter::toOBJ>> Assimp Scene" << fileIn << "Load Failed";
        return false;
    }
    if (!scene->HasMeshes()) {
        qDebug() << "ModelConverter::toOBJ>> Assimp Scene" << fileIn << "DONNT Have Any Meshes";
        return false;
    }

    Assimp::Exporter exporter;
    auto exret = exporter.Export(scene, "obj", fileOut.toStdString());
    if (exret == aiReturn::aiReturn_OUTOFMEMORY) {
        qDebug() << "ModelConverter::toOBJ>> Assimp Scene " << fileOut << "Export Result: aiReturn::aiReturn_OUTOFMEMORY";
        return false;
    } else if (exret == aiReturn::aiReturn_FAILURE) {
        qDebug() << "ModelConverter::toOBJ>> Assimp Scene " << fileOut << "Export Result: aiReturn::aiReturn_FAILURE";
        return false;
    }
    return true;
}
