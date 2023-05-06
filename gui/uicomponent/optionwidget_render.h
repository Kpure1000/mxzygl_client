#ifndef OPTIONWIDGET_RENDER_H
#define OPTIONWIDGET_RENDER_H

#include <QWidget>
#include <QColor>

namespace Ui {
class OptionWidget_Render;
}

class OptionWidget_Render : public QWidget
{
    Q_OBJECT

public:
    explicit OptionWidget_Render(QWidget *parent = nullptr);
    ~OptionWidget_Render();

private slots:
    void on_bt_color_clicked();

    void on_bt_save_clicked();

private:
    Ui::OptionWidget_Render *ui;

    QColor m_clearColor;
};

#endif // OPTIONWIDGET_RENDER_H
