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
    void on_bt_save_clicked();

    void on_bt_clearColor_clicked();

    void on_bt_skyColor_clicked();

    void on_bt_groundColor_clicked();

private:
    Ui::OptionWidget_Render *ui;

    QColor m_clearColor, m_skyColor, m_groundColor;
};

#endif // OPTIONWIDGET_RENDER_H
