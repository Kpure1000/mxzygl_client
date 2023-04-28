#ifndef MX_OPTIONWIDGET_SERVER_H
#define MX_OPTIONWIDGET_SERVER_H

#include <QWidget>
#include "function/function.h"

#include "gui/optionwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class OptionWidget_Server;
} // namespace Ui
QT_END_NAMESPACE

class OptionWidget_Server : public QWidget
{
    Q_OBJECT
public:
    explicit OptionWidget_Server(QWidget *parent = nullptr);
    ~OptionWidget_Server();

private:
    Ui::OptionWidget_Server *ui;

    struct ServerInfo {
        QString addr;
        int port;
        QString combined() {return addr + ":" + std::to_string(port).c_str();}
    } server_info;

signals:

};

#endif // MX_OPTIONWIDGET_SERVER_H
