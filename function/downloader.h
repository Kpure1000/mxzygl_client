#ifndef MX_DOWNLOADER_H
#define MX_DOWNLOADER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

class Client;

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = nullptr);

    void download();
    void query();

signals:
    void onResponsing(const QString &info, bool is_continue);
    void onDownloadSucceeded();
    void onDownloadFailed(const QString &info);

private:
    Client *m_client;

};

#endif // MX_DOWNLOADER_H
