#ifndef MX_ASSETIMPORTER_H
#define MX_ASSETIMPORTER_H

#include <QObject>
#include <QJsonObject>
#include <unordered_set>
#include <vector>

#include "resource/assetinfo.h"
//#include "utils/qstring_hash.h"

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

    inline bool has(const QString &filePath) const { return m_filePathDict.find(filePath) != m_filePathDict.end(); }

    inline size_t size() const { return m_filePaths.size(); }

    inline QStringList getFilePaths() const { return m_filePaths; }

    QStringList getFilePaths(const std::vector<int> &index) const;

    QStringList getFileNames(const std::vector<int> &index) const;

    std::vector<res::AssetInfo> getAssets() const;

    QJsonObject getAssets_JsonObject() const;

    ImportType type() const { return m_type; }

signals:
    void onAddPath();
    void onAddPaths();
    void onClear();

private:
    std::unordered_set<QString> m_filePathDict;
    QStringList m_filePaths;
    ImportType m_type;

};

#endif // MX_ASSETIMPORTER_H
