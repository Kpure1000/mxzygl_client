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
                this->clear();
            }
        }
    });

    m_info.insert("type", static_cast<int>(Protocal::HeaderField::REQUEST_UPLOADMODEL));
    m_info.insert("category", MetaCategory::getInstance()->getCategoryInt());
    // TODO: 连接MetaCategory修改信号
    connect(MetaCategory::getInstance(), &MetaCategory::onCategoryModyfied, this, [=](){
        this->m_info["category"] = MetaCategory::getInstance()->getCategoryInt();
    });

    QJsonArray headers = res::AssetInfo::get_headers();
    QJsonArray data;
    m_info.insert("headers", headers);
    m_info.insert("data", data);
}

AssetImporter::~AssetImporter()
{
//    qDebug() << "AssetImporter::~AssetImporter";
}

void AssetImporter::add(const QString &filePath)
{
    m_filePathDict.insert(filePath.toStdString());
    m_filePaths << filePath;
    auto data = m_info["data"].toArray();
    data.append(*res::AssetInfo::get_data(res::AssetInfo::AssetType::MODEL, filePath));
    m_info["data"] = data;
    emit onAddPath();
}

void AssetImporter::addPathsNotExist(const QStringList &filePaths)
{
    int addition_count = 0;
    for (const auto &filePath : filePaths) {
        if (!has(filePath)) {
            m_filePathDict.insert(filePath.toStdString());
            m_filePaths << filePath;
            auto data = m_info["data"].toArray();
            data.append(*res::AssetInfo::get_data(res::AssetInfo::AssetType::MODEL, filePath));
            m_info["data"] = data;
            addition_count++;
        }
    }
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

void AssetImporter::upload()
{
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

