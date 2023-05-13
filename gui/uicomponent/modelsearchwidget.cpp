#include "modelsearchwidget.h"
#include "ui_modelsearchwidget.h"

#include <QFileDialog>
#include <QSplitter>
#include <QDebug>
#include <QMessageBox>

#include "gui/uicomponent/previewwidget.h"
#include "function/configer/configmanager.h"

ModelSearchWidget::ModelSearchWidget(ModelSearch::SearchType searchType, QWidget *parent) :
    QWidget(parent), ui(new Ui::ModelSearchWidget), m_single_preview(nullptr)
{
    ui->setupUi(this);

    m_modelSearch = new ModelSearch(searchType, this);
    connect(m_modelSearch, &ModelSearch::onResultUpdate, this, [=](){
        m_preview->clearInfo();
        m_preview->refreshInfo();
        m_preview->selectGroup(0);
    });
    connect(m_modelSearch, &ModelSearch::onResultClear, this, [=](){
        m_preview->clearInfo();
    });
    connect(m_modelSearch, &ModelSearch::onResponsing, this, [=](const QString&, bool is_continue){
        ui->bt_search->setEnabled(!is_continue);
    });

    m_preview = new PreviewWidget(m_modelSearch->getResultsModelInfo(),
                                  3,
                                  3,
                                  PreviewWidget::PreviewType::MODEL,
                                  Qt::Orientation::Horizontal,
                                  false,
                                  this);

    connect(m_preview, &PreviewWidget::onPreview, this, [this](const std::vector<int> &index) {
        m_preview->previewFiles(m_modelSearch->getFilePaths(index), m_modelSearch->getPreviewInfo(index));
    });

    if (searchType == ModelSearch::SearchType::CONTENT) {
        // 显示浏览按钮
        ui->bt_browser->setHidden(false);
        // 添加单个模型预览界面
        m_single_preview = new PreviewWidget(m_modelSearch->getSearchModelInfo(),
                                             1,
                                             1,
                                             PreviewWidget::PreviewType::MODEL,
                                             Qt::Orientation::Vertical,
                                             false,
                                             this);
        auto split_preview = new QSplitter(this);
        split_preview->setOrientation(Qt::Orientation::Horizontal);
        split_preview->addWidget(m_single_preview);
        split_preview->setStretchFactor(0, 1);
        split_preview->addWidget(m_preview);
        split_preview->setStretchFactor(1, 2);
        ui->ly_preview->addWidget(split_preview);
        ui->le_input->setEnabled(false);

        connect(m_single_preview, &PreviewWidget::onPreview, this, [this](const std::vector<int> &index) {
            if (m_selectedFile != "" && index.size() > 0) {
                m_single_preview->previewFiles({m_selectedFile}, {tr("模型预览")});
            }
        });
    } else {
        ui->bt_browser->setHidden(true);
        ui->ly_preview->addWidget(m_preview);
        ui->le_input->setEnabled(true);
    }

//    emit m_preview->onPreview({0});

}

ModelSearchWidget::~ModelSearchWidget()
{
    delete ui;
}

void ModelSearchWidget::on_le_input_textChanged(const QString &input)
{
    if (m_modelSearch->getType() != ModelSearch::SearchType::CONTENT) {
        m_modelSearch->setSearchInfo(input);
    }
}

void ModelSearchWidget::on_bt_browser_clicked()
{
    QString open_dir = ConfigManager::getInstance()->getConfig("FileBrowser/ModelSearch").toString();
    m_selectedFile = QFileDialog::getOpenFileName(this, tr("浏览模型文件"), open_dir, "*.obj;*.fbx");
    if (m_selectedFile == "")
        return;
    if (nullptr == m_single_preview) {
        qDebug() << "ModelSearchWidget::on_bt_browser_clicked>> m_single_preview is null";
        return;
    }
    ui->le_input->setText(m_selectedFile);
    ConfigManager::getInstance()->setConfig("FileBrowser/ModelSearch", QFileInfo(m_selectedFile).absolutePath());
    // 设置搜索信息
    m_modelSearch->setSearchInfo(m_selectedFile);
    m_single_preview->clearInfo();
    m_single_preview->refreshInfo();
    // 预览该模型
    m_single_preview->selectGroup(0);
}


void ModelSearchWidget::on_bt_search_clicked()
{
    ui->bt_search->setEnabled(false);
    m_modelSearch->clearResults();
    if (m_modelSearch->getType() != ModelSearch::SearchType::CONTENT) {
        m_modelSearch->setSearchInfo(ui->le_input->text());
    }
    m_modelSearch->search();
}

