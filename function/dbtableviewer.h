#ifndef DBTABLEVIEWER_H
#define DBTABLEVIEWER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

class Client;

class DBTableViewer : public QObject
{
    Q_OBJECT
public:
    explicit DBTableViewer(QObject *parent = nullptr);
    ~DBTableViewer();

    void pullTBInfo();

    QJsonObject *getTableInfo() const { return m_info; }

signals:
    void onResponsing(const QString &info, bool is_continue);
    void onTBPullSuccessful();

private:
    void setInfoData(const QJsonObject &data);

    Client *m_client;

    QJsonObject *m_info;


};

#endif // DBTABLEVIEWER_H
