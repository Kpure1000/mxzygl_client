#ifndef MX_INPUTDAILOG_H
#define MX_INPUTDAILOG_H

#include <QWidget>
#include <functional>

namespace Ui {
class InputDailog;
}

class InputDailog : public QWidget
{
    Q_OBJECT

public:
    explicit InputDailog(const QString &titles,
                         const QString &hint,
                         std::function<bool(const QString &input, QString &info)> rules,
                         QWidget *parent = nullptr);
    ~InputDailog();

    void setHint(const QString & hint);

signals:
    void onInputValueChanged(const QString &input);
    void onAccecpted(const QString &input);
    void onRejected();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::InputDailog *ui;

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // MX_INPUTDAILOG_H
