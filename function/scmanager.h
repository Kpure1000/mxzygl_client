#ifndef SCMANAGER_H
#define SCMANAGER_H

#include <QObject>

#include <QJsonObject>
#include <unordered_set>

class Client;

class SCManager : public QObject
{
    Q_OBJECT
public:
    explicit SCManager(QObject *parent = nullptr);
    ~SCManager();

    void pull();

    void push();

    void add(const QString &typeName);

    void remove(int index);

    void modify(int index, const QString &name);

    QString sc(int index) const;

    QJsonObject *getSCInfo() const { return m_scInfo; }
    QJsonArray getSCNameList();

signals:
    void onResponsing(const QString &info, bool is_continue);

    void onPullSuccessful();
    void onPushSuccessful();

    void onAddRepeat(const QString &typeName);
    void onAddSuccessed();

    void onRemoveSuccessed();

    void onModifyRepeat(const QString &typeName);
    void onModifySuccessed();

private:
    void setScData(const QJsonObject &data);

    Client *m_client;

    QJsonObject *m_scInfo;

    std::unordered_set<std::string> m_nameSet;
};

#endif // SCMANAGER_H
