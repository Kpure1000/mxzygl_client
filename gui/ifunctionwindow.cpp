#include "ifunctionwindow.h"

#include <QDebug>
#include <QKeyEvent>
#include <QApplication>

IFunctionWindow::IFunctionWindow(const QString &title, QSize size, bool delete_when_close, bool showModal, bool maxFollow, QWidget *parent)
    : QMainWindow(parent), m_delete_when_close(delete_when_close), m_max_follow(maxFollow)
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

void IFunctionWindow::keyPressEvent(QKeyEvent *event)
{
    // TODO: 方便调试
    if(event->key() == Qt::Key::Key_Escape) {
        this->close();
    }
}

bool FunctionWnidowManager::show(const std::string &title, bool is_maximized, bool onTop)
{
    if (!this->has(title)){
        return false;
    }
    auto win = this->get(title);
    if (onTop)
        win->showOnTop();
    win->m_max_follow && is_maximized ? win->showMaximized() : win->show();
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

    if (nullptr != newWindow->parent() && dynamic_cast<QWidget*>(newWindow->parent())->isMaximized() && newWindow->m_max_follow)
        newWindow->showMaximized();
    else
        newWindow->show();
}
