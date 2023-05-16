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
#include <QMenuBar>
#include <QMenu>

#include "uicomponent/previewwidget.h"
#include "function/configer/configmanager.h"
#include "gui/uicomponent/categoryselector.h"
#include "gui/uicomponent/loggingwidget.h"
#include "function/layoutmanager.h"

ImportWindow::ImportWindow(QWidget *parent)
    : IFunctionWindow("", {800, 600}, false, false, true, parent)
{
    auto center_widget = centralWidget();

    // ----------------视图----------------
    {
        auto top_menubar = new QMenuBar(this);
        auto menu_view = new QMenu(tr("视图(&V)"), this);

        // ----------------保存布局----------------
        menu_view->addAction(tr("保存布局"), this, [=]() {
            LayoutManager::getInstance()->save(this, "IndexWindow");
        });
        // ----------------加载布局----------------
        menu_view->addAction(tr("加载布局"), this, [=]() {
            LayoutManager::getInstance()->restore(this, "IndexWindow");
        });

        top_menubar->addMenu(menu_view);
        this->setMenuBar(top_menubar);
    }

    auto ly_total = new QVBoxLayout(center_widget);

    auto cate_sel = new CategorySelector(center_widget);
    auto docker_cate = new QDockWidget(tr("资源导入 - 大类选择器"), center_widget);
    docker_cate->setObjectName("docker_cate");
    docker_cate->setWidget(cate_sel);
    docker_cate->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    docker_cate->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::TopDockWidgetArea, docker_cate);

    m_logging_widget = new LoggingWidget(center_widget);
    auto docker_logging = new QDockWidget(tr("资源导入 - 信息输出"), center_widget);
    docker_logging->setObjectName("docker_logging");
    docker_logging->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    docker_logging->setWidget(m_logging_widget);
    docker_logging->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::RightDockWidgetArea, docker_logging);

    auto tabw = new QTabWidget(center_widget);
    ly_total->addWidget(tabw);

    auto modelImporter = new AssetImporter(AssetImporter::ImportType::MODEL, this);
    tabw->addTab(setupImportWidget(modelImporter), tr("模型导入"));

    auto bvhImporter = new AssetImporter(AssetImporter::ImportType::BVH, this);
    tabw->addTab(setupImportWidget(bvhImporter), tr("骨骼动画导入"));

    auto effectImporter = new AssetImporter(AssetImporter::ImportType::EFFECT, this);
    tabw->addTab(setupImportWidget(effectImporter), tr("特效资源导入"));

    connect(tabw, &QTabWidget::currentChanged, this, [this, tabw](int index){
        this->setWindowTitle(tabw->tabText(index));
    });

    emit tabw->currentChanged(0);
}

QWidget *ImportWindow::setupImportWidget(AssetImporter *importer)
{

    QPushButton *bt_pull, *bt_fileBrowse, *bt_clear, *bt_upload;

    auto totalWidget = new QWidget(this);

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

    bt_pull = new QPushButton(tr("拉取类型和标签信息"), totalWidget);
    connect(bt_pull, &QPushButton::clicked, totalWidget, [=](){
        importer->pullTypeAndTags();
    });
    ly_top->addWidget(bt_pull, 1);

    bt_fileBrowse = new QPushButton(tr("添加资源"), totalWidget);
    bt_fileBrowse->setEnabled(false);
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

    bt_clear = new QPushButton(tr("清空"), totalWidget);
    bt_clear->setEnabled(false);
    connect(bt_clear, &QPushButton::clicked, totalWidget, [importer](){
        // 清空预览与表格
        importer->clear();
    });
    ly_top->addWidget(bt_clear, 1);

    bt_upload = new QPushButton(tr("上传"), totalWidget);
    bt_upload->setEnabled(false);
    connect(bt_upload, &QPushButton::clicked, totalWidget, [=]() {
        // 上传
        bt_upload->setEnabled(false);
        importer->upload();
    });
    connect(importer, &AssetImporter::onResponsing, this, [=](const QString & info, bool is_continue){
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

    connect(importer, &AssetImporter::onImportSuccessful, this, [=](){
        bt_fileBrowse->setEnabled(false);
        bt_clear->setEnabled(false);
        bt_upload->setEnabled(false);
//        bt_pull->setEnabled(true);
    });

    connect(importer, &AssetImporter::onResponsing, this, [=](const QString & info, bool is_continue){
        is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
    });
    connect(importer, &AssetImporter::onTypeAndTagsLoaded, this, [=](){
        bt_fileBrowse->setEnabled(true);
        bt_clear->setEnabled(true);
        bt_upload->setEnabled(true);
//        bt_pull->setEnabled(false);
    });

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
