#ifndef MX_ASSETIMPORTER_H
#define MX_ASSETIMPORTER_H

#include <QObject>
#include <QJsonObject>
#include <unordered_set>
#include <vector>

#include "resource/assetinfo.h"

class Client;

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

    void upload();

    inline bool has(const QString &filePath) const { return m_filePathDict.find(filePath.toStdString()) != m_filePathDict.end(); }

    inline size_t size() const { return m_filePaths.size(); }

    inline QStringList getFilePaths() const { return m_filePaths; }

    QStringList getFilePaths(const std::vector<int> &index) const;

    QStringList getFileNames(const std::vector<int> &index) const;

    ImportType type() const { return m_type; }

    QJsonObject *getInfoRef() { return &m_info; }

signals:
    void onAddPath();
    void onAddPaths();
    void onClear();

    void onResponsing(const QString & info, bool is_continue);
    void onResponsedSuccessful();
    void onResponsedFailed();

private:
    std::unordered_set<std::string> m_filePathDict;
    QStringList m_filePaths;
    ImportType m_type;
    QJsonObject m_info;

    Client *m_client;

};

#endif // MX_ASSETIMPORTER_H
