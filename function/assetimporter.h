#ifndef MX_ASSETIMPORTER_H
#define MX_ASSETIMPORTER_H

#include <QObject>
#include <QJsonObject>
#include <unordered_set>
#include <vector>

#include "resource/assetinfo.h"

class AssetImporter : public QObject
{
    Q_OBJECT
public:
    using ImportType = res::AssetInfo::AssetType;

    explicit AssetImporter(ImportType type, QObject *parent = nullptr);
    ~AssetImporter();

    void add(const QString &filePath);

    void addPathsNotExist(const QStringList &filePaths);

    void clear();

    inline bool has(const QString &filePath) const { return m_filePathDict.find(filePath.toStdString()) != m_filePathDict.end(); }

    inline size_t size() const { return m_filePaths.size(); }

    inline QStringList getFilePaths() const { return m_filePaths; }

    QStringList getFilePaths(const std::vector<int> &index) const;

    QStringList getFileNames(const std::vector<int> &index) const;

    std::vector<res::AssetInfo> getAssets() const;

    QJsonObject getAssets_JsonObject() const;

    ImportType type() const { return m_type; }

    QJsonObject *getInfoRef() { return &m_info; }

signals:
    void onAddPath();
    void onAddPaths();
    void onClear();

private:
    std::unordered_set<std::string> m_filePathDict;
    QStringList m_filePaths;
    ImportType m_type;
    QJsonObject m_info;

};

#endif // MX_ASSETIMPORTER_H
