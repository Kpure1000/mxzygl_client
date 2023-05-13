#include "layoutmanager.h"

#include "function/configer/configmanager.h"

#include <QDockWidget>
#include <QList>

void LayoutManager::save(QMainWindow *win, const QString &title)
{
    auto winTitle = title == "" ? win->windowTitle() : title;
    ConfigManager::getInstance()->setConfigs({
        {winTitle + "/Geometry", win->saveGeometry()},
        {winTitle + "/State", win->saveState()}
    });
}

void LayoutManager::restore(QMainWindow *win, const QString &title)
{
    auto winTitle = title == "" ? win->windowTitle() : title;
    win->restoreGeometry( ConfigManager::getInstance()->getConfig(winTitle + "/Geometry", win->saveGeometry()).toByteArray() );
    win->restoreState(    ConfigManager::getInstance()->getConfig(winTitle + "/State",    win->saveState()).toByteArray()    );
    QList<QDockWidget* > dwList = win->findChildren<QDockWidget*>();
    for (auto &dw : dwList) {
        win->restoreDockWidget(dw);
    }
}
