#ifndef LOGGINGWIDGET_H
#define LOGGINGWIDGET_H

#include <QWidget>

namespace Ui {
class LoggingWidget;
}

class LoggingWidget : public QWidget
{
    Q_OBJECT

public:

    enum class LoggingType : int {
        Info,
        Trace,
        Warning,
        Error
    };
    Q_ENUM(LoggingType)

    explicit LoggingWidget(QWidget *parent = nullptr);
    ~LoggingWidget();

    void add(LoggingType type, const QString& msg);

    inline void info    ( const QString &msg ) { add( LoggingType::Info,    msg ); }
    inline void trace   ( const QString &msg ) { add( LoggingType::Trace,   msg ); }
    inline void warning ( const QString &msg ) { add( LoggingType::Warning, msg ); }
    inline void error   ( const QString &msg ) { add( LoggingType::Error,   msg ); }

    void clear();

private slots:
    void on_bt_clear_clicked();

private:
    Ui::LoggingWidget *ui;
};

#endif // LOGGINGWIDGET_H
