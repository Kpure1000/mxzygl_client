#include "ifunctionwindow.h"

#include <QDebug>

IFunctionWindow::IFunctionWindow(const QString &title, QSize size, bool delete_when_close, bool showModal, QWidget *parent)
    : QMainWindow(parent), m_delete_when_close(delete_when_close)
{
    this->setAttribute(Qt::WA_DeleteOnClose, delete_when_close);
    if (showModal)
        this->setWindowModality(Qt::WindowModality::ApplicationModal);

    this->setWindowTitle(title);
    this->resize(size);

    this->setCentralWidget(new QWidget(this));

    this->setWindowFlags(Qt::Dialog
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowCloseButtonHint);
}

void IFunctionWindow::showOnTop()
{
    setWindowFlag(Qt::WindowStaysOnTopHint,true);
    setWindowFlag(Qt::WindowStaysOnTopHint,false);
}

IFunctionWindow::~IFunctionWindow()
{
//    qDebug() << "Function Window " << this->windowTitle() <<" Decontrusct";
}

bool FunctionWnidowManager::show(const std::string &title, bool onTop)
{
    if (!this->has(title)){
        return false;
    }
    auto win = this->get(title);
    if (onTop)
        win->showOnTop();
    win->show();
    return true;
}

void FunctionWnidowManager::create(const std::string &title, IFunctionWindow *newWindow) {
    this->addAny(title, newWindow);
    if (newWindow->m_delete_when_close) {
        connect(newWindow, &IFunctionWindow::destroyed, this, [title, this](){
            this->remove(title);
        });
    }
    newWindow->showOnTop();
    newWindow->show();
}
