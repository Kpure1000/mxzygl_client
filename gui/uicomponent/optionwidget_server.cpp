#include "optionwidget_server.h"

#include "ui_option_server.h"

#include <QDebug>
#include <QKeyEvent>

#include "mxzygl.h"

OptionWidget_Server::OptionWidget_Server(QWidget *parent) : QWidget(parent), ui(new Ui::OptionWidget_Server)
{
    ui->setupUi(this);
    ui->retranslateUi(this);

    server_info = ConfigManager::getInstance()->getServer();

    ui->le_addr->setText(tr(server_info.addr.c_str()));
    ui->sb_port->setValue(server_info.port);

    ui->bt_save->setDisabled(true);

    connect(ui->le_addr,&QLineEdit::textChanged, this, [this](const QString& str){
        server_info.addr = str.toStdString();
        ui->bt_save->setDisabled(false);
    });

    connect(ui->sb_port, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this](int val){
        server_info.port = val;
        ui->bt_save->setDisabled(false);
    });

    connect(ui->bt_save, &QPushButton::clicked, this, [=](){
        ConfigManager::getInstance()->setServer(server_info);
        ui->bt_save->setDisabled(true);
        qDebug() << "OptionWidget_Server>>Server Cnofig" << server_info.combined() << " Saved";
    });

}
