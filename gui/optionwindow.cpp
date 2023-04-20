#include "optionwindow.h"

#include <QGridLayout>
#include <QTabWidget>

#include "mxzygl.h"

OptionWindow::OptionWindow(QWidget *parent)
    : IFunctionWindow("选项", {800,600}, parent)
{
    this->setLayout(new QGridLayout(this));
    auto tabw = new QTabWidget(this);
    this->layout()->addWidget(tabw);

    auto w_server = new OptionWidget_Server(this);
    tabw->addTab(w_server, "服务器");

    tabw->addTab(new QWidget, "其他");
}
