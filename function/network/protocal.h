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

//        REQUEST_GETFEATURE      = 0x00000001,   // 计算特征
//        RESPONSE_GETFEATURE     = 0x10000001,   // 计算特征

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

        REQUEST_UPLOADMODEL_SIMPLE  = 0x0000000D, // 上传模型(不计算特征值)
        RESPONSE_UPLOADMODEL_SIMPLE = 0x1000000D, // 上传模型(不计算特征值)

        REQUEST_SHOWTABLE     = 0x0000000E,   // 请求显示模型数据表
        RESPONSE_SHOWTABLE    = 0x1000000E,   // 显示模型数据表

        REQUEST_INDEXPULL        = 0x0000000F,  // 请求拉取索引
        RESPONSE_INDEXPULL       = 0x1000000F,  // 拉取索引
        REQUEST_INDEXPUSH        = 0x00000010,  // 请求推送索引
        RESPONSE_INDEXPUSH       = 0x10000010,  // 推送索引

        REQUEST_VERSIONCREATE    = 0x00000011,  // 版本库创建
        RESPONSE_VERSIONCREATE   = 0x10000011,
        REQUEST_VERSIONPULL      = 0x00000012,  // 版本库拉取
        RESPONSE_VERSIONPULL     = 0x10000012,
        REQUEST_VERSIONADD       = 0x00000013,  // 版本增加
        RESPONSE_VERSIONADD      = 0x10000013,
        REQUEST_VERSIONSYNC      = 0x00000014,  // 版本同步
        RESPONSE_VERSIONSYNC     = 0x10000014,
        REQUEST_VERSIONQUERY     = 0x00000015,  // 版本查询
        RESPONSE_VERSIONQUERY    = 0x10000015,
        REQUEST_VERSIONDELETE    = 0x00000016,  // 版本删除
        RESPONSE_VERSIONDELETE   = 0x10000016,
        REQUEST_VERSIONROLLBACK  = 0x00000017,  // 版本回退
        RESPONSE_VERSIONROLLBACK = 0x10000017,

        REQUEST_SC               = 0x00000018,   // 标准分类
        RESPONSE_SC              = 0x10000018,

        REQUEST_PULLSC           = 0x00000019,   // 拉取标准分类
        RESPONSE_PULLSC          = 0x10000019,
        REQUEST_PUSHSC           = 0x0000001A,   // 推送标准分类
        RESPONSE_PUSHSC          = 0x1000001A,

        REQUEST_LOGIN            = 0x0000001B,   // 登录
        RESPONSE_LOGIN           = 0x1000001B,
        REQUEST_SIGNUP           = 0x0000001C,   // 注册
        RESPONSE_SIGNUP          = 0x1000001C,

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
