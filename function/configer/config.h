#ifndef MX_CONFIG_H
#define MX_CONFIG_H

#include <QObject>

namespace conf {

    struct ServerInfo {
        std::string addr;
        int port;
        inline QString combined() { return (addr + ':' + std::to_string(port)).c_str(); }
    };

    struct RenderConf {
        float clearColor[4];
    };

}

#endif // MX_CONFIG_H
