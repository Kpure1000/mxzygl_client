#include "optionwindow.h"

#include <QGridLayout>
#include <QTabWidget>

#include "gui/uicomponent/optionwidget_server.h"

OptionWindow::OptionWindow(QWidget *parent)
    : IFunctionWindow("选项", {600,500}, true, parent)
{
    this->setLayout(new QGridLayout(this));
    auto tabw = new QTabWidget(this);
    this->layout()->addWidget(tabw);

    auto w_server = new OptionWidget_Server(this);
    tabw->addTab(w_server, "服务器");

    tabw->addTab(new QWidget, "其他");
}
