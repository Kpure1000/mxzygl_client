#include "versioncontroller.h"

#include "function/network/client.h"
#include "function/network/protocal.h"

#include <QDebug>

VersionController::VersionController(QObject *parent)
    : QObject{parent}, m_repo{new QJsonArray{}}, m_version{new QJsonObject{}}
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
//                    emit onResponsing(tr("版本库创建成功!"), false);
                    // 版本库创建成功
                    emit onCreateReopSuccessful(QJsonObject{{"name", QString(extraData)},
                                                            {"cur_version", "-"},
                                                            {"id", "-"},
                                                            {"create_time", "-"}});
                } else if (Protocal::HeaderField::RESPONSE_VERSIONPULL == response_type) {
//                    emit onResponsing(tr("版本库拉取成功!"), false);
                    // 版本库拉取成功
                    setPullData(data);
                    emit onPullReopSuccessful();
                } else if (Protocal::HeaderField::RESPONSE_VERSIONADD == response_type) {
//                    emit onResponsing(tr("版本增加成功!"), false);
                    // 版本增加成功
                    emit onAddVersionSuccessful();
                } else if (Protocal::HeaderField::RESPONSE_VERSIONSYNC == response_type) {
//                    emit onResponsing(tr("版本同步成功!"), false);
                    // 版本同步成功
                    emit onSyncVersionSuccessful();
                } else if (Protocal::HeaderField::RESPONSE_VERSIONQUERY == response_type) {
//                    emit onResponsing(tr("版本查询成功!"), false);
                    // 版本查询成功
                    auto data_tmp = data;
                    QJsonArray headers;
                    for (const auto &h : data["headers"].toArray()) {
                        headers << QJsonObject{
                            {"name", h.toString()},
                            {"editable", false},
                            {"is_array", false},
                            {"visible",  true}
                        };
                    }
                    data_tmp["headers"] = headers;
                    emit onQueryVersionSuccessful(data_tmp);
                } else if (Protocal::HeaderField::RESPONSE_VERSIONDELETE == response_type) {
//                    emit onResponsing(tr("版本删减成功!"), false);
                    // 版本删减成功
                    emit onDeleteVersionSuccessful();
                } else if (Protocal::HeaderField::RESPONSE_VERSIONROLLBACK == response_type) {
//                    emit onResponsing(tr("版本回退成功!"), false);
                    // 版本回退成功
                    emit onRollbackVersionSuccessful();
                }
            }
        }
    });

}

VersionController::~VersionController()
{
    delete m_repo;
    delete m_version;
}

void VersionController::createRepo(const QString &repo_name)
{
    emit onResponsing(tr("开始创建版本库"), true);
    QJsonObject data = {{
        {"type",       static_cast<int>(Protocal::HeaderField::REQUEST_VERSIONCREATE)},
        {"headers",    QJsonArray() << QJsonObject()},
        {"data",       QJsonArray() << QJsonObject()},
        {"repo_name",  repo_name}
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

void VersionController::addVersion(const QString &repo_id, const QString &ver_name)
{
    emit onResponsing(tr("开始增加版本"), true);
    QJsonObject data = {{
        {"type",       static_cast<int>(Protocal::HeaderField::REQUEST_VERSIONADD)},
        {"headers",    QJsonArray() << QJsonObject()},
        {"data",       QJsonArray() << QJsonObject()},
        {"repo",  repo_id},
        {"name",  ver_name},
    }};
    m_client->sendData(data);
}

void VersionController::syncVersion(int repo_index, int ver_index)
{
    emit onResponsing(tr("开始同步版本"), true);
    QJsonObject data = {{
        {"type",       static_cast<int>(Protocal::HeaderField::REQUEST_VERSIONSYNC)},
        {"headers",    QJsonArray() << QJsonObject()},
        {"data",       QJsonArray() << QJsonObject()},
        {"tar_repo_id",getRepo(repo_index)["id"].toString()},
        {"tar_hash",   getVersion(repo_index, ver_index)["hash"].toString()},
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

void VersionController::deleteVersion(int repo_index, int ver_index)
{
    emit onResponsing(tr("开始删除版本"), true);
    QJsonObject data = {{
        {"type",       static_cast<int>(Protocal::HeaderField::REQUEST_VERSIONDELETE)},
        {"headers",    QJsonArray() << QJsonObject()},
        {"data",       QJsonArray() << QJsonObject()},
        {"tar_repo_id",getRepo(repo_index)["id"].toString()},
        {"tar_hash",   getVersion(repo_index, ver_index)["hash"].toString()},
    }};
    m_client->sendData(data);
}

void VersionController::rollbackVersion(int repo_index, int ver_index)
{
    emit onResponsing(tr("开始回退版本"), true);
    QJsonObject data = {{
        {"type",       static_cast<int>(Protocal::HeaderField::REQUEST_VERSIONROLLBACK)},
        {"headers",    QJsonArray() << QJsonObject()},
        {"data",       QJsonArray() << QJsonObject()},
        {"tar_repo_id",getRepo(repo_index)["id"].toString()},
        {"tar_hash",   getVersion(repo_index, ver_index)["hash"].toString()},
    }};
    m_client->sendData(data);
}

QJsonArray *VersionController::getRepoInfo() const
{
    return m_repo;
}

QJsonObject *VersionController::getVersionInf() const
{
    return m_version;
}

QJsonObject VersionController::getRepo(int repo_index) const
{
    if ((*m_repo).size() < repo_index + 1) {
        qDebug() << QString("VersionController::getRepo>> repo data size'%1' < selected size'%2'").arg((*m_repo).size(), repo_index);
        return QJsonObject{};
    }
    return (*m_repo)[repo_index].toObject();
}

QJsonArray VersionController::getVersions(int repo_index) const
{
    const auto & repoObj = getRepo(repo_index);
    if (!repoObj.isEmpty()) {
        auto repoID = repoObj["id"].toString();
        return (*m_version)[repoID].toArray();
    }
    return QJsonArray{};
}

QJsonObject VersionController::getVersion(int repo_index, int version_index) const
{
    const auto & versions = getVersions(repo_index);
    if (versions.size() < version_index + 1) {
        qDebug() << QString("VersionController::getVersion>> version data size'%1' < selected size'%2'").arg(versions.size(), version_index);
        return QJsonObject{};
    }
    return versions[version_index].toObject();
}

int VersionController::getRepoIndexFromName(const QString &name)
{
    int count = 0;
    for (const auto& data : (*m_repo)) {
        const auto & dObj = data.toObject();
        if (dObj["name"].toString() == name) {
            return count;
        }
        count++;
    }
    return -1;
}

int VersionController::getRepoIndexFromID(const QString &id)
{
    int count = 0;
    for (const auto& data : (*m_repo)) {
        const auto & dObj = data.toObject();
        if (dObj["id"].toString() == id) {
            return count;
        }
        count++;
    }
    return -1;
}

int VersionController::getVersionIndexFromHash(int repo_index, const QString &hash)
{
    const auto &vers = getVersions(repo_index);
    int count = 0;
    for (const auto& data : vers) {
        const auto & dObj = data.toObject();
        if (dObj["hash"].toString() == hash) {
            return count;
        }
        count++;
    }
    return -1;
}

void VersionController::addRepoTmp(const QJsonObject &repo_info)
{
    (*m_repo) << repo_info;
    m_repoSet.insert(repo_info["name"].toString().toStdString());
}

bool VersionController::hasRepo(const QString &repo_name) const
{
    return m_repoSet.find(repo_name.toStdString()) != m_repoSet.end();
}

void VersionController::setPullData(const QJsonObject &data)
{
    const auto& dObj = data["data"].toArray()[0].toObject();
    (*m_repo) = dObj["repo"].toArray();
    (*m_version) = dObj["version"].toObject();

    m_repoSet.clear();

    const auto &data_repo = (*m_repo);
    for (const auto &d : data_repo) {
        auto dObj = d.toObject();
        m_repoSet.insert(dObj["name"].toString().toStdString());
    }
}
