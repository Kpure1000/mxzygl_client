#include "loggingwidget.h"
#include "ui_loggingwidget.h"

#include <QTime>

LoggingWidget::LoggingWidget(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::LoggingWidget)
{
    ui->setupUi(this);
}

LoggingWidget::~LoggingWidget()
{
    delete ui;
}

void LoggingWidget::add(LoggingType type, const QString &msg)
{
    switch (type) {
    case LoggingWidget::LoggingType::Info:
        ui->te_logging->append(QString().asprintf("<p>I[%s] %s</p>", QTime::currentTime().toString().toStdString().c_str(), msg.toStdString().c_str()));
        break;
    case LoggingWidget::LoggingType::Trace:
        ui->te_logging->append(QString().asprintf("<p style=\"color:#66a334\";>T[%s] %s</p>", QTime::currentTime().toString().toStdString().c_str(), msg.toStdString().c_str()));
        break;
    case LoggingWidget::LoggingType::Warning:
        ui->te_logging->append(QString().asprintf("<p style=\"color:#a45d24\";>W[%s] %s</p>", QTime::currentTime().toString().toStdString().c_str(), msg.toStdString().c_str()));
        break;
    case LoggingWidget::LoggingType::Error:
        ui->te_logging->append(QString().asprintf("<p style=\"color:#ff6666\";>E[%s] %s</p>", QTime::currentTime().toString().toStdString().c_str(), msg.toStdString().c_str()));
        break;
    }
}

void LoggingWidget::clear()
{
    ui->te_logging->clear();
}

void LoggingWidget::on_bt_clear_clicked()
{
    clear();
}

