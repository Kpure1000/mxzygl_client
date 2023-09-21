#include "usermanager.h"

#include "function/network/client.h"
#include "function/network/protocal.h"

#include <QJsonObject>
#include <QJsonArray>

UserManager::UserManager(QObject *parent) : QObject(parent)
{
    m_client = new Client{this};

    connect(m_client, &Client::onSendingStateChanged, this, &UserManager::onResponsing);

    connect(m_client, &Client::onReadOver, this, [=](const QJsonObject &data, const QByteArray &) {
        auto response_type = static_cast<Protocal::HeaderField>(data["type"].toInt());
        auto status = data["status"].toString();
        if (Protocal::HeaderField::RESPONSE_ERROR == response_type) {
            emit onResponsingFailed(tr("服务请求错误. Info:") + status);
        } else {
            if (!status.isEmpty()) {
                if (Protocal::HeaderField::RESPONSE_LOGIN == response_type) {
                    emit onLoginFailed(status);
                } else if (Protocal::HeaderField::RESPONSE_SIGNUP == response_type) {
                    emit onSignupFailed(status);
                }
            } else {
                if (Protocal::HeaderField::RESPONSE_LOGIN == response_type) {
                    m_name = data["name"].toString();
                    m_permission = static_cast<res::UserInfo::UserPermission>(data["permission"].toInt());
                    emit onLoginSucceeded(m_name, m_permission);
                } else if (Protocal::HeaderField::RESPONSE_SIGNUP == response_type) {
                    emit onSignupSucceeded(data["name"].toString(), static_cast<res::UserInfo::UserPermission>(data["permission"].toInt()));
                }
            }
        }
    });


}

void UserManager::login(const QString &name, const QString &pswd)
{
    QJsonObject data = {{
        {"type",       static_cast<int>(Protocal::HeaderField::REQUEST_LOGIN)},
        {"headers",    QJsonArray() << QJsonObject()},
        {"data",       QJsonArray() << QJsonObject()},
        {"name",       name},
        {"pswd",       pswd}
    }};
    m_client->sendData(data);
}

void UserManager::signup(const QString &name, const QString &pswd)
{
    QJsonObject data = {{
        {"type",       static_cast<int>(Protocal::HeaderField::REQUEST_SIGNUP)},
        {"headers",    QJsonArray() << QJsonObject()},
        {"data",       QJsonArray() << QJsonObject()},
        {"name",       name},
        {"pswd",       pswd}
    }};
    m_client->sendData(data);
}
