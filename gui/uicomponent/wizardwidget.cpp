#include "wizardwidget.h"
#include "ui_wizardwidget.h"

#include <QDebug>

WizardWidget::WizardStep::WizardStep(std::function<QWidget *()> widget_creator, const QString &name)
    : widget_creator(widget_creator), widget(nullptr), name(name)
{}

template<class T>
void WizardWidget::WizardStep::show(T *layout)
{
    widget = widget_creator();
    layout->addWidget(widget);
    widget->show();
}

void WizardWidget::WizardStep::hide()
{
    widget->hide();
}

WizardWidget::WizardWidget(QWidget *parent) : QWidget(parent), ui(new Ui::WizardWidget)
{
    ui->setupUi(this);
    ui->bt_next->setEnabled(false);
}

WizardWidget::~WizardWidget()
{
    delete ui;
}

void WizardWidget::setNextButtonEnable(bool cango)
{
    ui->bt_next->setEnabled(cango);
}

void WizardWidget::pushStep(std::function<QWidget *()> func, const QString &name)
{   
    m_steps.emplace_back(std::make_shared<WizardStep>(func, name));
    if (m_steps.size() == 1) {
        ui->bt_next->setEnabled(true);
        m_currentIndex = 0;
        m_steps[m_currentIndex]->show(ui->gb_stepName->layout());
        ui->gb_stepName->setTitle(QString().asprintf("第%d步: %s", m_currentIndex + 1, name.toStdString().c_str()));
        emit onSwitchStep();
    }
}

std::shared_ptr<WizardWidget::WizardStep> WizardWidget::currentStep()
{
    if (m_currentIndex >= 0)
        return m_steps[m_currentIndex];
    qDebug() << "WizardWidget::currentStep>> Step is Empty!";
    return nullptr;
}

void WizardWidget::on_bt_next_clicked()
{
    m_currentIndex++;
    if (m_currentIndex < static_cast<int>(m_steps.size())) {
        m_steps[m_currentIndex]->show(ui->gb_stepName->layout());
        m_steps[m_currentIndex - 1]->hide();
        ui->gb_stepName->setTitle(QString().asprintf("第%d步: %s", m_currentIndex + 1, m_steps[m_currentIndex]->name.toStdString().c_str()));
        emit onSwitchStep();
    }
    if (m_currentIndex == static_cast<int>(m_steps.size()) - 1) {
        ui->bt_next->setEnabled(false);
    }
}
