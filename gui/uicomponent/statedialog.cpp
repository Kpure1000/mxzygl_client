#include "statedialog.h"
#include "ui_statedialog.h"

#include <QDebug>
#include <QTime>

StateDialog::StateDialog(const QString &title, QWidget *parent) :
      QDialog(parent),
      ui(new Ui::StateDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(title);
    ui->btb->setEnabled(false);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    this->setWindowFlag(Qt::WindowCloseButtonHint, false);
}

StateDialog::~StateDialog()
{
    delete ui;
}

void StateDialog::doStateChanged(const QString &info, bool state)
{
    ui->te_info->append(QTime::currentTime().toString() + ">> " + info);
    if (state == false) {
        ui->btb->setEnabled(true);
    }
}
