#include "optionwindow.h"

#include <QGridLayout>
#include <QTabWidget>

#include "gui/uicomponent/optionwidget_server.h"
#include "gui/uicomponent/optionwidget_asset.h"
#include "gui/uicomponent/optionwidget_render.h"

OptionWindow::OptionWindow(QWidget *parent)
    : IFunctionWindow("选项", {600,500}, true, parent)
{
    this->setLayout(new QGridLayout(this));
    auto tabw = new QTabWidget(this);
    this->layout()->addWidget(tabw);

    auto w_server = new OptionWidget_Server(this);
    tabw->addTab(w_server, "服务器");

    auto w_asset = new OptionWidget_Asset(this);
    tabw->addTab(w_asset, "资源");

    auto w_render = new OptionWidget_Render(this);
    tabw->addTab(w_render, "渲染");
}
