#include "loginwidget.h"
#include "ui_loginwidget.h"

#include "function/usermanager.h"

#include <QMessageBox>

LoginWidget::LoginWidget(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::LoginWidget)
{
    ui->setupUi(this);

    ui->bt_signup->setEnabled(false);

    ui->le_name->setFocus();

    m_userManager = UserManager::getInstance();

    connect(m_userManager, &UserManager::onResponsingFailed, this, [=](const QString &info) {
        QMessageBox::warning(this, tr("服务请求错误"), info);
    });

    connect(m_userManager, &UserManager::onLoginSucceeded, this, [=](const QString &name, res::UserInfo::UserPermission per) {
        ui->lb_hint->clear();
        QMessageBox::information(this, tr("验证成功"), QString(tr("%1, 您好！\n您将以'%2'身份登入系统。")).arg(name, res::UserInfo::getPermissionName(per)));
    });

    connect(m_userManager, &UserManager::onLoginFailed, this, [=](const QString &info) {
        ui->lb_hint->setText(QString(R"(<span style='color:#ff8080;'>✘ %1</span>)").arg(info));
    });

    connect(m_userManager, &UserManager::onSignupSucceeded, this, [=](const QString &name, res::UserInfo::UserPermission per) {
        ui->bt_signup->setEnabled(false);
        ui->lb_signupHint->clear();
        ui->le_signupName->clear();
        ui->le_signupPswd->clear();
        ui->le_confirmPswd->clear();

        ui->le_name->setText(name);
        ui->le_pswd->clear();
        ui->lb_hint->clear();
        QMessageBox::information(this, tr("注册成功"), QString(tr("%1, 您好！\n权限为'%2'")).arg(name, res::UserInfo::getPermissionName(per)));
    });

    connect(m_userManager, &UserManager::onSignupFailed, this, [=](const QString &info) {
        ui->lb_signupHint->setText(QString(R"(<span style='color:#ff8080;'>✘ %1</span>)").arg(info));
    });
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::on_bt_login_clicked()
{
    ui->lb_hint->setText(QString(R"(<span>%1</span>)").arg(tr("等待服务器验证...")));
    m_userManager->login(ui->le_name->text(), ui->le_pswd->text());
}


void LoginWidget::on_le_pswd_textEdited(const QString &)
{
    ui->lb_hint->clear();
}


void LoginWidget::on_le_name_textEdited(const QString &)
{
    ui->lb_hint->clear();
}

UserManager *LoginWidget::userManager() const
{
    return m_userManager;
}


void LoginWidget::on_bt_signup_clicked()
{
    if (ui->le_signupPswd->text() != ui->le_confirmPswd->text()) {
        ui->lb_signupHint->setText(QString(R"(<span style='color:#ff8080;'>✘ %1</span>)").arg("密码不一致"));
        ui->bt_signup->setEnabled(false);
        return;
    }
    ui->lb_signupHint->setText(QString(R"(<span>%1</span>)").arg(tr("等待服务器验证...")));
    m_userManager->signup(ui->le_signupName->text(), ui->le_signupPswd->text());
}


void LoginWidget::on_le_signupName_textEdited(const QString &)
{
    ui->lb_signupHint->clear();
}


void LoginWidget::on_le_signupPswd_textEdited(const QString &arg1)
{
    if (arg1 == ui->le_confirmPswd->text()) {
        ui->lb_signupHint->setText(QString(R"(<span>%1</span>)").arg(tr("可以注册")));
        ui->bt_signup->setEnabled(true);
    } else {
        ui->lb_signupHint->setText(QString(R"(<span style='color:#ff8080;'>✘ %1</span>)").arg("密码不一致"));
        ui->bt_signup->setEnabled(false);
    }
}


void LoginWidget::on_le_confirmPswd_textEdited(const QString &arg1)
{
    if (ui->le_signupPswd->text() == arg1) {
        ui->lb_signupHint->setText(QString(R"(<span>%1</span>)").arg(tr("可以注册")));
        ui->bt_signup->setEnabled(true);
    } else {
        ui->lb_signupHint->setText(QString(R"(<span style='color:#ff8080;'>✘ %1</span>)").arg("密码不一致"));
        ui->bt_signup->setEnabled(false);
    }
}

