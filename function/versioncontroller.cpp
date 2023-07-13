#include "versioncontroller.h"

#include "function/network/client.h"
#include "function/network/protocal.h"

VersionController::VersionController(QObject *parent)
    : QObject{parent}
{
    m_client = new Client(this);

    connect(m_client, &Client::onSendingStateChanged, this, &VersionController::onResponsing);

    connect(m_client, &Client::onReadOver, this, [=](const QJsonObject &data, const QByteArray &extraData) {
        auto response_type = static_cast<Protocal::HeaderField>(data["type"].toInt());
        auto status = data["status"].toString();
        if (Protocal::HeaderField::RESPONSE_ERROR == response_type) {
            emit onResponsing(tr("服务请求错误. Info:") + status, false);
        } else {
            if (!status.isEmpty()) {
                emit onResponsing(tr("版本管理请求失败. Info: ") + status, false);
            } else {
                if (Protocal::HeaderField::RESPONSE_VERSIONCREATE == response_type) {
                    emit onResponsing(tr("版本库创建成功!"), false);
                    // TODO: 版本库创建成功
                    emit onCreateReopSuccessful();
                } else if (Protocal::HeaderField::RESPONSE_VERSIONPULL == response_type) {
                    emit onResponsing(tr("版本库拉取成功!"), false);
                    // TODO: 版本库拉取成功
                    emit onPullReopSuccessful();
                } else if (Protocal::HeaderField::RESPONSE_VERSIONADD == response_type) {
                    emit onResponsing(tr("版本增加成功!"), false);
                    // TODO: 版本增加成功
                    emit onAddVersionSuccessful();
                } else if (Protocal::HeaderField::RESPONSE_VERSIONSYNC == response_type) {
                    emit onResponsing(tr("版本同步成功!"), false);
                    // TODO: 版本同步成功
                    emit onSyncVersionSuccessful();
                } else if (Protocal::HeaderField::RESPONSE_VERSIONQUERY == response_type) {
                    emit onResponsing(tr("版本查询成功!"), false);
                    // TODO: 版本查询成功
                    emit onQueryVersionSuccessful();
                } else if (Protocal::HeaderField::RESPONSE_VERSIONDELETE == response_type) {
                    emit onResponsing(tr("版本删减成功!"), false);
                    // TODO: 版本删减成功
                    emit onDeleteVersionSuccessful();
                } else if (Protocal::HeaderField::RESPONSE_VERSIONROLLBACK == response_type) {
                    emit onResponsing(tr("版本回退成功!"), false);
                    // TODO: 版本回退成功
                    emit onRollbackVersionSuccessful();
                }
            }
        }
    });

}

void VersionController::createRepo()
{
    emit onResponsing(tr("开始创建版本库"), true);
    QJsonObject data = {{
        {"type",       static_cast<int>(Protocal::HeaderField::REQUEST_VERSIONCREATE)},
        {"headers",    QJsonArray() << QJsonObject()},
        {"data",       QJsonArray() << QJsonObject()}
    }};
    m_client->sendData(data);
}

void VersionController::pullRepo()
{
    emit onResponsing(tr("开始拉取版本库"), true);
    QJsonObject data = {{
        {"type",       static_cast<int>(Protocal::HeaderField::REQUEST_VERSIONPULL)},
        {"headers",    QJsonArray() << QJsonObject()},
        {"data",       QJsonArray() << QJsonObject()}
    }};
    m_client->sendData(data);
}

void VersionController::addVersion()
{
    emit onResponsing(tr("开始增加版本"), true);
    QJsonObject data = {{
        {"type",       static_cast<int>(Protocal::HeaderField::REQUEST_VERSIONADD)},
        {"headers",    QJsonArray() << QJsonObject()},
        {"data",       QJsonArray() << QJsonObject()}
    }};
    m_client->sendData(data);
}

void VersionController::syncVersion()
{
    emit onResponsing(tr("开始同步版本"), true);
    QJsonObject data = {{
        {"type",       static_cast<int>(Protocal::HeaderField::REQUEST_VERSIONSYNC)},
        {"headers",    QJsonArray() << QJsonObject()},
        {"data",       QJsonArray() << QJsonObject()}
    }};
    m_client->sendData(data);
}

void VersionController::queryVersion()
{
    emit onResponsing(tr("开始查询版本"), true);
    QJsonObject data = {{
        {"type",       static_cast<int>(Protocal::HeaderField::REQUEST_VERSIONQUERY)},
        {"headers",    QJsonArray() << QJsonObject()},
        {"data",       QJsonArray() << QJsonObject()}
    }};
    m_client->sendData(data);
}

void VersionController::deleteVersion()
{
    emit onResponsing(tr("开始删除版本"), true);
    QJsonObject data = {{
        {"type",       static_cast<int>(Protocal::HeaderField::REQUEST_VERSIONDELETE)},
        {"headers",    QJsonArray() << QJsonObject()},
        {"data",       QJsonArray() << QJsonObject()}
    }};
    m_client->sendData(data);
}

void VersionController::rollbackVersion()
{
    emit onResponsing(tr("开始回退版本"), true);
    QJsonObject data = {{
        {"type",       static_cast<int>(Protocal::HeaderField::REQUEST_VERSIONROLLBACK)},
        {"headers",    QJsonArray() << QJsonObject()},
        {"data",       QJsonArray() << QJsonObject()}
    }};
    m_client->sendData(data);
}
