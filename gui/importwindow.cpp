#include "importwindow.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QSpacerItem>
#include <QFileInfo>

#include "uicomponent/previewwidget.h"
#include "function/configer/configer.h"

ImportWindow::ImportWindow(const QString &title, AssetImporter::ImportType type, QWidget *parent)
    : IFunctionWindow(title, parent ? parent->size() : QSize{800, 600}, false, parent)
{
    m_assetImporter = new AssetImporter(type, this);
    connect(m_assetImporter, &AssetImporter::onAddPaths, this, [this]() {
        m_previewWidget->refreshInfo();
    });
    connect(m_assetImporter, &AssetImporter::onClear, this, [this]() {
        m_previewWidget->clearInfo();
    });

    auto ly_top = new QHBoxLayout();
    ly_top->setSpacing(2);

    auto bt_fileBrowse = new QPushButton(tr("添加资源"), this);
    connect(bt_fileBrowse, &QPushButton::clicked, this, [this]() {
        QStringList selectedFiles;
        QString config_key, open_title, open_option;
        switch (m_assetImporter->type()) {
        case AssetImporter::ImportType::MODEL: {
            config_key = "FileBrowser/ImportType_MODEL";
            open_title = tr("浏览模型文件");
            open_option = "*.obj;*.fbx";
            break;
        }
        case AssetImporter::ImportType::BVH: {
            config_key = "FileBrowser/ImportType_BVH";
            open_title = tr("浏览骨骼动画文件");
            open_option = "*.bvh";
            break;
        }
        case AssetImporter::ImportType::EFFECT: {
            config_key = "FileBrowser/ImportType_EFFECT";
            open_title = tr("浏览特效文件");
            open_option = "*.obj;*.fbx;*.bvh";
            break;
        }
        }
        QString open_dir = ConfigManager::getInstance()->getConfig(config_key).toString();
        selectedFiles = QFileDialog::getOpenFileNames(this, open_title, open_dir, open_option);
        if (selectedFiles.size() > 0)
            ConfigManager::getInstance()->setConfig(config_key, QFileInfo(selectedFiles[0]).absolutePath());

        m_assetImporter->addPathsNotExist(selectedFiles);
    });

    ly_top->addWidget(bt_fileBrowse, 1);

    auto bt_clear = new QPushButton(tr("清空"), this);
    connect(bt_clear, &QPushButton::clicked, this, [this](){
        // 清空预览与表格
        m_assetImporter->clear();
    });
    ly_top->addWidget(bt_clear, 1);

    auto bt_upload = new QPushButton(tr("上传"), this);
    connect(bt_upload, &QPushButton::clicked, this, [this]() {
        // TODO 连接服务器 上传
    });
    ly_top->addWidget(bt_upload, 1);

    ly_top->addSpacerItem(new QSpacerItem(40, 20));
    ly_top->setStretch(ly_top->count() - 1, 4);

    auto ly_total = new QVBoxLayout(this);

    ly_total->addLayout(ly_top, 0);

    m_previewWidget = new PreviewWidget(m_assetImporter->getInfoRef(),
                                        2,
                                        3,
                                        static_cast<PreviewWidget::PreviewType>(type),
                                        Qt::Orientation::Horizontal,
                                        true,
                                        this);

    connect(m_previewWidget, &PreviewWidget::onPreview, this, [this](const std::vector<int> &index){
        m_previewWidget->previewFiles(m_assetImporter->getFilePaths(index), m_assetImporter->getFileNames(index));
    });

    ly_total->addWidget(m_previewWidget, 1);

    ly_total->setSpacing(1);

    this->setLayout(ly_total);
}
