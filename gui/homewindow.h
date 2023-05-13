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
    void closeEvent(QCloseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void makeMenu();
    void saveLayout();
    void restoreLayout();

private:
    QMenuBar* m_top_menubar = nullptr;
    QTabWidget * m_tabw = nullptr;

    bool _is_first_paint = true;

};

#endif // MX_HOMEWINDOW_H
