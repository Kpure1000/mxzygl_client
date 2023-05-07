#include "optionwidget_render.h"
#include "ui_optionwidget_render.h"

#include <QColorDialog>
#include <QDebug>
#include <QVector3D>

#include "function/configer/configmanager.h"


OptionWidget_Render::OptionWidget_Render(QWidget *parent) : QWidget(parent), ui(new Ui::OptionWidget_Render)
{
    ui->setupUi(this);

    auto val = ConfigManager::getInstance()->getConfig("Render/ClearColor");

    if (!val.canConvert<QVector3D>()) {
        qDebug() << "OptionWidget_Render::on_bt_color_clicked>> ClearColor " << val << "is not Vec3";
        val = QVector3D{.0f, .0f, .0f};
    }

    auto colorv3 = val.value<QVector3D>();

    m_clearColor = QColor::fromRgbF(colorv3.x(), colorv3.y(), colorv3.z());

    ui->bt_color->setStyleSheet(QString().asprintf("background-color: #%04X", m_clearColor.rgb()));
}

OptionWidget_Render::~OptionWidget_Render()
{
    delete ui;
}

void OptionWidget_Render::on_bt_color_clicked()
{
    auto colorDialog = new QColorDialog(m_clearColor, this);
    colorDialog->setWindowTitle(tr("选择清屏颜色"));
    colorDialog->setAttribute(Qt::WA_DeleteOnClose, true);

    connect(colorDialog, &QColorDialog::accepted, this, [colorDialog, this](){
        m_clearColor = colorDialog->currentColor();
        ui->bt_color->setStyleSheet(QString().asprintf("background-color: #%04X", m_clearColor.rgb()));
    });
    colorDialog->exec();
}


void OptionWidget_Render::on_bt_save_clicked()
{
    // 保存ClearColor
    ConfigManager::getInstance()->setConfig("Render/ClearColor",
                                            QVector3D{static_cast<float>(m_clearColor.redF()),
                                                      static_cast<float>(m_clearColor.greenF()),
                                                      static_cast<float>(m_clearColor.blueF())});
}

