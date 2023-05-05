#ifndef MODELSEARCH_H
#define MODELSEARCH_H

#include <QObject>

#include <memory>
#include <QJsonObject>

#include "resource/searchinfo.h"

class ModelSearch : public QObject
{
    Q_OBJECT
public:
    using SearchType = res::SearchInfo::SearchType;
    explicit ModelSearch(SearchType type, QObject *parent = nullptr);

    void setSearchInfo(const QString &info);

    QJsonObject *getResultsModelInfo() { return m_result_info.get(); }
    QJsonObject *getSearchModelInfo() { return m_search_info.get(); }

    SearchType getType() const { return m_type; }

signals:

private:
    SearchType m_type;

    std::shared_ptr<QJsonObject> m_search_info, m_result_info;

};

#endif // MODELSEARCH_H
