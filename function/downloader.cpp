#include "downloader.h"

#include "function/network/client.h"
#include "function/network/protocal.h"

Downloader::Downloader(QObject *parent) : QObject(parent)
{
    m_client = new Client(this);

    connect(m_client, &Client::onSendingStateChanged, this, &Downloader::onResponsing);

    connect(m_client, &Client::onReadOver, this, [=](const QJsonObject &data, const QByteArray &extraData) {
        auto response_type = static_cast<Protocal::HeaderField>(data["type"].toInt());
        auto status = data["status"].toString();
        if (Protocal::HeaderField::RESPONSE_ERROR == response_type) {
            emit onResponsing(tr("服务请求错误. Info:") + status, false);
            emit onDownloadFailed(tr("服务请求错误. Info: ") + status);
        } else {
            if (!status.isEmpty()) {
                emit onResponsing(tr("下载失败或接口错误. Info: ") + status, false);
                emit onDownloadFailed(tr("下载失败或接口错误. Info: ") + status);
            } else {
                if (Protocal::HeaderField::RESPONSE_DOWNLOAD == response_type) {
                    emit onResponsing(tr("下载成功!"), false);
                    emit onDownloadSucceeded();
                }
            }
        }
    });

}

void Downloader::download()
{
    emit onResponsing(tr("开始下载"), true);
    QJsonObject data = {{
        {"type",       static_cast<int>(Protocal::HeaderField::REQUEST_DOWNLOAD)},
        {"is_query",      false},
        {"headers",    QJsonArray() << QJsonObject()},
        {"data",       QJsonArray() << QJsonObject()}
    }};
    m_client->sendData(data);
}

void Downloader::query()
{
    emit onResponsing(tr("开始查询"), true);
    QJsonObject data = {{
      {"type",       static_cast<int>(Protocal::HeaderField::REQUEST_DOWNLOAD)},
      {"is_query",      true},
      {"headers",    QJsonArray() << QJsonObject()},
      {"data",       QJsonArray() << QJsonObject()}
    }};
    m_client->sendData(data);
}
