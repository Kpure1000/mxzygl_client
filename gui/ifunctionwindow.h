#ifndef MX_IFUNCTIONWINDOW_H
#define MX_IFUNCTIONWINDOW_H

#include <QDialog>
#include <QDebug>
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

#endif // MX_IFUNCTIONWINDOW_H
