#include "loginwindow.h"

#include <QNetworkReply>
#include <QUrl>
#include <QMessageBox>
#include <QJsonDocument>
#include <QCoreApplication>
#include <QJsonObject>

#include "uicomponent/loginwidget.h"
#include "function/usermanager.h"
#include "gui/homewindow.h"
#include "function/layoutmanager.h"
#include "gui/ifunctionwindow.h"
#include "gui/optionwindow.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>

LoginWindow::LoginWindow(QWidget *parent) : QMainWindow(parent)
{
    authorization();

    this->setWindowTitle(tr("登录"));

    auto top_menubar = new QMenuBar(this);
    this->setMenuBar(top_menubar);
    top_menubar->addAction("服务器选项", this, [this](){
        auto winName = "服务器选项";
        auto fm = FunctionWnidowManager::getInstance();
        if (!fm->show(winName, this->isMaximized(), true)) {
            fm->create(winName, new OptionWindow{this, false});
        }
    });

    auto centW = new QWidget{this};
    this->setCentralWidget(centW);

    auto ly_total = new QVBoxLayout{centW};

    auto loginW = new LoginWidget{centW};
    ly_total->addWidget(loginW, 1);

    auto te_hint = new QTextEdit{centW};
    ly_total->addWidget(te_hint, 0);
    te_hint->setReadOnly(true);
    te_hint->setMaximumHeight(te_hint->fontMetrics().height() * 4.5);

    connect(loginW->userManager(), &UserManager::onResponsing, this, [=](const QString &info, bool is_warning) {
        if (is_warning) {
            te_hint->append(QString(R"(<span style="color: #ebbb1c">%1</span>)").arg(info));
        } else {
            te_hint->append(QString(R"(<span>%1</span>)").arg(info));
        }
    });

    connect(loginW->userManager(), &UserManager::onLoginSucceeded, this, [=]() {
        this->close();
        auto homeWin = new HomeWindow{};
        homeWin->showMaximized();
    });
}

void LoginWindow::closeEvent(QCloseEvent *event)
{
    LayoutManager::getInstance()->save(this, "LoginWindow");
}

void LoginWindow::showEvent(QShowEvent *event)
{
    LayoutManager::getInstance()->restore(this, "LoginWindow");
}

void LoginWindow::authorization()
{
    this->setEnabled(false);
    auto network_access_manager_authorization = new QNetworkAccessManager(this);
    connect(network_access_manager_authorization, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply){
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull()) {
            qDebug() << "Failed to create JSON doc.";
            QMessageBox::critical(this,"错误","鉴权错误！");
            QCoreApplication::exit(1);
            return;
        }

        if (!doc.isObject()) {
            qDebug() << "JSON is not an object.";
            QMessageBox::critical(this,"错误","鉴权错误！");
            QCoreApplication::exit(1);
            return;
        }

        QJsonObject obj = doc.object();
        if (obj.isEmpty()) {
            qDebug() << "JSON object is empty.";
            QMessageBox::critical(this,"错误","鉴权错误！");
            QCoreApplication::exit(1);
            return;
        }

        if (obj.contains("code") && obj["code"] == 0){
            qDebug() << "authorization code is: " << QString::number(obj["code"].toInt());
            this->setEnabled(true);
            return;
        }
        else{
            qDebug() << "authorization code is: " << QString::number(obj["code"].toInt());
            QMessageBox::about(this,"错误","未获授权！");
            QCoreApplication::exit(1);
            return;
        }
    });
    QUrl authorization_url("http://localhost:9722/auth?appcode=APP1605&pincode=1234567890");
    QNetworkRequest authorization_request(authorization_url);
    network_access_manager_authorization->get(authorization_request);
}
