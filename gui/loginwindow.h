#ifndef MX_LOGINWINDOW_H
#define MX_LOGINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

class LoginWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit LoginWindow(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;

signals:

private:
    void authorization();


    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event) override
    {
        if (event->key() == Qt::Key_Escape) {
            this->close();
        }
    }
};

#endif // MX_LOGINWINDOW_H
