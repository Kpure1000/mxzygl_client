#include "optionwindow.h"

#include <QGridLayout>
#include <QTabWidget>

#include "gui/uicomponent/optionwidget_server.h"
#include "gui/uicomponent/optionwidget_asset.h"
#include "gui/uicomponent/optionwidget_render.h"
#include "function/configer/configmanager.h"

OptionWindow::OptionWindow(QWidget *parent)
    : IFunctionWindow("选项", {600,500}, true, false, parent)
{
    auto center_widget = centralWidget();

    auto ly_total = new QGridLayout(center_widget);
    auto tabw = new QTabWidget(center_widget);
    ly_total->addWidget(tabw);

    auto w_server = new OptionWidget_Server(center_widget);
    tabw->addTab(w_server, "服务器");

    auto w_asset = new OptionWidget_Asset(center_widget);
    tabw->addTab(w_asset, "资源");

    auto w_render = new OptionWidget_Render(center_widget);
    tabw->addTab(w_render, "渲染");

    // 持久化上次打开时候的tab index
    connect(tabw, &QTabWidget::currentChanged, this, [](int curIndex){
        ConfigManager::getInstance()->setConfig("OptionWindow/lastTabIndex", curIndex);
    });
    auto lastTabIndex = ConfigManager::getInstance()->getConfig("OptionWindow/lastTabIndex", 0).toInt();
    tabw->setCurrentIndex(lastTabIndex);

}
