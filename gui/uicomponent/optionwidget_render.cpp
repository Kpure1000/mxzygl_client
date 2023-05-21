#include "optionwidget_render.h"
#include "ui_optionwidget_render.h"

#include <QColorDialog>
#include <QDebug>
#include <QVector3D>

#include "function/configer/configmanager.h"


OptionWidget_Render::OptionWidget_Render(QWidget *parent) : QWidget(parent), ui(new Ui::OptionWidget_Render)
{
    ui->setupUi(this);

    auto configM = ConfigManager::getInstance();

    // 清屏颜色
    {
        auto val = configM->getConfig("Render/ClearColor");
        if (!val.canConvert<QVector3D>()) {
            qDebug() << "OptionWidget_Render::OptionWidget_Render>> ClearColor " << val << "is not Vec3";
            val = QVector3D{.0f, .0f, .0f};
        }
        auto colorv3 = val.value<QVector3D>();
        m_clearColor = QColor::fromRgbF(colorv3.x(), colorv3.y(), colorv3.z());
        ui->bt_clearColor->setStyleSheet(QString().asprintf("background-color: #%04X", m_clearColor.rgb()));
        ui->le_clearColor->setText(QString().asprintf("#%04X", m_clearColor.rgb()));
    }
    // 天空盒颜色
    {
        auto val = configM->getConfig("Render/SkyColor");
        if (!val.canConvert<QVector3D>()) {
            qDebug() << "OptionWidget_Render::OptionWidget_Render>> SkyColor " << val << "is not Vec3";
            val = QVector3D{.0f, .0f, .0f};
        }
        auto colorv3 = val.value<QVector3D>();
        m_skyColor = QColor::fromRgbF(colorv3.x(), colorv3.y(), colorv3.z());
        ui->bt_skyColor->setStyleSheet(QString().asprintf("background-color: #%04X", m_skyColor.rgb()));
        ui->le_skyColor->setText(QString().asprintf("#%04X", m_skyColor.rgb()));

        val = configM->getConfig("Render/GroundColor");
        if (!val.canConvert<QVector3D>()) {
            qDebug() << "OptionWidget_Render::OptionWidget_Render>> GroundColor " << val << "is not Vec3";
            val = QVector3D{.0f, .0f, .0f};
        }
        colorv3 = val.value<QVector3D>();
        m_groundColor = QColor::fromRgbF(colorv3.x(), colorv3.y(), colorv3.z());
        ui->bt_groundColor->setStyleSheet(QString().asprintf("background-color: #%04X", m_groundColor.rgb()));
        ui->le_groundColor->setText(QString().asprintf("#%04X", m_groundColor.rgb()));
    }
    // Arcball
    {
        ui->spin_arc_moveSpeed  -> setValue(configM->getConfig("Render/ArcBall_moveSpeed",   ui->spin_arc_moveSpeed->value()  ).toDouble());
        ui->spin_arc_rotateSpeed-> setValue(configM->getConfig("Render/ArcBall_rotateSpeed", ui->spin_arc_rotateSpeed->value()).toDouble());
        ui->spin_arc_drag       -> setValue(configM->getConfig("Render/ArcBall_drag",        ui->spin_arc_drag->value()       ).toDouble());
    }
    // FPSCamera
    {
        ui->spin_fps_moveSpeed  -> setValue(configM->getConfig("Render/FPSCamera_moveSpeed",   ui->spin_fps_moveSpeed->value()  ).toDouble());
        ui->spin_fps_rotateSpeed-> setValue(configM->getConfig("Render/FPSCamera_rotateSpeed", ui->spin_fps_rotateSpeed->value()).toDouble());
    }
}

OptionWidget_Render::~OptionWidget_Render()
{
    delete ui;
}

QVector3D colorToVec3(const QColor &color)
{
    return QVector3D{static_cast<float>(color.redF()),
                     static_cast<float>(color.greenF()),
                     static_cast<float>(color.blueF())};
}

void OptionWidget_Render::on_bt_save_clicked()
{
    // 保存
    ConfigManager::getInstance()->setConfigs({
        {"Render/ClearColor",            colorToVec3 (   m_clearColor    ) },
        {"Render/SkyColor",              colorToVec3 (   m_skyColor      ) },
        {"Render/GroundColor",           colorToVec3 (   m_groundColor   ) },
        {"Render/ArcBall_moveSpeed",     ui->spin_arc_moveSpeed  ->value() },
        {"Render/ArcBall_rotateSpeed",   ui->spin_arc_rotateSpeed->value() },
        {"Render/ArcBall_drag",          ui->spin_arc_drag       ->value() },
        {"Render/FPSCamera_moveSpeed",   ui->spin_fps_moveSpeed  ->value() },
        {"Render/FPSCamera_rotateSpeed", ui->spin_fps_rotateSpeed->value() }
    });
}

void OptionWidget_Render::on_bt_clearColor_clicked()
{
    auto colorDialog = new QColorDialog(m_clearColor, this);
    colorDialog->setWindowTitle(tr("选择清屏颜色"));
    colorDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    connect(colorDialog, &QColorDialog::accepted, this, [colorDialog, this](){
        m_clearColor = colorDialog->currentColor();
        ui->bt_clearColor->setStyleSheet(QString().asprintf("background-color: #%04X", m_clearColor.rgb()));
        ui->le_clearColor->setText(QString().asprintf("#%04X", m_clearColor.rgb()));
    });
    colorDialog->exec();
}

void OptionWidget_Render::on_bt_skyColor_clicked()
{
    auto colorDialog = new QColorDialog(m_skyColor, this);
    colorDialog->setWindowTitle(tr("选择天空颜色"));
    colorDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    connect(colorDialog, &QColorDialog::accepted, this, [colorDialog, this](){
        m_skyColor = colorDialog->currentColor();
        ui->bt_skyColor->setStyleSheet(QString().asprintf("background-color: #%04X", m_skyColor.rgb()));
        ui->le_skyColor->setText(QString().asprintf("#%04X", m_skyColor.rgb()));
    });
    colorDialog->exec();
}

void OptionWidget_Render::on_bt_groundColor_clicked()
{
    auto colorDialog = new QColorDialog(m_groundColor, this);
    colorDialog->setWindowTitle(tr("选择地面颜色"));
    colorDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    connect(colorDialog, &QColorDialog::accepted, this, [colorDialog, this](){
        m_groundColor = colorDialog->currentColor();
        ui->bt_groundColor->setStyleSheet(QString().asprintf("background-color: #%04X", m_groundColor.rgb()));
        ui->le_groundColor->setText(QString().asprintf("#%04X", m_groundColor.rgb()));
    });
    colorDialog->exec();
}
