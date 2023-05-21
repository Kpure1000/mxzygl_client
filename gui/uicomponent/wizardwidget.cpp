#include "wizardwidget.h"
#include "ui_wizardwidget.h"

#include <QDebug>

WizardWidget::WizardStep::WizardStep(QWidget *widget, const QString &name) : widget(widget), name(name)
{}

WizardWidget::WizardWidget(QWidget *parent) : QWidget(parent), ui(new Ui::WizardWidget)
{
    ui->setupUi(this);
    ui->bt_next->setEnabled(false);
}

WizardWidget::~WizardWidget()
{
    delete ui;
}

void WizardWidget::pushStep(QWidget *widget, const QString &name)
{
    ui->gb_stepName->layout()->addWidget(widget);
    widget->hide();
    m_steps.emplace_back(std::make_shared<WizardStep>(widget, name));
    if (m_steps.size() == 1) {
        ui->bt_next->setEnabled(true);
        m_currentIndex = 0;
        widget->show();
        ui->gb_stepName->setTitle(QString().asprintf("第%d步: %s", m_currentIndex + 1, name.toStdString().c_str()));
        emit onNextStep();
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
        m_steps[m_currentIndex - 1]->widget->hide();
        m_steps[m_currentIndex]->widget->show();
        ui->gb_stepName->setTitle(QString().asprintf("第%d步: %s", m_currentIndex + 1, m_steps[m_currentIndex]->name.toStdString().c_str()));
        emit onNextStep();
    }
    if (m_currentIndex == static_cast<int>(m_steps.size()) - 1) {
        ui->bt_next->setEnabled(false);
    }
}
