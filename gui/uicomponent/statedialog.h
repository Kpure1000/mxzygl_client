#ifndef MX_STATEDIALOG_H
#define MX_STATEDIALOG_H

#include <QDialog>

namespace Ui {
class StateDialog;
}

class StateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StateDialog(const QString &title, QWidget *parent = nullptr);
    ~StateDialog();

public slots:
    void doStateChanged(const QString &info, bool state);

private:
    Ui::StateDialog *ui;
};

#endif // MX_STATEDIALOG_H
