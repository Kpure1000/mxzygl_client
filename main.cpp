#include <QApplication>
#include <QDebug>

#include <memory>

#include "function/configer/configmanager.h"
#include "function/renderer/renderapi.h"
#include "utils/jobsystem.h"
#include "gui/homewindow.h"
#include "utils/qssloader.h"

int main(int argc, char *argv[])
{
    auto format = QSurfaceFormat::defaultFormat();
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3, 3);
    QSurfaceFormat::setDefaultFormat(format);

    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication a(argc, argv);

    ConfigManager::getInstance("conf.ini");
    RenderAPI::getInstance();
    JobSystem::getInstance(std::max(get_core_numbers() - 2, 2u));

    auto css = QssLoader::getInstance()->loadStyleSheet(":/assets/assets/css/app.css");
    a.setStyleSheet(css);

    auto homeWin = std::make_shared<HomeWindow>(nullptr, &a);
    homeWin->showMaximized();
//    homeWin->show();
    return a.exec();
}
