#ifndef MX_IFUNCTIONWINDOW_H
#define MX_IFUNCTIONWINDOW_H

#include <QMainWindow>

#include "utils/assetmanager.h"

class FunctionWnidowManager;

/**
 * @brief 功能弹窗接口
 */
class IFunctionWindow : public QMainWindow
{
    Q_OBJECT
public:

    IFunctionWindow(const   QString     &title,
                            QSize       size                = {800, 600},
                            bool        delete_when_close   = true,
                            bool        showModal           = false,
                            QWidget     *parent             = nullptr);

    void showOnTop();

    ~IFunctionWindow();

private:
    friend FunctionWnidowManager;
    bool m_delete_when_close;

};

class FunctionWnidowManager : public QObject, public IAssetManager<std::string, IFunctionWindow*>
{
    Q_OBJECT
public:
    explicit FunctionWnidowManager(QObject *parent = nullptr) : QObject(parent) {}

    /**
     * @brief show
     * @param title
     * @return if window is created, return true
     */
    bool show(const std::string& title, bool onTop = true);

    void create(const std::string& title, IFunctionWindow* newWindow);

    static FunctionWnidowManager *getInstance(QObject *parent = nullptr)
    {
        static FunctionWnidowManager instance_am(parent);
        return &instance_am;
    }

signals:
};

#endif // MX_IFUNCTIONWINDOW_H
