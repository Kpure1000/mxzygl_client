#ifndef WIZARDWIDGET_H
#define WIZARDWIDGET_H

#include <QWidget>
#include <vector>
#include <memory>
#include <functional>

namespace Ui {
class WizardWidget;
}

class WizardWidget : public QWidget
{
    Q_OBJECT

public:
    struct WizardStep {
        std::function<QWidget *()> widget_creator;
        QWidget *widget;
        QString name;
        explicit WizardStep(std::function<QWidget *()> widget_creator, const QString &name);

        template<class T>
        void show(T*layout);
        void hide();
    };

    explicit WizardWidget(QWidget *parent = nullptr);
    ~WizardWidget();

    void setNextButtonEnable(bool cango);

    void pushStep(std::function<QWidget *()> func, const QString &name);

    std::shared_ptr<WizardStep> currentStep();

signals:
    void onSwitchStep();

private slots:
    void on_bt_next_clicked();

private:
    Ui::WizardWidget *ui;

    std::vector<std::shared_ptr<WizardStep>> m_steps;
    int m_currentIndex = -1;
};

#endif // WIZARDWIDGET_H
