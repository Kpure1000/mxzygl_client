#ifndef MX_INDEXWINDOW_H
#define MX_INDEXWINDOW_H

#include "gui/ifunctionwindow.h"

class IndexEditor;

class IndexWindow : public IFunctionWindow
{
    Q_OBJECT
public:
    explicit IndexWindow(QWidget *parent = nullptr);

signals:

private:
    QWidget *initModelWidget(IndexEditor *editor);
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;
    bool _is_first_paint = true;

};

#endif // MX_INDEXWINDOW_H
