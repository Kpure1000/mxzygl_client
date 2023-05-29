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

signals:
    void onPullSuccessful();
    void onPushSuccessful();

private:
    void setOrgData(const QJsonObject &data);

    IndexType m_type;

    Client *m_client;

    QJsonObject *m_org_info, *m_index_info;

};

#endif // MX_INDEXEDITOR_H
