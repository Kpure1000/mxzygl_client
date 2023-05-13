#ifndef MODELSEARCH_H
#define MODELSEARCH_H

#include <QObject>

#include <memory>
#include <QJsonObject>

#include "resource/searchinfo.h"

class Client;

class ModelSearch : public QObject
{
    Q_OBJECT
public:
    using SearchType = res::SearchInfo::SearchType;
    explicit ModelSearch(SearchType type, QObject *parent = nullptr);

    void setSearchInfo(const QString &info);

    void search();

    void clearResults();

    QJsonObject *getResultsModelInfo() { return m_result_info.get(); }
    QJsonObject *getSearchModelInfo() { return m_search_info.get(); }

    SearchType getType() const { return m_type; }

    QStringList getFilePaths(const std::vector<int> &index) const;

    QStringList getPreviewInfo(const std::vector<int> &index) const;

signals:
    void onResponsing(const QString & info, bool is_continue);
//    void onSearchOver(const QString & info, bool is_successful);
    void onResultUpdate();
    void onResultClear();

private:
    void setResultInfo(const QJsonObject &result);

private:
    SearchType m_type;

    std::shared_ptr<QJsonObject> m_search_info, m_result_info;
    QString m_search_keyword;

    Client *m_client;

};

#endif // MODELSEARCH_H
