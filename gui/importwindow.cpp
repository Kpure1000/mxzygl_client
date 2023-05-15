#include "importwindow.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QSpacerItem>
#include <QFileInfo>
#include <QTabWidget>
#include <QDockWidget>
#include <QMessageBox>
#include <QCloseEvent>
#include <QShowEvent>

#include "uicomponent/previewwidget.h"
#include "function/configer/configmanager.h"
#include "gui/uicomponent/categoryselector.h"
#include "gui/uicomponent/loggingwidget.h"
#include "function/layoutmanager.h"

ImportWindow::ImportWindow(QWidget *parent)
    : IFunctionWindow("", {800, 600}, false, false, true, parent)
{
    auto center_widget = centralWidget();
    auto ly_total = new QVBoxLayout(center_widget);

    auto cate_sel = new CategorySelector(center_widget);
    auto docker_cate = new QDockWidget(tr("资源导入 - 大类选择器"), center_widget);
    docker_cate->setObjectName("docker_cate");
    docker_cate->setWidget(cate_sel);
    docker_cate->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    docker_cate->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::TopDockWidgetArea, docker_cate);

    auto logging_widget = new LoggingWidget(center_widget);
    auto docker_logging = new QDockWidget(tr("资源导入 - 信息输出"), center_widget);
    docker_logging->setObjectName("docker_logging");
    docker_logging->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    docker_logging->setWidget(logging_widget);
    docker_logging->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::RightDockWidgetArea, docker_logging);

    auto tabw = new QTabWidget(center_widget);
    ly_total->addWidget(tabw);

    auto modelImporter = new AssetImporter(AssetImporter::ImportType::MODEL, this);
    tabw->addTab(setupImportWidget(this, modelImporter), tr("模型导入"));

    auto bvhImporter = new AssetImporter(AssetImporter::ImportType::BVH, this);
    tabw->addTab(setupImportWidget(this, bvhImporter), tr("骨骼动画导入"));

    auto effectImporter = new AssetImporter(AssetImporter::ImportType::EFFECT, this);
    tabw->addTab(setupImportWidget(this, effectImporter), tr("特效资源导入"));

    connect(tabw, &QTabWidget::currentChanged, this, [this, tabw](int index){
        this->setWindowTitle(tabw->tabText(index));
    });

    connect(modelImporter, &AssetImporter::onResponsing, this, [=](const QString & info, bool is_continue){
        is_continue ? logging_widget->info(info) : logging_widget->warning(info);
    });
    connect(bvhImporter, &AssetImporter::onResponsing, this, [=](const QString & info, bool is_continue){
        is_continue ? logging_widget->info(info) : logging_widget->warning(info);
    });

    connect(effectImporter, &AssetImporter::onResponsing, this, [=](const QString & info, bool is_continue){
        is_continue ? logging_widget->info(info) : logging_widget->warning(info);
    });

    emit tabw->currentChanged(0);
}

QWidget *ImportWindow::setupImportWidget(QWidget* parent, AssetImporter *importer)
{
    auto totalWidget = new QWidget(parent);

    PreviewWidget *previewWidget = new PreviewWidget(importer->getInfoRef(),
                                                     2,
                                                     3,
                                                     static_cast<PreviewWidget::PreviewType>(importer->type()),
                                                     Qt::Orientation::Horizontal,
                                                     true,
                                                     totalWidget);

    connect(importer, &AssetImporter::onAddPaths, totalWidget, [previewWidget]() {
        previewWidget->refreshInfo();
    });
    connect(importer, &AssetImporter::onClear, totalWidget, [previewWidget]() {
        previewWidget->clearInfo();
    });

    auto ly_top = new QHBoxLayout();
    ly_top->setContentsMargins(12, 12, 12, 1);

    auto bt_fileBrowse = new QPushButton(tr("添加资源"), totalWidget);
    connect(bt_fileBrowse, &QPushButton::clicked, totalWidget, [=]() {
        QStringList selectedFiles;
        QString config_key, open_title, open_option;
        switch (importer->type()) {
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
        selectedFiles = QFileDialog::getOpenFileNames(totalWidget, open_title, open_dir, open_option);
        if (selectedFiles.size() > 0)
            ConfigManager::getInstance()->setConfig(config_key, QFileInfo(selectedFiles[0]).absolutePath());

        importer->addPathsNotExist(selectedFiles);
        previewWidget->selectGroup(0);
    });

    ly_top->addWidget(bt_fileBrowse, 1);

    auto bt_clear = new QPushButton(tr("清空"), totalWidget);
    connect(bt_clear, &QPushButton::clicked, totalWidget, [importer](){
        // 清空预览与表格
        importer->clear();
    });
    ly_top->addWidget(bt_clear, 1);

    auto bt_upload = new QPushButton(tr("上传"), totalWidget);
    connect(bt_upload, &QPushButton::clicked, totalWidget, [=]() {
        // 上传
        bt_upload->setEnabled(false);
        importer->upload();
    });
    connect(importer, &AssetImporter::onResponsing, parent, [=](const QString & info, bool is_continue){
        bt_upload->setEnabled(!is_continue);
    });

    ly_top->addWidget(bt_upload, 1);

    ly_top->addSpacerItem(new QSpacerItem(40, 20));
    ly_top->setStretch(ly_top->count() - 1, 4);

    auto ly_total = new QVBoxLayout(totalWidget);
    totalWidget->setLayout(ly_total);

    ly_total->addLayout(ly_top, 0);

    connect(previewWidget, &PreviewWidget::onPreview, totalWidget,
            [importer, previewWidget](const std::vector<int> &index) {
        previewWidget->previewFiles(importer->getFilePaths(index), importer->getFileNames(index));
    });

    ly_total->addWidget(previewWidget, 1);

    ly_total->setSpacing(1);

    return totalWidget;
}

void ImportWindow::closeEvent(QCloseEvent *event)
{
    LayoutManager::getInstance()->save(this, "ImportWindow");
    QWidget::closeEvent(event);
}

void ImportWindow::showEvent(QShowEvent *event)
{
    if (_is_first_paint) {
        _is_first_paint = false;
        LayoutManager::getInstance()->restore(this, "ImportWindow");
    }
    repaint();
}
