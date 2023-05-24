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
        ui->te_logging->append(QString().asprintf("<p><b>I[%s] %s</b></p>", QTime::currentTime().toString().toStdString().c_str(), msg.toStdString().c_str()));
        break;
    case LoggingWidget::LoggingType::Trace:
        ui->te_logging->append(QString().asprintf("<p style=\"color:#008787\";><b>T[%s] %s</b></p>", QTime::currentTime().toString().toStdString().c_str(), msg.toStdString().c_str()));
        break;
    case LoggingWidget::LoggingType::Warning:
        ui->te_logging->append(QString().asprintf("<p style=\"color:#f1d66e\";><b>W[%s] %s</b></p>", QTime::currentTime().toString().toStdString().c_str(), msg.toStdString().c_str()));
        break;
    case LoggingWidget::LoggingType::Error:
        ui->te_logging->append(QString().asprintf("<p style=\"color:#ff6666\";><b>E[%s] %s</b></p>", QTime::currentTime().toString().toStdString().c_str(), msg.toStdString().c_str()));
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

