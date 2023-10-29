#include "optionwidget_asset.h"
#include "ui_optionwidget_asset.h"

#include "function/configer/configmanager.h"

#include <QDebug>

OptionWidget_Asset::OptionWidget_Asset(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OptionWidget_Asset)
{
    ui->setupUi(this);

    m_single_size_limit = ConfigManager::getInstance()->getConfig("Asset/SingleSizeLimit", 100.f).toDouble();
    m_total_size_limit = ConfigManager::getInstance()->getConfig("Asset/TotalSizeLimit", 1024.f).toDouble();

    m_cache_timeout = ConfigManager::getInstance()->getConfig("Asset/CacheTimeout", 1.f).toDouble();
    m_cache_timeout_retry = ConfigManager::getInstance()->getConfig("Asset/CacheTimeoutRetry", .5f).toDouble();

    ui->spinb_timeout->setValue(m_cache_timeout);
    ui->spinb_retry->setValue(m_cache_timeout_retry);

    ui->bt_save->setDisabled(true);

    connect(ConfigManager::getInstance(), &ConfigManager::onConfModified, this, [this](){
        auto _single_size_limit = ConfigManager::getInstance()->getConfig("Asset/SingleSizeLimit").toDouble();
        auto _total_size_limit= ConfigManager::getInstance()->getConfig("Asset/TotalSizeLimit").toDouble();
        ui->spinb_singleLim->setValue(_single_size_limit);
        ui->spinb_totalLim->setValue(_total_size_limit);
        m_single_size_limit = ui->spinb_singleLim->value();
        m_total_size_limit = ui->spinb_totalLim->value();

        auto _cache_timeout = ConfigManager::getInstance()->getConfig("Asset/CacheTimeout").toDouble();
        auto _cache_timeout_retry = ConfigManager::getInstance()->getConfig("Asset/CacheTimeoutRetry").toDouble();
        ui->spinb_timeout->setValue(_cache_timeout);
        ui->spinb_retry->setValue(_cache_timeout_retry);
        m_single_size_limit = ui->spinb_singleLim->value();
        m_total_size_limit = ui->spinb_totalLim->value();
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

    connect(ui->spinb_singleLim, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=](double val) {
        m_single_size_limit = val;
        ui->bt_save->setDisabled(false);
    });
    connect(ui->spinb_totalLim, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=](double val) {
        m_total_size_limit = val;
        ui->bt_save->setDisabled(false);
    });

    connect(ui->bt_save, &QPushButton::clicked, this, [=]() {
        ConfigManager::getInstance()->setConfigs(
            {
             {"Asset/SingleSizeLimit", m_single_size_limit},
             {"Asset/TotalSizeLimit", m_total_size_limit},
             {"Asset/CacheTimeout", m_cache_timeout},
             {"Asset/CacheTimeoutRetry", m_cache_timeout_retry},
            });
        ui->bt_save->setDisabled(true);
        qDebug() << "OptionWidget_Asset>>Asset Cnofig Saved";
    });
}

OptionWidget_Asset::~OptionWidget_Asset()
{
    delete ui;
}
