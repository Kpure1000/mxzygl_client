#include <QApplication>

#include <memory>

#include "function/configer/configer.h"
#include "utils/jobsystem.h"
#include "gui/homewindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ConfigManager::getInstance("conf.ini");
    JobSystem::getInstance(get_core_numbers() - 2);

    auto homeWin = std::make_shared<HomeWindow>(nullptr, &a);
//    homeWin->showMaximized();
    homeWin->show();
    return a.exec();
}
