#include "optionwidget_server.h"

#include "ui_option_server.h"

#include <QDebug>
#include <QKeyEvent>

OptionWidget_Server::OptionWidget_Server(QWidget *parent) : QWidget(parent), ui(new Ui::OptionWidget_Server)
{
    ui->setupUi(this);
    ui->retranslateUi(this);

    auto server_info_raw = ConfigManager::getInstance()->getConfigs({"Server/Addr", "Server/Port"});
    server_info = {server_info_raw[0].toString(), server_info_raw[1].toInt()};

    ui->le_addr->setText(tr(server_info.addr.toStdString().c_str()));
    ui->sb_port->setValue(server_info.port);

    ui->bt_save->setDisabled(true);

    connect(ConfigManager::getInstance(), &ConfigManager::onConfModified, this, [this](){
        auto server_info_raw = ConfigManager::getInstance()->getConfigs({"Server/Addr", "Server/Port"});
        ui->le_addr->setText(server_info_raw[0].toString());
        ui->sb_port->setValue(server_info_raw[1].toInt());
    });

    connect(ui->le_addr,&QLineEdit::textChanged, this, [this](const QString& str){
        server_info.addr = str;
        ui->bt_save->setDisabled(false);
    });

    connect(ui->sb_port, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this](int val){
        server_info.port = val;
        ui->bt_save->setDisabled(false);
    });

    connect(ui->bt_save, &QPushButton::clicked, this, [=]() {
        ConfigManager::getInstance()->setConfigs(
            {{"Server/Addr", server_info.addr}, {"Server/Port", server_info.port}});
        ui->bt_save->setDisabled(true);
        qDebug() << "OptionWidget_Server>>Server Cnofig" << server_info.combined() << " Saved";
    });
}
