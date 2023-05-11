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
        RESPONSE_SEARCHTYPE     = 0x10000005    // 搜索模型类型
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
