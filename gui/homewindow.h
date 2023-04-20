#ifndef MX_HOMEWINDOW_H
#define MX_HOMEWINDOW_H

#include <QWidget>
#include <QMenuBar>
#include <QTabWidget>

#include "function/function.h"

class HomeWindow : public QWidget
{
    Q_OBJECT
public:
    explicit HomeWindow(QWidget* parent = nullptr, QApplication* current_app = nullptr);
    ~HomeWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    QMenuBar* m_top_menubar = nullptr;
    QTabWidget * m_tabw = nullptr;

};

#endif // MX_HOMEWINDOW_H
