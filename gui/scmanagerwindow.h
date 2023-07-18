#ifndef MX_SCMANAGERWINDOW_H
#define MX_SCMANAGERWINDOW_H

#include "gui/ifunctionwindow.h"

class LoggingWidget;
class InfoTableWidget;
class SCManager;

class SCManagerWindow : public IFunctionWindow
{
    Q_OBJECT
public:
    explicit SCManagerWindow(QWidget *parent = nullptr);

signals:

private:
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;

    QWidget *initManagerWidget(SCManager* manager);
    void initCategoryWidget();
    void initLoggingWidget();

private:
    bool _is_first_paint = true;

    SCManager *m_scManager;
    LoggingWidget *m_logging_widget;

    int m_selected_row;
};

#endif // MX_SCMANAGERWINDOW_H
