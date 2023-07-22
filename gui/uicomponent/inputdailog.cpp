#include "inputdailog.h"
#include "ui_inputdailog.h"

#include <QPushButton>
#include <QKeyEvent>

InputDailog::InputDailog(const QString &titles,
                         const QString &hint,
                         std::function<bool(const QString &, QString &)> rules,
                         QWidget *parent)
    : QWidget(parent), ui(new Ui::InputDailog)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    this->setWindowModality(Qt::WindowModality::ApplicationModal);

    this->setAttribute(Qt::WA_DeleteOnClose);

    this->setWindowTitle(titles);
    ui->lb_hint->setText(hint);

    ui->lb_check->setText("");

    ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(false);

    connect(ui->le_input, &QLineEdit::textChanged, this, [=](const QString &text) {
        emit onInputValueChanged(text);
        QString info{};
        auto res = rules(text, info);
        ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(res);
        if (res) {
            ui->lb_check->setText(QString(R"(<div style='color:#66a334;'>✔ %1</div>)").arg(info));
        } else {
            ui->lb_check->setText(QString(R"(<div style='color:#ff8080;'>✘ %1</div>)").arg(info));
        }
    });
}

InputDailog::~InputDailog()
{
    delete ui;
}

void InputDailog::setHint(const QString &hint)
{
    ui->lb_hint->setText(hint);
}

void InputDailog::on_buttonBox_accepted()
{
    emit onAccecpted(ui->le_input->text());
    this->close();
}

void InputDailog::on_buttonBox_rejected()
{
    emit onRejected();
    this->close();
}

void InputDailog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key::Key_Escape) {
        emit ui->buttonBox->rejected();
    }
    if(event->key() == Qt::Key::Key_Enter) {
        emit ui->buttonBox->accepted();
    }
}
