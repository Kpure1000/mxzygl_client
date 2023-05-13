#ifndef MX_INDEXWINDOW_H
#define MX_INDEXWINDOW_H

#include "gui/ifunctionwindow.h"

class IndexWindow : public IFunctionWindow
{
    Q_OBJECT
public:
    explicit IndexWindow(QWidget *parent = nullptr);

signals:

private:
    QWidget *initIndexWidget();
    void initModelWidget();
    void closeEvent(QCloseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    bool _is_first_paint = true;

};

#endif // MX_INDEXWINDOW_H
