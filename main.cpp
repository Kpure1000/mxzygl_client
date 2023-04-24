#include <QApplication>

#include <memory>

#include "mxzygl.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ConfigManager::getInstance("conf.ini");
    JobSystem::getInstance(get_core_numbers());

    auto homeWin = std::make_shared<HomeWindow>(nullptr, &a);
//    homeWin->showMaximized();
    homeWin->show();
    return a.exec();
}
