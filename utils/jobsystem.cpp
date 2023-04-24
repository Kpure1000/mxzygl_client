#include "jobsystem.h"

#if WIN32
#include "windows.h"
#else
#include "unistd.h"
#endif
unsigned int get_core_numbers() {
#if WIN32
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    int allCPUNum_ = sysInfo.dwNumberOfProcessors;
    return allCPUNum_;
#else //linux \
    //获取当前系统的所有CPU核数，包含禁用的
    int allCPUNum_ = sysconf(_SC_NPROCESSORS_CONF);
    //获取当前系统的可用CPU核数
    int enableCPUNum_ = sysconf(_SC_NPROCESSORS_ONLN);
    return enableCPUNum_;
#endif
}
