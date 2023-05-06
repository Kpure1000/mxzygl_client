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
        return;
    }

    auto colorv3 = val.value<QVector3D>();

    m_clearColor = QColor::fromRgbF(colorv3.x(), colorv3.y(), colorv3.z());

    ui->bt_color->setStyleSheet(QString().sprintf("background-color: #%04X", m_clearColor.rgb()));
}

OptionWidget_Render::~OptionWidget_Render()
{
    delete ui;
}

void OptionWidget_Render::on_bt_color_clicked()
{
    auto val = ConfigManager::getInstance()->getConfig("Render/ClearColor");

    if (!val.canConvert<QVector3D>()) {
        qDebug() << "OptionWidget_Render::on_bt_color_clicked>> ClearColor " << val << "is not Vec3";
        return;
    }

    auto colorv3 = val.value<QVector3D>();

    m_clearColor = QColorDialog::getColor(QColor::fromRgbF(colorv3.x(), colorv3.y(), colorv3.z()), nullptr, tr("选取颜色"));

    ui->bt_color->setStyleSheet(QString().sprintf("background-color: #%04X", m_clearColor.rgb()));
}


void OptionWidget_Render::on_bt_save_clicked()
{
    // 保存ClearColor
    ConfigManager::getInstance()->setConfig("Render/ClearColor",
                                            QVector3D{static_cast<float>(m_clearColor.redF()),
                                                      static_cast<float>(m_clearColor.greenF()),
                                                      static_cast<float>(m_clearColor.blueF())});
}

