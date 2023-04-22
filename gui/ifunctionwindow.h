#ifndef MX_IFUNCTIONWINDOW_H
#define MX_IFUNCTIONWINDOW_H

#include <QDialog>
#include <QDebug>

#include "utils/assetmanager.h"

/**
 * @brief 功能弹窗接口
 */
class IFunctionWindow : public QDialog
{
    Q_OBJECT
public:
    IFunctionWindow(const QString& title, QSize size = {800,600}, QWidget *parent = nullptr) : QDialog(parent)
    {
        if(parent->isMaximized()){
            this->showMaximized();
        }
//        this->setAttribute(Qt::WA_DeleteOnClose);
        this->setWindowTitle(title);
        this->resize(size);

        this->setWindowFlags(Qt::Dialog
                             | Qt::WindowMinMaxButtonsHint
                             | Qt::WindowCloseButtonHint);
    }

    void showOnTop()
    {
        setWindowFlag(Qt::WindowStaysOnTopHint,true);
        setWindowFlag(Qt::WindowStaysOnTopHint,false);
    }

    ~IFunctionWindow()
    {
        qDebug() << "Function Window " << this->windowTitle() <<" Decontrusct";
    }
};

class FunctionWnidowManager : public QObject, public IAssetManager<std::string, IFunctionWindow*>
{
    Q_OBJECT
public:
    explicit FunctionWnidowManager(QObject *parent = nullptr) : QObject(parent) {}

    /**
     * @brief show
     * @param title
     * @return true if window is created, else false
     */
    bool show(const std::string& title, bool onTop = true)
    {
        if (!this->has(title)){
            return false;
        }
        auto win = this->get(title);
        if (nullptr == win || win->isHidden()) {
            delete win;
            return false;
        }
        if (onTop)
            win->showOnTop();
        win->show();
        return true;
    }

    void create(const std::string& title, IFunctionWindow* newWindow) {
        this->addAny(title, newWindow);
        newWindow->showOnTop();
        newWindow->show();
    }

    static FunctionWnidowManager *getInstance(QObject *parent = nullptr)
    {
        static FunctionWnidowManager instance_am(parent);
        return &instance_am;
    }

signals:
};

#endif // MX_IFUNCTIONWINDOW_H
