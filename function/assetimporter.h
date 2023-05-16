#ifndef MX_ASSETIMPORTER_H
#define MX_ASSETIMPORTER_H

#include <QObject>
#include <QJsonObject>
#include <unordered_set>
#include <vector>

#include "resource/assetinfo.h"
#include "typemanager.h"
#include "tagsmanager.h"

class Client;

class AssetImporter : public QObject
{
    Q_OBJECT
public:
    using ImportType = res::AssetInfo::AssetType;

    enum class ResponseType : int {

    };

    explicit AssetImporter(ImportType type, QObject *parent = nullptr);
    ~AssetImporter();

    void addPathsNotExist(const QStringList &filePaths);

    void clear();

    void pullTypeAndTags();

    void upload();

    inline bool has(const QString &filePath) const { return m_filePathDict.find(filePath.toStdString()) != m_filePathDict.end(); }

    inline size_t size() const { return m_filePaths.size(); }

    inline QStringList getFilePaths() const { return m_filePaths; }

    QStringList getFilePaths(const std::vector<int> &index) const;

    QStringList getFileNames(const std::vector<int> &index) const;

    ImportType type() const { return m_type; }

    QJsonObject *getInfoRef() { return &m_info; }

signals:
    void onAddPaths();
    void onClear();

    void onResponsing(const QString & info, bool is_continue);
    void onImportSuccessful();
    void onTypeAndTagsLoaded();

private:
    std::unordered_set<std::string> m_filePathDict;
    QStringList m_filePaths;
    ImportType m_type;
    QJsonObject m_info;    

    Client *m_client;

    TypeManager *m_typeManager;
    TagsManager *m_tagsManager;

    bool is_typeLoaded = false, is_tagsLoaded = false;

};

#endif // MX_ASSETIMPORTER_H
