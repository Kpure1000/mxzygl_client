#include "assetimporter.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>
#include <QFileInfo>

#include "function/network/client.h"
#include "function/network/protocal.h"
#include "function/metacategory.h"
#include "function/assetloader/modelloader.h"
#include "function/assetloader/modelconverter.h"
#include "utils/jobsystem.h"

AssetImporter::AssetImporter(ImportType type, QObject *parent)
    : QObject{parent}, m_type(type)
{
    m_client = new Client(this);

    connect(m_client, &Client::onSendingStateChanged, this, &AssetImporter::onResponsing);

    connect(m_client, &Client::onReadOver, this, [=](const QJsonObject &data, const QByteArray &extraData) {
        auto upload_cost = std::chrono::duration<float>(std::chrono::steady_clock::now() - m_upload_start).count();
        auto response_type = static_cast<Protocal::HeaderField>(data["type"].toInt());
        auto status = data["status"].toString();
        if (Protocal::HeaderField::RESPONSE_ERROR == response_type) {
            emit onResponsing(tr("服务请求错误. Info:") + status, false);
        } else {
            if (!status.isEmpty()) {
                if (Protocal::HeaderField::RESPONSE_UPLOADMODEL == response_type) {
                    emit onResponsing(tr("上传失败. Info: ") + status, false);
                } else if (Protocal::HeaderField::REQUEST_UPLOADMODEL_SIMPLE == response_type){
                    emit onResponsing(tr("简单上传失败. Info: ") + status, false);
                }
            } else {
                if (Protocal::HeaderField::RESPONSE_UPLOADMODEL == response_type) {
                    emit onResponsing(tr("上传成功, 耗时 ") + QString().asprintf("%f", upload_cost) + tr(" 秒"), false);
                    emit onUploadSuccessful();
                } else if (Protocal::HeaderField::RESPONSE_UPLOADMODEL_SIMPLE == response_type){
                    emit onResponsing(tr("简单上传成功, 耗时 ") + QString().asprintf("%f", upload_cost) + tr(" 秒"), false);
                    emit onUploadSimpleSuccessful();
                }
            }
        }
    });

    m_typeManager = new TypeManager(type, this);
    m_tagsManager = new TagsManager(type, this);

    connect(m_typeManager, &TypeManager::onResponsing, this, &AssetImporter::onResponsing);
    connect(m_tagsManager, &TagsManager::onResponsing, this, &AssetImporter::onResponsing);

    connect(m_typeManager, &TypeManager::onPullSuccessful, this, [=](){
        is_typeLoaded = true;
        if (is_tagsLoaded) {
            addTypeAndTagsData();
        }
    }, Qt::QueuedConnection);
    connect(m_tagsManager, &TagsManager::onPullSuccessful, this, [=](){
        is_tagsLoaded = true;
        if (is_typeLoaded) {
            addTypeAndTagsData();
        }
    }, Qt::QueuedConnection);

    m_info.insert("category", MetaCategory::getInstance()->getCategoryInt());
    // TODO: 连接MetaCategory修改信号
    connect(MetaCategory::getInstance(), &MetaCategory::onCategoryModyfied, this, [=](){
        this->m_info["category"] = MetaCategory::getInstance()->getCategoryInt();
    });

    QJsonArray headers;
    headers << res::AssetInfo::toHeaderElement("name",          false,  false,  true);
    headers << res::AssetInfo::toHeaderElement("type",          true,   true,   false);
    headers << res::AssetInfo::toHeaderElement("tags",          true,   true,   false);
    headers << res::AssetInfo::toHeaderElement("fileType",      false,  false,  true);
    headers << res::AssetInfo::toHeaderElement("path",          false,  false,  true);
    headers << res::AssetInfo::toHeaderElement("trans_model",   false,  false,  false);
    headers << res::AssetInfo::toHeaderElement("trans_camera",      false,  false,  false);
    QJsonArray data;
    m_info.insert("headers", headers);
    m_info.insert("data", data);
}

AssetImporter::~AssetImporter()
{
//    qDebug() << "AssetImporter::~AssetImporter";
}

void AssetImporter::addPathsNotExist(const QStringList &filePaths)
{
    int addition_count = 0;
    for (const auto &filePath : filePaths) {
        if (!has(filePath)) {
            m_filePathDict.insert(filePath.toStdString());
            m_filePaths << filePath;
            auto data = m_info["data"].toArray();

            auto fileInfo = QFileInfo(filePath);
            QJsonObject item;
            item.insert("name", fileInfo.baseName());

            QString fileType;
            if (m_type == ImportType::MODEL || m_type == ImportType::BVH) {
                fileType = fileInfo.filePath().split('.').back();
            } else {
                // TODO: Effect Asset fileType
            }
            item.insert("fileType", fileType);
            item.insert("path", filePath);

            data << item;
            m_info["data"] = data;
            addition_count++;
        }
    }
//    qDebug() << m_info;
    if (addition_count)
        emit onAddPaths();
}

void AssetImporter::clear()
{
    m_filePathDict.clear();
    m_filePaths.clear();
    m_info["data"] = QJsonArray();
    emit onClear();
}

void AssetImporter::alignToOrigin()
{
    if (m_type == ImportType::EFFECT) {
        emit onResponsing("AssetImporter::alignToOrigin>> 特效资源不可执行对齐原点操作", false);
        qDebug() << "AssetImporter::alignToOrigin>> 特效资源不可执行对齐原点操作";
        return;
    }

    emit onResponsing(QString().asprintf("开始执行对齐原点操作, 共%d个模型", m_filePaths.size()), false);
    emit onStartAlignToOrigin(m_filePaths.size());

    for (auto &filePath : m_filePaths) {
        auto info = QFileInfo(filePath);
        auto baseName = info.baseName();
        auto path = info.absolutePath();
        auto savedName = path + '/' + baseName + ".obj";

        m_filePathDict.erase(m_filePathDict.find(filePath.toStdString()));
        m_filePathDict.insert(savedName.toStdString());

        filePath = savedName;

        JobSystem::getInstance()->submit([=](){
            auto convertResult = ModelConverter::toOBJ(info.absoluteFilePath(), savedName);

            int doneCount = m_saveCount.load() + 1;
            this->m_saveCount++;

            if (!convertResult) {
                emit onResponsing(QString().asprintf("模型 '%s' 转换失败", info.absoluteFilePath().toStdString().c_str()), false);
            }

            emit onResponsing(QString().asprintf("完成 %d / %d 个模型 '%s'", doneCount, m_filePaths.size(), baseName.toStdString().c_str()), true);

            emit onDoneAlignToOrigin(doneCount);

            if (m_saveCount == m_filePaths.size()) {
                auto data = m_info["data"].toArray();

                for (int i = 0; i < data.size(); i++) {
                    auto row = data[i].toObject();
                    row["path"] = m_filePaths[i];
                    row["fileType"] = "obj";
                    data[i] = row;
                }

                m_info["data"] = data;

                emit saveSuccessful();
            }
        });

    }
}

void AssetImporter::initAllTransform(const Transform &trans_model, const Transform &trans_camera)
{
    auto data = m_info["data"].toArray();
    for (auto row : data) {
        auto obj = row.toObject();
        obj["trans_model"] = Transform::toJson(trans_model);
        obj["trans_camera"] = Transform::toJson(trans_camera);
        row = obj;
    }
    m_info["data"] = data;
}

void AssetImporter::saveTransform(int row, const Transform &trans_model, const Transform &trans_camera)
{
    auto data = m_info["data"].toArray();
    auto obj = data[row].toObject();
    obj["trans_model"] = Transform::toJson(trans_model);
    obj["trans_camera"] = Transform::toJson(trans_camera);
    data[row] = obj;
    m_info["data"] = data;
}

std::pair<Transform, Transform> AssetImporter::getTransform(int row) const
{
    auto data = m_info["data"].toArray();
    auto obj = data[row].toObject();
    return {
        Transform::fromJson(obj["trans_model" ].toArray()),
        Transform::fromJson(obj["trans_camera"].toArray())
    };
}

void AssetImporter::pullTypeAndTags()
{
    auto headers = m_info["headers"].toArray();
    for (auto header : headers) {
        auto obj = header.toObject();
        if (obj["name"].toString() == "type" || obj["name"].toString() == "tags") {
            obj["visible"] = true;
            header = obj;
        }
    }
    m_info["headers"] = headers;
    m_typeManager->pull();
    m_tagsManager->pull();
}

void AssetImporter::upload()
{
    m_info["type"] = static_cast<int>(Protocal::HeaderField::REQUEST_UPLOADMODEL);
    __upload();
}

void AssetImporter::upload_simple()
{
    m_info["type"] = static_cast<int>(Protocal::HeaderField::REQUEST_UPLOADMODEL_SIMPLE);
    __upload();
}

QStringList AssetImporter::getFilePaths(const std::vector<int> &index) const
{
    QStringList filePaths;
    for (const auto &id : index) {
        filePaths << m_filePaths[id];
    }
    return filePaths;
}

QStringList AssetImporter::getFileNames(const std::vector<int> &index) const
{
    QStringList fileNames;
    for (const auto &id : index) {
        auto fileInfo = QFileInfo(m_filePaths[id]);
        fileNames << fileInfo.baseName();
    }
    return fileNames;
}

void AssetImporter::addTypeAndTagsData()
{
    auto data = m_info["data"].toArray();
    for (auto row : data) {
        auto obj = row.toObject();
        obj["type"] = QJsonObject{{"array", m_typeManager->getTypesNameList()},  {"value", ""}};
        obj["tags"] = QJsonObject{{"array", m_tagsManager->getTagsNameList()},   {"value", ""}};
        row = obj;
    }
    m_info["data"] = data;
    emit onTypeAndTagsLoaded();
}

void AssetImporter::__upload()
{
    m_upload_start = std::chrono::steady_clock::now();
    switch (m_type) {
    case ImportType::MODEL:  emit onResponsing(tr("模型资源简单上传开始"), true); break;
    case ImportType::BVH:    emit onResponsing(tr("骨骼动画资源简单上传开始"), true); break;
    case ImportType::EFFECT: emit onResponsing(tr("特效资源简单上传开始"), true); break;
    }
    m_client->sendData(m_info);
}

