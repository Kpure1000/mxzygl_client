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
#include <QSplitter>
#include <QProgressDialog>

#include "uicomponent/previewwidget.h"
#include "function/configer/configmanager.h"
#include "gui/uicomponent/categoryselector.h"
#include "gui/uicomponent/loggingwidget.h"
#include "function/layoutmanager.h"
#include "gui/uicomponent/wizardwidget.h"
#include "gui/uicomponent/infotablewidget.h"
#include "gui/uicomponent/interactivewidget.h"
#include "gui/uicomponent/previewpane.h"
#include "gui/uicomponent/renderwidget.h"
#include "function/renderer/renderer.h"

ImportWindow::ImportWindow(QWidget *parent)
    : IFunctionWindow("", {800, 600}, true, false, true, parent)
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
    tabw->addTab(setupModelWizard(modelImporter), tr("模型导入"));
//    tabw->addTab(setupBrowseWidget(modelImporter), tr("模型导入"));

    auto bvhImporter = new AssetImporter(AssetImporter::ImportType::BVH, this);
    tabw->addTab(setupBVHWizard(bvhImporter), tr("骨骼动画导入"));
//    tabw->addTab(setupBrowseWidget(bvhImporter), tr("骨骼动画导入"));

    auto effectImporter = new AssetImporter(AssetImporter::ImportType::EFFECT, this);
    tabw->addTab(setupEffectWizard(effectImporter), tr("特效资源导入"));
//    tabw->addTab(setupBrowseWidget(effectImporter), tr("特效资源导入"));

    connect(tabw, &QTabWidget::currentChanged, this, [this, tabw](int index){
        this->setWindowTitle(tabw->tabText(index));
    });

    emit tabw->currentChanged(0);
}

WizardWidget *ImportWindow::setupModelWizard(AssetImporter *importer)
{
    auto ww = new WizardWidget(this);
    // 1. 添加模型文件
    auto firstStep = setupBrowseWidget(importer, ww);
    ww->pushStep(firstStep, "添加模型文件");
    // 2. 对齐原点, + 覆盖原模型文件
    auto secondStep = setupModel_UniformFormat(importer, ww);
    ww->pushStep(secondStep, tr("对齐原点 + 覆盖原模型文件"));
    // 3. 三维模型变换 + 摄像机位置调整
    auto thirdStep = setupModel_TransformCamera(importer, ww);
    ww->pushStep(thirdStep, tr("三维模型变换 + 摄像机位置调整"));
    // 4. 编辑类型和标签属性 + 缩略图创建 + 资源上传
    auto fourthStep = setupModel_ThumbUpload(importer, ww);
    ww->pushStep(fourthStep, tr("编辑类型和标签属性 + 缩略图创建 + 资源上传"));

    connect(importer, &AssetImporter::onResponsing, this, [=](const QString & info, bool is_continue){
        is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
    });

    return ww;
}

WizardWidget *ImportWindow::setupBVHWizard(AssetImporter *importer)
{
    auto ww = new WizardWidget(this);
    // 1. 浏览文件 + 编辑类型和标签属性
    auto firstStep = setupBrowseWidget(importer, ww);
    ww->pushStep(firstStep, tr("浏览文件 + 编辑类型和标签属性"));
    // 2. 对齐原点 + 覆盖原模型文件
    // 3. 三维模型变换 + 摄像机位置调整
    // 4. 缩略图创建 + 资源上传

    connect(importer, &AssetImporter::onResponsing, this, [=](const QString & info, bool is_continue){
        is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
    });

    return ww;
}

WizardWidget *ImportWindow::setupEffectWizard(AssetImporter *importer)
{
    auto ww = new WizardWidget(this);
    // 1. 浏览文件 + 编辑类型和标签属性
    auto firstStep = setupBrowseWidget(importer, ww);
    ww->pushStep(firstStep, "浏览文件 + 编辑类型和标签属性");
    // 2. 对齐原点, + 覆盖原模型文件
    ww->pushStep(new QPushButton("asd", this), "asd");
    // 3. 三维模型变换 + 摄像机位置调整
    // 4. 缩略图创建 + 资源上传

    connect(importer, &AssetImporter::onResponsing, this, [=](const QString & info, bool is_continue){
        is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
    });

    return ww;
}

QWidget *ImportWindow::setupBrowseWidget(AssetImporter *importer, WizardWidget* wizard)
{

    QPushButton *bt_fileBrowse, *bt_clear;

    auto totalWidget = new QWidget(this);

    connect(wizard, &WizardWidget::onSwitchStep, this, [=]() {
        // 当进行到当前步骤时
        if (totalWidget == wizard->currentStep()->widget) {
            wizard->setNextButtonEnable(false);
        }
    });

    PreviewWidget *previewWidget = new PreviewWidget(importer->getInfoRef(),
                                                     2,
                                                     3,
                                                     static_cast<PreviewWidget::PreviewType>(importer->type()),
                                                     Qt::Orientation::Horizontal,
                                                     true,
                                                     totalWidget);

    connect(importer, &AssetImporter::onAddPaths, totalWidget, [=]() {
        wizard->setNextButtonEnable(true);
        previewWidget->refreshInfo();
    });

    connect(importer, &AssetImporter::onClear, totalWidget, [=]() {
        wizard->setNextButtonEnable(false);
        previewWidget->clearInfo();
    });

    auto ly_top = new QHBoxLayout();
    ly_top->setContentsMargins(12, 12, 12, 1);

    bt_fileBrowse = new QPushButton(tr("添加资源"), totalWidget);
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
    connect(bt_clear, &QPushButton::clicked, totalWidget, [importer]() {
        // 清空预览与表格
        importer->clear();
    });
    ly_top->addWidget(bt_clear, 1);

    ly_top->addSpacerItem(new QSpacerItem(40, 20));
    ly_top->setStretch(ly_top->count() - 1, 4);

    auto ly_total = new QVBoxLayout(totalWidget);
    totalWidget->setLayout(ly_total);

    ly_total->addLayout(ly_top, 0);

    connect(previewWidget, &PreviewWidget::onPreview, totalWidget, [importer, previewWidget](const std::vector<int> &index) {
        previewWidget->previewFiles(importer->getFilePaths(index), importer->getFileNames(index), false);
    });

    ly_total->addWidget(previewWidget, 1);

    ly_total->setSpacing(1);

    return totalWidget;
}

QWidget *ImportWindow::setupModel_UniformFormat(AssetImporter *importer, WizardWidget* wizard)
{
    auto totalWidget = new QWidget(this);
    auto ly_total = new QVBoxLayout(totalWidget);
    auto bt_align = new QPushButton(tr("对齐原点并转换格式"), this);
    ly_total->addWidget(bt_align);

    PreviewWidget *previewWidget = new PreviewWidget(importer->getInfoRef(),
                                                     2,
                                                     3,
                                                     static_cast<PreviewWidget::PreviewType>(importer->type()),
                                                     Qt::Orientation::Horizontal,
                                                     true,
                                                     totalWidget);
    ly_total->addWidget(previewWidget);

    connect(wizard, &WizardWidget::onSwitchStep, this, [=]() {
        // 当进行到当前步骤时
        if (totalWidget == wizard->currentStep()->widget) {
            previewWidget->refreshInfo();
            previewWidget->selectGroup(0);
        }
    });

    connect(previewWidget, &PreviewWidget::onPreview, totalWidget, [importer, previewWidget](const std::vector<int> &index) {
        previewWidget->previewFiles(importer->getFilePaths(index), importer->getFileNames(index), false);
    });

    connect(bt_align, &QPushButton::clicked, this, [=]() {
        int res = QMessageBox::warning(this, tr("操作警告"), tr("转换格式可能覆盖原模型文件, 且操作不可撤销!"), QMessageBox::StandardButton::Cancel, QMessageBox::StandardButton::Ok);
        if (res != QMessageBox::StandardButton::Ok)
            return;
        bt_align->setEnabled(false);
        importer->alignToOrigin();
    });

    auto progressDialog = new QProgressDialog(tr(""), tr("隐藏进度显示, 转换不会终止"), 0, 1, this);
    progressDialog->reset();
    auto prog_bt_cancel = new QPushButton(tr("完成"), progressDialog);
    prog_bt_cancel->setEnabled(false);
    progressDialog->setCancelButton(prog_bt_cancel);
    progressDialog->setWindowFlag(Qt::WindowCloseButtonHint, false);
    progressDialog->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    progressDialog->setAutoClose(false);

    connect(importer, &AssetImporter::onStartAlignToOrigin, this, [=](int size) {
        progressDialog->setLabelText(QString().asprintf("已转换 0 个, 剩余 %d 个, 请勿关闭导入程序", size));
        progressDialog->setMaximum(size);
        progressDialog->open();
    });
    connect(importer, &AssetImporter::onDoneAlignToOrigin, this, [=](int progress) {
        progressDialog->setLabelText(QString().asprintf("已转换 %d 个, 剩余 %d 个, 请勿关闭导入程序", progress, progressDialog->maximum() - progress));
        progressDialog->setValue(progress);
    });
    connect(importer, &AssetImporter::saveSuccessful, this, [=]() {
        progressDialog->setLabelText(QString().asprintf("已转换 %d 个, 剩余 0 个.", progressDialog->maximum()));
        prog_bt_cancel->setEnabled(true);
        progressDialog->reset();

        m_logging_widget->trace("保存成功");

        previewWidget->refreshInfo();
    });
    return totalWidget;
}

QWidget *ImportWindow::setupModel_TransformCamera(AssetImporter *importer, WizardWidget* wizard)
{
    auto totalWidget = new QWidget(this);
    auto ly_total = new QHBoxLayout(totalWidget);

    auto infotable = new InfoTableWidget(importer->getInfoRef(), 1, false, totalWidget);

    auto splitter = new QSplitter(Qt::Horizontal, totalWidget);
    ly_total->addWidget(splitter);

//    ly_total->addWidget(infotable, 1);
    splitter->addWidget(infotable);

    auto interact = new InteractiveWidget(totalWidget);
//    ly_total->addWidget(interact, 2);
    splitter->addWidget(interact);

    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);

    connect(wizard, &WizardWidget::onSwitchStep, this, [=]() {
        // 当进行到当前步骤时
        if (totalWidget == wizard->currentStep()->widget) {
            infotable->refresh();
            auto renderer = interact->getPreviewPane()->getRenderWidget()->getRenderer();
            importer->initAllTransform(renderer->getModelTransform(), renderer->getCameraTransform());
            infotable->selectGroup(0);
        }
    });

    connect(infotable, &InfoTableWidget::onGroupSelected, this, [=](const std::vector<int> &rows) {
        auto previewPane = interact->getPreviewPane();
        previewPane->doClear();
        disconnect(previewPane, &PreviewPane::onSelectedPane, this, 0);
        if (!rows.empty()) {
            m_transform_row_selected = rows[0];
            connect(previewPane, &PreviewPane::onSelectedPane, this, [=]() {
                infotable->jumpTo(m_transform_row_selected);
            });
            QString filePaht = importer->getFilePaths()[m_transform_row_selected];
            QString fileInfo = importer->getFileNames({m_transform_row_selected})[0];

            previewPane->doPreviewModel(filePaht, fileInfo, true);

            auto renderer = interact->getPreviewPane()->getRenderWidget()->getRenderer();
            auto trans_pair = importer->getTransform(m_transform_row_selected);
            renderer->setModelTransform(trans_pair.first);
            renderer->setCameraTransform(trans_pair.second);
        }
    });
    connect(interact, &InteractiveWidget::onSaveTransform, this, [=](){
        auto renderer = interact->getPreviewPane()->getRenderWidget()->getRenderer();
        if (m_transform_row_selected >= 0) {
            importer->saveTransform(m_transform_row_selected, renderer->getModelTransform(), renderer->getCameraTransform());
        }
    });

    return totalWidget;
}

QWidget *ImportWindow::setupModel_ThumbUpload(AssetImporter *importer, WizardWidget* wizard)
{
    auto totalWidget = new QWidget(this);
    auto ly_total = new QVBoxLayout(totalWidget);

    auto ly_top = new QHBoxLayout();
//    ly_top->setContentsMargins(12, 12, 12, 1);

    auto bt_pull = new QPushButton(tr("拉取类型和标签信息"), totalWidget);
    connect(bt_pull, &QPushButton::clicked, totalWidget, [=](){
        importer->pullTypeAndTags();
    });
    ly_top->addWidget(bt_pull, 1);

    auto bt_upload_simple = new QPushButton(tr("简单上传(不计算特征)"), totalWidget);
    bt_upload_simple->setEnabled(false);
    ly_top->addWidget(bt_upload_simple, 1);

    auto bt_upload = new QPushButton(tr("上传"), totalWidget);
    bt_upload->setEnabled(false);
    ly_top->addWidget(bt_upload, 1);

    ly_total->addLayout(ly_top);

    PreviewWidget *previewWidget = new PreviewWidget(importer->getInfoRef(),
                                                     2,
                                                     3,
                                                     static_cast<PreviewWidget::PreviewType>(importer->type()),
                                                     Qt::Orientation::Horizontal,
                                                     true,
                                                     totalWidget);
    ly_total->addWidget(previewWidget);

    connect(previewWidget, &PreviewWidget::onPreview, totalWidget, [importer, previewWidget](const std::vector<int> &index) {
        previewWidget->previewFiles(importer->getFilePaths(index), importer->getFileNames(index), true);
        auto panes = previewWidget->getPreviewPane();
        for (size_t i = 0; i < panes.size(); i++) {
            if (i < index.size()) {
                auto renderer = panes[i]->getRenderWidget()->getRenderer();
                auto trans_pair = importer->getTransform(index[i]);
                renderer->setModelTransform(trans_pair.first);
                renderer->setCameraTransform(trans_pair.second);
            }
        }
    });

    connect(bt_upload_simple, &QPushButton::clicked, totalWidget, [=]() {
        // 上传
        bt_upload->setEnabled(false);
        bt_upload_simple->setEnabled(false);
        importer->upload_simple();
    });

    connect(bt_upload, &QPushButton::clicked, totalWidget, [=]() {
        // 上传
        bt_upload->setEnabled(false);
        bt_upload_simple->setEnabled(false);
        importer->upload();
    });

    connect(importer, &AssetImporter::onClear, this, [=](){
        previewWidget->clearInfo();
    });

    connect(wizard, &WizardWidget::onSwitchStep, this, [=]() {
        // 当进行到当前步骤时
        if (totalWidget == wizard->currentStep()->widget) {
            previewWidget->refreshInfo();
            previewWidget->selectGroup(0);
        }
    });

    connect(importer, &AssetImporter::onTypeAndTagsLoaded, this, [=](){
        bt_upload->setEnabled(true);
        bt_upload_simple->setEnabled(true);
        m_logging_widget->trace("类型与标签拉取成功");
        previewWidget->refreshInfo();
    });

    connect(importer, &AssetImporter::onUploadSuccessful, this, [=](){
        bt_upload->setEnabled(false);
        bt_upload_simple->setEnabled(false);
        importer->clear();
        m_logging_widget->trace("上传成功");
        QMessageBox::information(this, tr("模型资源上传"), tr("上传成功!"));
    });

    connect(importer, &AssetImporter::onUploadSimpleSuccessful, this, [=](){
        bt_upload->setEnabled(false);
        bt_upload_simple->setEnabled(false);
        importer->clear();
        m_logging_widget->trace("简单上传成功");
        QMessageBox::information(this, tr("模型资源简单上传"), tr("简单上传成功!"));
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
