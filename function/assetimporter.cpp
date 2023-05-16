#include "assetimporter.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>
#include <QFileInfo>

#include "function/network/client.h"
#include "function/network/protocal.h"
#include "function/metacategory.h"

AssetImporter::AssetImporter(ImportType type, QObject *parent)
    : QObject{parent}, m_type(type)
{
    m_client = new Client(this);

    connect(m_client, &Client::onSendingStateChanged, this, &AssetImporter::onResponsing);

    connect(m_client, &Client::onReadOver, this, [=](const QJsonObject &data, const QByteArray &extraData) {
        auto response_type = static_cast<Protocal::HeaderField>(data["type"].toInt());
        auto status = data["status"].toString();
        if (Protocal::HeaderField::RESPONSE_ERROR == response_type) {
            emit onResponsing(tr("服务请求错误. Info:") + status, false);
        } else {
            if (!status.isEmpty()) {
                emit onResponsing(tr("上传失败. Info: ") + status, false);
            } else {
                emit onResponsing(tr("上传成功!"), false);
                emit onImportSuccessful();
                this->clear();
            }
        }
    });

    m_modelType = new ModelTypeManager(type, this);
    m_ModelTags = new ModelTagsManager(type, this);

    connect(m_modelType, &ModelTypeManager::onResponsing, this, &AssetImporter::onResponsing);
    connect(m_ModelTags, &ModelTagsManager::onResponsing, this, &AssetImporter::onResponsing);

    connect(m_modelType, &ModelTypeManager::onPullSuccessful, this, [=](){
        is_typeLoaded = true;
        if (is_tagsLoaded) {
            emit onTypeAndTagsLoaded();
        }
    });
    connect(m_ModelTags, &ModelTagsManager::onPullSuccessful, this, [=](){
        is_tagsLoaded = true;
        if (is_typeLoaded) {
            emit onTypeAndTagsLoaded();
        }
    });

    m_info.insert("type", static_cast<int>(Protocal::HeaderField::REQUEST_UPLOADMODEL));
    m_info.insert("category", MetaCategory::getInstance()->getCategoryInt());
    // TODO: 连接MetaCategory修改信号
    connect(MetaCategory::getInstance(), &MetaCategory::onCategoryModyfied, this, [=](){
        this->m_info["category"] = MetaCategory::getInstance()->getCategoryInt();
    });

    QJsonArray headers;
    headers << res::AssetInfo::toHeaderElement("name",      false,  false,  true);
    headers << res::AssetInfo::toHeaderElement("type",      true,   true,   true);
    headers << res::AssetInfo::toHeaderElement("tags",      true,   true,   true);
    headers << res::AssetInfo::toHeaderElement("fileType",  false,  false,  true);
    headers << res::AssetInfo::toHeaderElement("path",      false,  false,  true);
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
            item.insert("type", QJsonObject{{"array", m_modelType->getTypesNameList()},  {"value", ""}});
            item.insert("tags", QJsonObject{{"array", m_ModelTags->getTagsNameList()},   {"value", ""}});
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

void AssetImporter::pullTypeAndTags()
{
    m_modelType->pull();
    m_ModelTags->pull();
}

void AssetImporter::upload()
{
    switch (m_type) {
    case ImportType::MODEL:  emit onResponsing(tr("模型资源上传开始"), true); break;
    case ImportType::BVH:    emit onResponsing(tr("骨骼动画资源上传开始"), true); break;
    case ImportType::EFFECT: emit onResponsing(tr("特效资源上传开始"), true); break;
    }
    m_client->sendData(m_info);
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

