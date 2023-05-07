#ifndef MX_HOMEWINDOW_H
#define MX_HOMEWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QTabWidget>

class HomeWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit HomeWindow(QWidget* parent = nullptr, QApplication* current_app = nullptr);
    ~HomeWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void makeMenu();

private:
    QMenuBar* m_top_menubar = nullptr;
    QTabWidget * m_tabw = nullptr;

};

#endif // MX_HOMEWINDOW_H
