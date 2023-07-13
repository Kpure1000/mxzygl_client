#ifndef MX_VERSIONWINDOW_H
#define MX_VERSIONWINDOW_H

#include "gui/ifunctionwindow.h"

class LoggingWidget;

class VersionWindow : public IFunctionWindow
{
    Q_OBJECT
public:
    explicit VersionWindow(QWidget *parent = nullptr);

signals:

private:
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;
    bool _is_first_paint = true;
    LoggingWidget *m_logging_widget;
};

#endif // MX_VERSIONWINDOW_H
