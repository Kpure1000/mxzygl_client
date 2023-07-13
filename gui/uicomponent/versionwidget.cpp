#include "versionwidget.h"
#include "ui_versionwidget.h"

#include <QMessageBox>

#include "function/versioncontroller.h"

VersionWidget::VersionWidget(VersionController * vctrl, QWidget *parent) :
    QWidget(parent), ui(new Ui::VersionWidget), m_vctrl{vctrl}
{
    ui->setupUi(this);

    connect(ui->bt_create, &QPushButton::clicked, this, [=](){
        m_vctrl->createRepo();
    });
    connect(ui->bt_pull, &QPushButton::clicked, this, [=](){
        m_vctrl->pullRepo();
    });
    connect(ui->bt_add, &QPushButton::clicked, this, [=](){
        m_vctrl->addVersion();
    });
    connect(ui->bt_sync, &QPushButton::clicked, this, [=](){
        m_vctrl->syncVersion();
    });
    connect(ui->bt_query, &QPushButton::clicked, this, [=](){
        m_vctrl->queryVersion();
    });
    connect(ui->bt_delet, &QPushButton::clicked, this, [=](){
        m_vctrl->deleteVersion();
    });
    connect(ui->bt_rollback, &QPushButton::clicked, this, [=](){
        m_vctrl->rollbackVersion();
    });


    connect(m_vctrl, &VersionController::onCreateReopSuccessful, this, [=](){
        QMessageBox::information(this, tr("版本库创建"), tr("版本库创建成功!"));
    });

    connect(m_vctrl, &VersionController::onPullReopSuccessful, this, [=](){
        QMessageBox::information(this, tr("版本库拉取"), tr("版本库拉取成功!"));
    });

    connect(m_vctrl, &VersionController::onAddVersionSuccessful, this, [=](){
        QMessageBox::information(this, tr("版本增加"), tr("版本增加成功!"));
    });

    connect(m_vctrl, &VersionController::onSyncVersionSuccessful, this, [=](){
        QMessageBox::information(this, tr("版本同步"), tr("版本同步成功!"));
    });

    connect(m_vctrl, &VersionController::onQueryVersionSuccessful, this, [=](){
        QMessageBox::information(this, tr("版本查询"), tr("版本查询成功!"));
    });

    connect(m_vctrl, &VersionController::onDeleteVersionSuccessful, this, [=](){
        QMessageBox::information(this, tr("版本删除"), tr("版本删除成功!"));
    });

    connect(m_vctrl, &VersionController::onRollbackVersionSuccessful, this, [=](){
        QMessageBox::information(this, tr("版本回退"), tr("版本回退成功!"));
    });

}

VersionWidget::~VersionWidget()
{
    delete ui;
}
