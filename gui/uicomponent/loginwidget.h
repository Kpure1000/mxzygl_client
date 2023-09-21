#ifndef MX_LOGINWIDGET_H
#define MX_LOGINWIDGET_H

#include <QWidget>

namespace Ui {
class LoginWidget;
}

class UserManager;

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

    UserManager *userManager() const;

private slots:
    void on_bt_login_clicked();

    void on_le_pswd_textEdited(const QString &);

    void on_le_name_textEdited(const QString &);

    void on_bt_signup_clicked();

    void on_le_signupName_textEdited(const QString &arg1);

    void on_le_signupPswd_textEdited(const QString &arg1);

    void on_le_confirmPswd_textEdited(const QString &arg1);

private:
    Ui::LoginWidget *ui;

    UserManager *m_userManager;
};

#endif // MX_LOGINWIDGET_H
