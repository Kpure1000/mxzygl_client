#ifndef MX_ASSETIMPORTER_H
#define MX_ASSETIMPORTER_H

#include <QObject>
#include <QJsonObject>
#include <unordered_set>
#include <vector>
#include <atomic>
#include <chrono>

#include "resource/assetinfo.h"
#include "typemanager.h"
#include "tagsmanager.h"
#include "function/renderer/transform.h"

class Client;
class VersionController;

class AssetImporter : public QObject
{
    Q_OBJECT
public:
    using ImportType = res::AssetInfo::AssetType;

    explicit AssetImporter(ImportType type, QObject *parent = nullptr);
    ~AssetImporter();

    bool addPathsNotExist(const QStringList &filePaths);

    void clear();

    void alignToOrigin();

    void initAllTransform(const Transform &trans_model, const Transform &trans_camera);
    void saveTransform(int row, const Transform &trans_model, const Transform &trans_camera);
    /**
     * return pair[trans_model, trans_camera]
     */
    std::pair<Transform, Transform> getTransform(int row) const;

    void pullTypeAndTags();

    void pullVersionInfo();

    void upload();

    void upload_simple();

    inline bool has(const QString &filePath) const { return m_filePathDict.find(filePath.toStdString()) != m_filePathDict.end(); }

    inline size_t size() const { return m_filePaths.size(); }

    inline QStringList getFilePaths() const { return m_filePaths; }

    QStringList getFilePaths(const std::vector<int> &index) const;

    QStringList getFileNames(const std::vector<int> &index) const;

    ImportType type() const { return m_type; }

    QJsonObject *getInfoRef() { return &m_info; }

signals:
    void onSingleSizeOutofLimit(qint64 singleSize, qint64 limit);
    void onTotalSizeOutofLimit(qint64 totalSize, qint64 newTotalSize, qint64 limit);
    void onAddPaths();
    void onClear();

    void onResponsing(const QString & info, bool is_continue);
    void onStartAlignToOrigin(int totalSize);
    void onDoneAlignToOrigin(int proccessing);
    void saveSuccessful();
    void onUploadSuccessful();
    void onUploadSimpleSuccessful();
    void onTypeAndTagsLoaded();
    void onVersionLoaded();

private:
    void addTypeAndTagsData();
    void addVersionInfo();

    void __upload();

private:
    std::unordered_set<std::string> m_filePathDict;
    QStringList m_filePaths;
    ImportType m_type;
    QJsonObject m_info;    

    Client *m_client;

    TypeManager *m_typeManager;
    TagsManager *m_tagsManager;
    VersionController *m_versionCtrl;

    bool is_typeLoaded = false, is_tagsLoaded = false;
    std::atomic_int m_saveCount = 0;

    std::chrono::steady_clock::time_point m_upload_start;

    qint64 m_totalSize = 0ll;
    qint64 m_singleSizeLimit, m_totalSizeLimit;

};

#endif // MX_ASSETIMPORTER_H
