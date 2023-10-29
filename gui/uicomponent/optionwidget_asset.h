#ifndef OPTIONWIDGET_ASSET_H
#define OPTIONWIDGET_ASSET_H

#include <QWidget>

namespace Ui {
class OptionWidget_Asset;
}

class OptionWidget_Asset : public QWidget
{
    Q_OBJECT

public:
    explicit OptionWidget_Asset(QWidget *parent = nullptr);
    ~OptionWidget_Asset();

private:
    Ui::OptionWidget_Asset *ui;

    double m_single_size_limit;
    double m_total_size_limit;

    double m_cache_timeout;
    double m_cache_timeout_retry;
};

#endif // OPTIONWIDGET_ASSET_H
