#include "assetimporter.h"

#include <QJsonArray>
#include <QDebug>

AssetImporter::AssetImporter(ImportType type, QObject *parent)
    : QObject{parent}, m_type(type)
{
    QJsonArray headers;
    // 由于在addPathsNotExist中有addition_count检测，m_filePaths一定有元素
    auto header_names = res::AssetInfo::getInfoNameList();
    for (auto& header : header_names) {
        headers.append(header);
    }
    QJsonArray data;
    m_info.insert("headers", headers);
    m_info.insert("data", data);
}

AssetImporter::~AssetImporter()
{
    qDebug() << "AssetImporter::~AssetImporter";
}

void AssetImporter::add(const QString &filePath)
{
    m_filePathDict.insert(filePath.toStdString());
    m_filePaths << filePath;
    auto data = m_info["data"].toArray();
    data.append(*(res::AssetInfo(m_type, filePath).getJsonObject()));
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
            data.append(*(res::AssetInfo(m_type, filePath).getJsonObject()));
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
        fileNames << res::AssetInfo(m_type, m_filePaths[id]).name;
    }
    return fileNames;
}

std::vector<res::AssetInfo> AssetImporter::getAssets() const
{
    std::vector<res::AssetInfo> assets;
    assets.reserve(m_filePaths.size());
    for (const auto &filePath : m_filePaths) {
        assets.emplace_back(m_type, filePath);
    }
    return assets;
}

QJsonObject AssetImporter::getAssets_JsonObject() const
{
    QJsonObject info;
    QJsonArray headers;
    // 由于在addPathsNotExist中有addition_count检测，m_filePaths一定有元素
    auto header_names = res::AssetInfo::getInfoNameList();
    for (auto& header : header_names) {
        headers.append(header);
    }
    QJsonArray data;
    for (const auto &filePath : m_filePaths) {
        auto asset = res::AssetInfo(m_type, filePath);
        data.append(*(asset.getJsonObject()));
    }
    info.insert("headers", headers);
    info.insert("data", data);
    return info;
}

