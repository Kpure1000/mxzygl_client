#ifndef MX_PROTOCAL_H
#define MX_PROTOCAL_H

#include <QByteArray>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include <QObject>

class Protocal : public QObject
{
Q_OBJECT
public:

    enum class HeaderField : int {
        RESPONSE_ERROR          = 0x11111111,   // 发生错误

        REQUEST_GETFEATURE      = 0x00000001,   // 计算特征
        RESPONSE_GETFEATURE     = 0x10000001,   // 计算特征

        REQUEST_SEARCHMODEL     = 0x00000002,   // 搜索模型
        RESPONSE_SEARCHMODEL    = 0x10000002,   // 搜索模型

        REQUEST_UPLOADMODEL     = 0x00000003,   // 上传模型
        RESPONSE_UPLOADMODEL    = 0x10000003,   // 上传模型

        REQUEST_SEARCHLABEL     = 0x00000004,   // 搜索模型标签
        RESPONSE_SEARCHLABEL    = 0x10000004,   // 搜索模型标签

        REQUEST_SEARCHTYPE      = 0x00000005,   // 搜索模型类型
        RESPONSE_SEARCHTYPE     = 0x10000005,   // 搜索模型类型

        REQUEST_TYPEINDEX       = 0x00000006,   // 请求类型索引
        RESPONSE_TYPEINDEX      = 0x10000006,   // 类型索引
        REQUEST_LABELINDEX      = 0x00000007,   // 请求标签索引
        RESPONSE_LABELINDEX     = 0x10000007,   // 标签索引
        REQUEST_MODELINDEX      = 0x00000008,   // 请求内容索引
        RESPONSE_MODELINDEX     = 0x10000008,   // 内容索引

        REQUEST_PULLTYPE        = 0x00000009,   // 请求拉取类型
        RESPONSE_PULLTYPE       = 0x10000009,   // 拉取类型
        REQUEST_PULLTAGS        = 0x0000000A,   // 请求拉取标签
        RESPONSE_PULLTAGS       = 0x1000000A,   // 拉取标签

        REQUEST_PUSHTYPE        = 0x0000000B,   // 请求推送类型
        RESPONSE_PUSHTYPE       = 0x1000000B,   // 推送类型
        REQUEST_PUSHTAGS        = 0x0000000C,   // 请求推送标签
        RESPONSE_PUSHTAGS       = 0x1000000C,   // 推送标签
    };
    Q_ENUM(HeaderField)

    Protocal(QObject *parent = nullptr);

    static QJsonObject pack(const QJsonObject &org);
    static QJsonObject unpack(const QJsonObject &packed);

    template <typename T>
    static QByteArray toByteArray(const T &var)
    {
        QByteArray array;
        int len_var = sizeof(var);
        array.resize(len_var);
        memcpy(array.data(), &var, len_var);
        return array;
    }

    template <typename T>
    static T fromByteArray(const QByteArray &array)
    {
        T var;
        memcpy(&var, array.data(), std::min((uint)array.size(), (uint)sizeof(var)));
        return var;
    }

};

#endif // MX_PROTOCAL_H
