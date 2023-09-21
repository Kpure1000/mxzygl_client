#ifndef MX_USERINFO_H
#define MX_USERINFO_H

#include <QMetaEnum>

namespace res {

class UserInfo : public QObject {
public:

    enum UserPermission {
        ADMIN   = 0x0001,
        NORMAL  = 0x0002
    };
    Q_ENUM(UserPermission)

    static QString getPermissionName(UserPermission per) {
        switch (per) {
        case ADMIN: return "管理员";
        case NORMAL: return "普通";
        }
        return "";
    }
};

}

#endif // MX_USERINFO_H
