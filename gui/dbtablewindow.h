#ifndef DBTABLEWINDOW_H
#define DBTABLEWINDOW_H

#include "ifunctionwindow.h"
#include <QMainWindow>

class LoggingWidget;
class DBTableViewer;

class DBTableWindow : public IFunctionWindow
{
    Q_OBJECT
public:
    DBTableWindow(QWidget *parent = nullptr);

    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    bool _is_first_paint = true;
    LoggingWidget *m_logging_widget;

    QWidget *setupTableWidget(DBTableViewer *);
};

#endif // DBTABLEWINDOW_H
