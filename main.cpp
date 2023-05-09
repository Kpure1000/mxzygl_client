#include <QApplication>

#include <memory>

#include "function/configer/configmanager.h"
#include "function/renderer/renderapi.h"
#include "utils/jobsystem.h"
#include "gui/homewindow.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication a(argc, argv);

    ConfigManager::getInstance("conf.ini");
    RenderAPI::getInstance();
    JobSystem::getInstance(std::max(get_core_numbers() - 2, 2u));

    auto homeWin = std::make_shared<HomeWindow>(nullptr, &a);
//    homeWin->showMaximized();
    homeWin->show();
    return a.exec();
}
