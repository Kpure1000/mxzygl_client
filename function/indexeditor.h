#ifndef MX_INDEXEDITOR_H
#define MX_INDEXEDITOR_H

#include <QObject>
#include <QJsonObject>

#include "resource/indexinfo.h"

class Client;

class IndexEditor : public QObject
{
    Q_OBJECT
public:

    using IndexType = res::IndexInfo::IndexType;

    explicit IndexEditor(IndexType type, QObject *parent = nullptr);
    ~IndexEditor();

    QJsonObject *getOriginInfo() const { return m_org_info; }
    QJsonObject *getIndexInfo() const { return m_index_info; }

    IndexType getType() { return m_type; }

    void pull_org();

    void compressToIndex();

    void syncToOrg();

    void push_org();

    QStringList getFilePaths(const std::vector<int> &index) const;

    QStringList getPreviewInfo(const std::vector<int> &index) const;

    QString index(int row)const;

    void modify(int index, const QString &name);

signals:
    void onResponsing(const QString &info, bool is_continue);
    void onPullSuccessful();
    void onPushSuccessful();
    void onCompressed();
    void onSyncToOrg();

    void onModifyRepeat(const QString &name);
    void onModifySuccessed();

private:
    void setOrgData(const QJsonObject &data);

    IndexType m_type;

    Client *m_client;

    QJsonObject *m_org_info, *m_index_info;

};

#endif // MX_INDEXEDITOR_H
