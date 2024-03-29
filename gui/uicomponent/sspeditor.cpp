#include "sspeditor.h"
#include "ui_sspeditor.h"

#include "previewwidget.h"
#include "previewpane.h"
#include "renderwidget.h"
#include "function/renderer/renderer.h"

SSPEditor::SSPEditor(PreviewWidget *prewviewer, QWidget *parent)
    : QWidget(parent) , ui(new Ui::SSPEditor), m_prewviewer{prewviewer}
{
    ui->setupUi(this);

    ui->groupBox->setEnabled(false);
    ui->bt_confirm->setEnabled(false);

    connect(m_prewviewer, &PreviewWidget::onRefresh, this, [=](){
        ui->groupBox->setEnabled(false);
        ui->bt_confirm->setEnabled(false);
    });

    connect(m_prewviewer->getPreviewPane()[0]->getRenderWidget()->getRenderer(),
            &Renderer::onRenderDataSet,
            this,
            [=]() {
                ui->groupBox->setEnabled(true);
                ui->bt_confirm->setEnabled(true);
                this->fresh();
            });

    float spp = m_prewviewer->getPreviewPane()[0]->getRenderWidget()->getRenderer()->getBVHSampleFreq();
    int frames = m_prewviewer->getPreviewPane()[0]->getRenderWidget()->getRenderer()->getBVHFrames();

    ui->spb_spp->setValue(static_cast<int>(spp));
    ui->lb_freq->setText(QString().asprintf("%.3f (fps)", 1.0f / spp));
    ui->lb_frames->setText(QString().asprintf("%d", frames));
    ui->lb_totalTime->setText(QString().asprintf("%.3f (ms)", frames * 1000.0f / spp));

    connect(ui->spb_spp, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int val){
        int frames = m_prewviewer->getPreviewPane()[0]->getRenderWidget()->getRenderer()->getBVHFrames();
        ui->lb_freq->setText(QString().asprintf("%.3f (fps)", 1.0f / val));
        ui->lb_frames->setText(QString().asprintf("%d", frames));
        ui->lb_totalTime->setText(QString().asprintf("%.3f (ms)", frames * 1000.0f / val));
    });

    connect(ui->bt_confirm, &QPushButton::clicked, this, [=](){
        m_prewviewer->getPreviewPane()[0]->getRenderWidget()->getRenderer()->setBVHSampleFreq(ui->spb_spp->value());
    });

}

SSPEditor::~SSPEditor()
{
    delete ui;
}

void SSPEditor::fresh()
{
    float spp = m_prewviewer->getPreviewPane()[0]->getRenderWidget()->getRenderer()->getBVHSampleFreq();
    int frames = m_prewviewer->getPreviewPane()[0]->getRenderWidget()->getRenderer()->getBVHFrames();

    ui->spb_spp->setValue(static_cast<int>(spp));
    ui->lb_freq->setText(QString().asprintf("%.3f (fps)", 1.0f / spp));
    ui->lb_frames->setText(QString().asprintf("%d", frames));
    ui->lb_totalTime->setText(QString().asprintf("%.3f (ms)", frames * 1000.0f / spp));
}
