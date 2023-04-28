#include "optionwidget_asset.h"
#include "ui_optionwidget_asset.h"

#include "function/configer/configmanager.h"

OptionWidget_Asset::OptionWidget_Asset(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OptionWidget_Asset)
{
    ui->setupUi(this);

    m_cache_timeout = ConfigManager::getInstance()->getConfig("Asset/CacheTimeout").toDouble();
    m_cache_timeout_retry = ConfigManager::getInstance()->getConfig("Asset/CacheTimeoutRetry").toDouble();

    ui->spinb_timeout->setValue(m_cache_timeout);
    ui->spinb_retry->setValue(m_cache_timeout_retry);

    ui->bt_save->setDisabled(true);

    connect(ConfigManager::getInstance(), &ConfigManager::onConfModified, this, [this](){

        m_cache_timeout = ConfigManager::getInstance()->getConfig("Asset/CacheTimeout").toDouble();
        m_cache_timeout_retry = ConfigManager::getInstance()->getConfig("Asset/CacheTimeoutRetry").toDouble();

        ui->spinb_timeout->setValue(m_cache_timeout);
        ui->spinb_retry->setValue(m_cache_timeout_retry);
    });

    connect(ui->spinb_timeout,
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this,
            [this](double val) {
                m_cache_timeout = val;
                ui->bt_save->setDisabled(false);
            });

    connect(ui->spinb_retry,
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this,
            [this](double val) {
                m_cache_timeout_retry = val;
                ui->bt_save->setDisabled(false);
            });

    connect(ui->bt_save, &QPushButton::clicked, this, [=]() {
        ConfigManager::getInstance()->setConfigs(
            {{"Asset/CacheTimeout", m_cache_timeout},
             {"Asset/CacheTimeoutRetry", m_cache_timeout_retry}});
        ui->bt_save->setDisabled(true);
        qDebug() << "OptionWidget_Asset>>Asset Cnofig Saved";
    });
}

OptionWidget_Asset::~OptionWidget_Asset()
{
    delete ui;
}
