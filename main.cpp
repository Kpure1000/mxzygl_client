#include <QApplication>

#include <memory>

#include "mxzygl.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ConfigManager::instance("asd.ini");

    auto homeWin = std::make_shared<HomeWindow>(nullptr, &a);
//    homeWin->showMaximized();
    homeWin->show();
    return a.exec();
}
