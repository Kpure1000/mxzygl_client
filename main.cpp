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

    a.setStyleSheet(R"(
QWidget {
    background-color:#3e3e3e;
    color:white;
    font-weight:bold;
}

QDockWidget {
    background-color:#5a5a5a;
}

QMainWindow::separator {
    width: 7px;
    border: 2px dashed #e2e2e2;
    background-color:#5a5a5a;
}

QDockWidget::item {
    background-color:#5a5a5a;
}

QMenu {
    background-color:#3e3e3e;
}
QMenu::item::selected {
    background-color:#525252;
}
QMenu::item::pressed {
    background-color:#4a4a4a;
}
QMenu::item::disabled {
    background-color:#393939;
}

QMenuBar {
    background-color:transparent;
}

QMenuBar::selected {
    background-color:#6a6a6a;
}

QMenuBar::item:selected {
    background-color:#5a5a5a;
}

QSplitter::handle {
    border: 2px dashed #e2e2e2;
    background-color:#5a5a5a;
}

QTabWidget::pane {
    border: 2px solid #828282;
}
QTabBar::tab {
    background-color:#1e1e1e;
}
QTabBar::tab::selected {
    background-color:#5a5a5a;
}

QTableWidget {
    color:black;
}
QHeaderView {
    color:black;
}

QPushButton::hover {
    background-color:#525252;
}

QPushButton::pressed {
    background-color:#4a4a4a;
}

QPushButton::disabled {
    background-color:#393939;
    color: #888888;
}

)");

    auto homeWin = std::make_shared<HomeWindow>(nullptr, &a);
    homeWin->showMaximized();
//    homeWin->show();
    return a.exec();
}
