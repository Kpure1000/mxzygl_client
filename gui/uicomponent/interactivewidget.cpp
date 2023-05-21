#include "interactivewidget.h"
#include "ui_interactivewidget.h"

#include "gui/uicomponent/renderwidget.h"
#include "function/renderer/renderer.h"

InteractiveWidget::InteractiveWidget(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::InteractiveWidget)
{
    ui->setupUi(this);
    auto renderW = ui->previewPane->getRenderWidget();
    connect(ui->previewPane->getRenderWidget(), &RenderWidget::onFrameUpdate, this, [=]() {
        auto trans_m = renderW->getRenderer()->getModelTransform();
        auto m_rot_eular = trans_m.rotation.toEulerAngles();
        ui->le_model_pos->setText(QString().asprintf( "%.2f , %.2f , %.2f", trans_m.position.x(), trans_m.position.y(), trans_m.position.z()));
        ui->le_model_rot->setText(QString().asprintf( "%.2f , %.2f , %.2f", m_rot_eular.x(),      m_rot_eular.y(),      m_rot_eular.z()));
        ui->le_model_sca->setText(QString().asprintf( "%.2f , %.2f , %.2f", trans_m.scale.x(),    trans_m.scale.y(),    trans_m.scale.z()));
        auto trans_c = renderW->getRenderer()->getCameraTransform();
        auto c_rot_eular = trans_c.rotation.toEulerAngles();
        ui->le_camera_pos->setText(QString().asprintf("%.2f , %.2f , %.2f", trans_c.position.x(), trans_c.position.y(), trans_c.position.z()));
        ui->le_camera_rot->setText(QString().asprintf("%.2f , %.2f , %.2f", c_rot_eular.x(),      c_rot_eular.y(),      c_rot_eular.z()));
    });
    connect(ui->bt_reset, &QPushButton::clicked, this, [=]() {
        renderW->getRenderer()->resetAllTransform();
    });
    connect(ui->bt_save, &QPushButton::clicked, this, &InteractiveWidget::onSaveTransform);
}

InteractiveWidget::~InteractiveWidget()
{
    delete ui;
}

PreviewPane *InteractiveWidget::getPreviewPane() const
{
    return ui->previewPane;
}
