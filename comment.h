// #include "homewindow.h"

// #include <QHBoxLayout>
// #include <QVBoxLayout>
// #include <QApplication>
// #include <QDebug>
// #include <QDialog>
// #include <QKeyEvent>
// #include <QCloseEvent>
// #include <QShowEvent>
// #include <QDockWidget>
// #include <QMessageBox>

// #include <QNetworkReply>
// #include <QUrl>

// #include "optionwindow.h"
// #include "importwindow.h"
// #include "gui/uicomponent/modelsearchwidget.h"
// #include "gui/uicomponent/categoryselector.h"
// #include "gui/uicomponent/loggingwidget.h"
// #include "gui/indexwindow.h"
// #include "function/layoutmanager.h"
// #include "gui/typemanagerwindow.h"
// #include "gui/tagsmanagerwindow.h"
// #include "gui/dbtablewindow.h"
// #include "gui/versionwindow.h"
// #include "gui/directorywindow.h"
// #include "gui/scmanagerwindow.h"

// HomeWindow::HomeWindow(QWidget* parent, QApplication* current_app) : QMainWindow(parent)
// {
//     // 授权
// //    this->setEnabled(false);
// //    auto network_access_manager_authorization = new QNetworkAccessManager(this);
// //    connect(network_access_manager_authorization, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply){
// //        QByteArray data = reply->readAll();
// //        QJsonDocument doc = QJsonDocument::fromJson(data);
// //        if (doc.isNull()) {
// //            qDebug() << "Failed to create JSON doc.";
// //            QMessageBox::critical(this,"错误","鉴权错误！");
// //            QCoreApplication::exit(1);
// //            return;
// //        }

// //        if (!doc.isObject()) {
// //            qDebug() << "JSON is not an object.";
// //            QMessageBox::critical(this,"错误","鉴权错误！");
// //            QCoreApplication::exit(1);
// //            return;
// //        }

// //        QJsonObject obj = doc.object();
// //        if (obj.isEmpty()) {
// //            qDebug() << "JSON object is empty.";
// //            QMessageBox::critical(this,"错误","鉴权错误！");
// //            QCoreApplication::exit(1);
// //            return;
// //        }

// //        if (obj.contains("code") && obj["code"] == 0){
// //            qDebug() << "authorization code is: " << QString::number(obj["code"].toInt());
// //            this->setEnabled(true);
// //            return;
// //        }
// //        else{
// //            qDebug() << "authorization code is: " << QString::number(obj["code"].toInt());
// //            QMessageBox::about(this,"错误","未获授权！");
// //            QCoreApplication::exit(1);
// //            return;
// //        }
// //    });
// //    QUrl authorization_url("http://localhost:9722/auth?appcode=APP1601&pincode=1234567890");
// //    QNetworkRequest authorization_request(authorization_url);
// //    network_access_manager_authorization->get(authorization_request);

//     this->setWindowTitle(tr("主页 - 模型检索"));

//     setWindowIcon(QIcon(":/assets/assets/icon/icon.jpg"));

//     auto centerWidget = new QWidget(this);
//     this->setCentralWidget(centerWidget);

//     auto ly_total = new QVBoxLayout(centerWidget);

//     makeMenu();

//     auto cate_sel = new CategorySelector(centerWidget);
// //    ly_total->addWidget(cate_sel);
//     auto docker_cate = new QDockWidget(tr("检索 - 大类选择器"), centerWidget);
//     docker_cate->setObjectName("docker_cate");
//     docker_cate->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
//     docker_cate->setWidget(cate_sel);
//     docker_cate->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
//     addDockWidget(Qt::TopDockWidgetArea, docker_cate);

//     auto logging_widget = new LoggingWidget(centerWidget);
//     auto docker_logging = new QDockWidget(tr("检索 - 信息输出"), centerWidget);
//     docker_logging->setObjectName("docker_logging");
//     docker_logging->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
//     docker_logging->setWidget(logging_widget);
//     docker_logging->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
//     addDockWidget(Qt::RightDockWidgetArea, docker_logging);

//     m_tabw = new QTabWidget(centerWidget);
//     ly_total->addWidget(m_tabw);

//     auto sw_content = new ModelSearchWidget(ModelSearch::SearchType::CONTENT,centerWidget);
//     m_tabw->addTab(sw_content, "模型内容检索");
//     auto sw_label = new ModelSearchWidget(ModelSearch::SearchType::TAGS,centerWidget);
//     m_tabw->addTab(sw_label, "模型标签检索");
//     auto sw_type = new ModelSearchWidget(ModelSearch::SearchType::TYPE,centerWidget);
//     m_tabw->addTab(sw_type, "模型类型检索");

//     connect(sw_content->getSearcher(), &ModelSearch::onResponsing, this, [=](const QString & info, bool is_continue){
//         is_continue ? logging_widget->info(info) : logging_widget->warning(info);
//     });
//     connect(sw_label->getSearcher(), &ModelSearch::onResponsing, this, [=](const QString & info, bool is_continue){
//         is_continue ? logging_widget->info(info) : logging_widget->warning(info);
//     });

//     connect(sw_type->getSearcher(), &ModelSearch::onResponsing, this, [=](const QString & info, bool is_continue){
//         is_continue ? logging_widget->info(info) : logging_widget->warning(info);
//     });

//     this->resize(800, 600);

// }

// HomeWindow::~HomeWindow()
// {
// //    qDebug() << "HomeWindow Closed";
// }

// void HomeWindow::keyPressEvent(QKeyEvent *ev)
// {
//     // TODO: 方便调试
//     if(ev->key() == Qt::Key::Key_Escape) {
//         close();
//     }
// }

// void HomeWindow::closeEvent(QCloseEvent *event)
// {
//     saveLayout();
//     QWidget::closeEvent(event);
// }

// void HomeWindow::showEvent(QShowEvent *event)
// {
//     if (_is_first_paint) {
//         _is_first_paint = false;
//         restoreLayout();
//     }
//     update();
// }

// void HomeWindow::makeMenu()
// {
//     m_top_menubar = new QMenuBar(this);

//     // ----------------文件----------------
//     {
//         auto menu_file = new QMenu(tr("文件(&F)"), this);
//         m_top_menubar->addMenu(menu_file);

//         // ----------------查看资源数据库----------------
//         menu_file->addAction(tr("模型数据表预览(&T)"), this, [this](){
//             auto winName = "模型数据表预览";
//             auto fm = FunctionWnidowManager::getInstance();
//             if (!fm->show(winName, this->isMaximized(), true)) {
//                 fm->create(winName, new DBTableWindow(this));
//             }
//         }, QKeySequence("Ctrl+T"));

//         // ----------------导入----------------
//         menu_file->addAction(tr("资源导入(&I)"), this, [this](){
//             auto winName = "资源导入";
//             auto fm = FunctionWnidowManager::getInstance();
//             if (!fm->show(winName, this->isMaximized(), true)) {
//                 fm->create(winName, new ImportWindow(this));
//             }
//         }, QKeySequence("Ctrl+I"));

//         menu_file->addSeparator();

//         // ----------------选项----------------
//         menu_file->addAction(tr("选项(&O)"), this, [this](){
//                 auto winName = "选项";
//                 auto fm = FunctionWnidowManager::getInstance();
//                 if (!fm->show(winName, this->isMaximized(), true)) {
//                     fm->create(winName, new OptionWindow(this));
//                 }
//         }, QKeySequence("Ctrl+O"));

//         menu_file->addSeparator();

//         // ----------------退出----------------
//         menu_file->addAction(tr("退出(&X)"), this, [=](){
//             this->close();
//         }, QKeySequence("Ctrl+w"));

//     }

//     // ----------------编辑----------------
//     {
//         auto menu_version = new QMenu(tr("编辑(&V)"), this);

//         // ----------------标签----------------
//         menu_version->addAction(tr("标签管理"), this, [=](){
//             auto winName = "标签管理";
//             auto fm = FunctionWnidowManager::getInstance();
//             if (!fm->show(winName, this->isMaximized(), true)){
//                 fm->create(winName, new TagsManagerWindow(this));
//             }
//         });

//         // ----------------类型----------------
//         menu_version->addAction(tr("类型管理"), this, [=](){
//             auto winName = "类型管理";
//             auto fm = FunctionWnidowManager::getInstance();
//             if (!fm->show(winName, this->isMaximized(), true)){
//                 fm->create(winName, new TypeManagerWindow(this));
//             }
//         });

//         menu_version->addSeparator();

//         // ----------------索引----------------
//         menu_version->addAction(tr("索引管理"), this, [=](){
//             auto winName = "索引管理";
//             auto fm = FunctionWnidowManager::getInstance();
//             if (!fm->show(winName, this->isMaximized(), true)){
//                 fm->create(winName, new IndexWindow(this));
//             }
//         });

//         menu_version->addSeparator();

//         // ----------------目录管理----------------
//         menu_version->addAction(tr("标准分类管理"), this, [=](){
//             auto winName = "标准分类管理";
//             auto fm = FunctionWnidowManager::getInstance();
//             if (!fm->show(winName, this->isMaximized(), true)){
//                 fm->create(winName, new SCManagerWindow(this));
//             }
//         });
//         menu_version->addAction(tr("标准分类目录管理"), this, [=](){
//             auto winName = "标准分类目录管理";
//             auto fm = FunctionWnidowManager::getInstance();
//             if (!fm->show(winName, this->isMaximized(), true)){
//                 fm->create(winName, new SCDirWindow(this));
//             }
//         });
//         menu_version->addAction(tr("标签目录管理"), this, [=](){
//             auto winName = "标签目录管理";
//             auto fm = FunctionWnidowManager::getInstance();
//             if (!fm->show(winName, this->isMaximized(), true)){
//                 fm->create(winName, new TagDirWindow(this));
//             }
//         });

//         menu_version->addSeparator();

//         // ----------------版本管理----------------
//         menu_version->addAction(tr("版本管理"), this, [=]() {
//             auto winName = "版本管理";
//             auto fm = FunctionWnidowManager::getInstance();
//             if (!fm->show(winName, this->isMaximized(), true)) {
//                 fm->create(winName, new VersionWindow(this));
//             }
//         });
//         m_top_menubar->addMenu(menu_version);
//     }

//     // ----------------视图----------------
//     {
//         auto menu_view = new QMenu(tr("视图(&V)"), this);

//         // ----------------保存布局----------------
//         menu_view->addAction(tr("保存布局"), this, [=]() {
//             saveLayout();
//         });
//         // ----------------加载布局----------------
//         menu_view->addAction(tr("加载布局"), this, [=]() {
//             restoreLayout();
//         });

//         m_top_menubar->addMenu(menu_view);
//     }

//     this->setMenuBar(m_top_menubar);

// }

// void HomeWindow::saveLayout()
// {
//     LayoutManager::getInstance()->save(this, "HomeWindow");
// }

// void HomeWindow::restoreLayout()
// {
//     LayoutManager::getInstance()->restore(this, "HomeWindow");
// }
// #include "importwindow.h"

// #include <QGridLayout>
// #include <QHBoxLayout>
// #include <QVBoxLayout>
// #include <QPushButton>
// #include <QFileDialog>
// #include <QSpacerItem>
// #include <QFileInfo>
// #include <QTabWidget>
// #include <QDockWidget>
// #include <QMessageBox>
// #include <QCloseEvent>
// #include <QShowEvent>
// #include <QMenuBar>
// #include <QMenu>
// #include <QSplitter>
// #include <QProgressDialog>
// #include <QDesktopServices>

// #include "uicomponent/previewwidget.h"
// #include "function/configer/configmanager.h"
// #include "gui/uicomponent/categoryselector.h"
// #include "gui/uicomponent/loggingwidget.h"
// #include "function/layoutmanager.h"
// #include "gui/uicomponent/wizardwidget.h"
// #include "gui/uicomponent/infotablewidget.h"
// #include "gui/uicomponent/interactivewidget.h"
// #include "gui/uicomponent/previewpane.h"
// #include "gui/uicomponent/renderwidget.h"
// #include "function/renderer/renderer.h"
// #include "gui/uicomponent/sspeditor.h"
// #include "gui/uicomponent/thumbwidget.h"

// ImportWindow::ImportWindow(QWidget *parent)
//     : IFunctionWindow("", {800, 600}, true, false, true, parent)
// {
//     auto center_widget = centralWidget();

//     // ----------------视图----------------
//     {
//         auto top_menubar = new QMenuBar(this);
//         auto menu_view = new QMenu(tr("视图(&V)"), this);

//         // ----------------保存布局----------------
//         menu_view->addAction(tr("保存布局"), this, [=]() {
//             LayoutManager::getInstance()->save(this, "IndexWindow");
//         });
//         // ----------------加载布局----------------
//         menu_view->addAction(tr("加载布局"), this, [=]() {
//             LayoutManager::getInstance()->restore(this, "IndexWindow");
//         });

//         top_menubar->addMenu(menu_view);
//         this->setMenuBar(top_menubar);
//     }

//     auto ly_total = new QVBoxLayout(center_widget);

//     auto cate_sel = new CategorySelector(center_widget);
//     auto docker_cate = new QDockWidget(tr("资源导入 - 大类选择器"), center_widget);
//     docker_cate->setObjectName("docker_cate");
//     docker_cate->setWidget(cate_sel);
//     docker_cate->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
//     docker_cate->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
//     addDockWidget(Qt::TopDockWidgetArea, docker_cate);

//     m_logging_widget = new LoggingWidget(center_widget);
//     auto docker_logging = new QDockWidget(tr("资源导入 - 信息输出"), center_widget);
//     docker_logging->setObjectName("docker_logging");
//     docker_logging->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
//     docker_logging->setWidget(m_logging_widget);
//     docker_logging->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
//     addDockWidget(Qt::RightDockWidgetArea, docker_logging);

//     auto tabw = new QTabWidget(center_widget);
//     ly_total->addWidget(tabw);

//     auto modelImporter = new AssetImporter(AssetImporter::ImportType::MODEL, this);
//     tabw->addTab(setupModelWizard(modelImporter), tr("模型导入"));
// //    tabw->addTab(setupBrowseWidget(modelImporter), tr("模型导入"));

//     auto bvhImporter = new AssetImporter(AssetImporter::ImportType::BVH, this);
//     tabw->addTab(setupBVHWizard(bvhImporter), tr("骨骼动画导入"));
// //    tabw->addTab(setupBrowseWidget(bvhImporter), tr("骨骼动画导入"));

//     auto effectImporter = new AssetImporter(AssetImporter::ImportType::EFFECT, this);
//     tabw->addTab(setupEffectWizard(effectImporter), tr("特效资源导入"));
// //    tabw->addTab(setupBrowseWidget(effectImporter), tr("特效资源导入"));

//     connect(tabw, &QTabWidget::currentChanged, this, [this, tabw](int index){
//         this->setWindowTitle(tabw->tabText(index));
//     });

//     emit tabw->currentChanged(0);
// }

// WizardWidget *ImportWindow::setupModelWizard(AssetImporter *importer)
// {
//     auto ww = new WizardWidget(this);
//     // 1. 添加模型文件
//     ww->pushStep([=](){return setupBrowseWidget(importer, ww);}, "添加模型文件");
//     // 2. 对齐原点, + 覆盖原模型文件
//     ww->pushStep([=](){return setupModel_UniformFormat(importer, ww);}, tr("对齐原点 + 覆盖原模型文件"));
//     // 3. 三维模型变换 + 摄像机位置调整
//     ww->pushStep([=](){return setupModel_TransformCamera(importer, ww);}, tr("三维模型变换 + 摄像机位置调整"));
//     // 4. 缩略图创建
//     ww->pushStep([=](){return setup_Thumb(importer, ww);}, tr("缩略图创建"));
//     // 5. 编辑类型和标签属性 + 资源上传
//     ww->pushStep([=](){return setupModel_Upload(importer, ww);}, tr("编辑类型和标签属性 + 资源上传"));

//     connect(importer, &AssetImporter::onResponsing, this, [=](const QString & info, bool is_continue){
//         is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
//     });

//     return ww;
// }

// WizardWidget *ImportWindow::setupBVHWizard(AssetImporter *importer)
// {
//     auto ww = new WizardWidget(this);
//     // 1. 添加骨骼动画文件
//     ww->pushStep([=](){return setupBrowseWidget(importer, ww);}, tr("添加骨骼动画文件"));
//     // 2. 几何尺度编辑
//     ww->pushStep([=](){return setupBVH_GeometryScale(importer, ww);}, tr("几何尺度编辑"));
//     // 3. 采样频率编辑
//     ww->pushStep([=](){return setupBVH_SampleFreq(importer, ww);}, tr("采样频率编辑"));
//     // 4. 缩略图创建
//     ww->pushStep([=](){return setup_Thumb(importer, ww);}, tr("缩略图创建"));
//     // 5. 编辑类型和标签属性 + 资源上传
//     ww->pushStep([=](){return setupBVH_Upload(importer, ww);}, tr("编辑类型和标签属性 + 资源上传"));

//     connect(importer, &AssetImporter::onResponsing, this, [=](const QString & info, bool is_continue){
//         is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
//     });

//     return ww;
// }

// WizardWidget *ImportWindow::setupEffectWizard(AssetImporter *importer)
// {
//     auto ww = new WizardWidget(this);
//     // 1. 添加特效图形文件
//     ww->pushStep([=](){return setupBrowseWidget(importer, ww);}, "添加特效图形文件");
//     // 2. 编辑类型和标签属性 + 资源上传
//     ww->pushStep([=](){return setupEffect_Upload(importer, ww);}, tr("编辑类型和标签属性 + 资源上传"));

//     connect(importer, &AssetImporter::onResponsing, this, [=](const QString & info, bool is_continue){
//         is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
//     });

//     return ww;
// }

// QWidget *ImportWindow::setupBrowseWidget(AssetImporter *importer, WizardWidget* wizard)
// {

//     QPushButton *bt_fileBrowse, *bt_clear;

//     auto totalWidget = new QWidget(this);

//     connect(wizard, &WizardWidget::onSwitchStep, this, [=]() {
//         // 当进行到当前步骤时
//         if (totalWidget == wizard->currentStep()->widget) {
//             wizard->setNextButtonEnable(false);
//         }
//     });

//     PreviewWidget *previewWidget = new PreviewWidget(importer->getInfoRef(),
//                                                      2,
//                                                      3,
//                                                      static_cast<PreviewWidget::PreviewType>(importer->type()),
//                                                      Qt::Orientation::Horizontal,
//                                                      true,
//                                                      true,
//                                                      totalWidget);

//     // TODO: 特效图形的预览暂时先写在这里,未来写进PreviewWidget
//     if (importer->type() == AssetImporter::ImportType::EFFECT)
//     {
//         connect(previewWidget->getInfoTable(), &InfoTableWidget::onGroupSelected, this, [=](const std::vector<int> &index){
//             auto m_previewPanes = previewWidget->getPreviewPane();
//             for (size_t i = 0; i < m_previewPanes.size(); i++) {
//                 disconnect(m_previewPanes[i], &PreviewPane::onSelectedPane, this, 0);
//                 if (i < index.size()) {
//                     connect(m_previewPanes[i], &PreviewPane::onSelectedPane, this, [=]() {
//                         auto effectPaths = importer->getFilePaths(index);
//                         QDesktopServices::openUrl(effectPaths[i]);
//                     });
//                 }
//             }
//         });
//     }

//     connect(importer, &AssetImporter::onAddPaths, totalWidget, [=]() {
//         wizard->setNextButtonEnable(true);
//         previewWidget->refreshInfo();
//     });

//     connect(importer, &AssetImporter::onClear, totalWidget, [=]() {
//         wizard->setNextButtonEnable(false);
//         previewWidget->clearInfo();
//     });

//     auto ly_top = new QHBoxLayout();
//     ly_top->setContentsMargins(12, 12, 12, 1);

//     bt_fileBrowse = new QPushButton(tr("添加资源"), totalWidget);
//     connect(bt_fileBrowse, &QPushButton::clicked, totalWidget, [=]() {
//         QStringList selectedFiles;
//         QString config_key, open_title, open_option;
//         switch (importer->type()) {
//         case AssetImporter::ImportType::MODEL: {
//             config_key = "FileBrowser/ImportType_MODEL";
//             open_title = tr("浏览模型文件");
//             open_option = "*.obj;*.fbx";
//             break;
//         }
//         case AssetImporter::ImportType::BVH: {
//             config_key = "FileBrowser/ImportType_BVH";
//             open_title = tr("浏览骨骼动画文件");
//             open_option = "*.bvh";
//             break;
//         }
//         case AssetImporter::ImportType::EFFECT: {
//             config_key = "FileBrowser/ImportType_EFFECT";
//             open_title = tr("浏览特效文件");
//             open_option = "*.exe;*.bat";
//             break;
//         }
//         }
//         QString open_dir = ConfigManager::getInstance()->getConfig(config_key).toString();
//         selectedFiles = QFileDialog::getOpenFileNames(totalWidget, open_title, open_dir, open_option);
//         if (selectedFiles.size() > 0)
//             ConfigManager::getInstance()->setConfig(config_key, QFileInfo(selectedFiles[0]).absolutePath());

//         importer->addPathsNotExist(selectedFiles);
//         previewWidget->selectGroup(0);
//     });

//     ly_top->addWidget(bt_fileBrowse, 1);

//     bt_clear = new QPushButton(tr("清空"), totalWidget);
//     connect(bt_clear, &QPushButton::clicked, totalWidget, [importer]() {
//         // 清空预览与表格
//         importer->clear();
//     });
//     ly_top->addWidget(bt_clear, 1);

//     ly_top->addSpacerItem(new QSpacerItem(40, 20));
//     ly_top->setStretch(ly_top->count() - 1, 4);

//     auto ly_total = new QVBoxLayout(totalWidget);
//     totalWidget->setLayout(ly_total);

//     ly_total->addLayout(ly_top, 0);

//     connect(previewWidget, &PreviewWidget::onPreview, totalWidget, [importer, previewWidget](const std::vector<int> &index) {
//         previewWidget->previewFiles(importer->getFilePaths(index), importer->getFileNames(index), false);
//     });

//     ly_total->addWidget(previewWidget, 1);

//     ly_total->setSpacing(1);

//     return totalWidget;
// }

// QWidget *ImportWindow::setup_Thumb(AssetImporter *importer, WizardWidget *wizard)
// {
//     auto totalWidget = new QWidget(this);
//     auto ly_total = new QVBoxLayout(totalWidget);

//     auto thumbWidget = new ThumbWidget(importer->getInfoRef(),
//                                        importer->type(),
//                                        6,
//                                        9,
//                                        totalWidget);
//     ly_total->addWidget(thumbWidget);

//     auto previewWidget = thumbWidget->getPreviewWidget();

//     connect(previewWidget, &PreviewWidget::onPreview, totalWidget, [=](const std::vector<int> &index) {
//         previewWidget->previewFiles(importer->getFilePaths(index), importer->getFileNames(index), false);
//         if (importer->type() == AssetImporter::ImportType::MODEL) {
//             QMessageBox::information(totalWidget, tr("模型缩略图创建"), tr("模型缩略图创建成功！"));
//         } else if (importer->type() == AssetImporter::ImportType::BVH) {
//             QMessageBox::information(totalWidget, tr("骨骼动画缩略图创建"), tr("骨骼动画缩略图创建成功！"));
//         }
//     });

//     connect(wizard, &WizardWidget::onSwitchStep, this, [=]() {
//         // 当进行到当前步骤时
//         if (totalWidget == wizard->currentStep()->widget) {
//             previewWidget->refreshInfo();
//             previewWidget->selectGroup(0);
//         }
//     });

//     return totalWidget;
// }

// QWidget *ImportWindow::setupModel_UniformFormat(AssetImporter *importer, WizardWidget* wizard)
// {
//     auto totalWidget = new QWidget(this);
//     auto ly_total = new QVBoxLayout(totalWidget);
//     auto bt_align = new QPushButton(tr("对齐原点并转换格式"), this);
//     ly_total->addWidget(bt_align);

//     PreviewWidget *previewWidget = new PreviewWidget(importer->getInfoRef(),
//                                                      2,
//                                                      3,
//                                                      static_cast<PreviewWidget::PreviewType>(importer->type()),
//                                                      Qt::Orientation::Horizontal,
//                                                      true,
//                                                      true,
//                                                      totalWidget);
//     ly_total->addWidget(previewWidget);

//     connect(wizard, &WizardWidget::onSwitchStep, this, [=]() {
//         // 当进行到当前步骤时
//         if (totalWidget == wizard->currentStep()->widget) {
//             previewWidget->refreshInfo();
//             previewWidget->selectGroup(0);
//         }
//     });

//     connect(previewWidget, &PreviewWidget::onPreview, totalWidget, [importer, previewWidget](const std::vector<int> &index) {
//         previewWidget->previewFiles(importer->getFilePaths(index), importer->getFileNames(index), false);
//     });

//     connect(bt_align, &QPushButton::clicked, this, [=]() {
//         int res = QMessageBox::warning(this, tr("操作警告"), tr("转换格式可能覆盖原模型文件, 且操作不可撤销!"), QMessageBox::StandardButton::Cancel, QMessageBox::StandardButton::Ok);
//         if (res != QMessageBox::StandardButton::Ok)
//             return;
//         bt_align->setEnabled(false);
//         importer->alignToOrigin();
//     });

//     auto progressDialog = new QProgressDialog(tr(""), tr("隐藏进度显示, 转换不会终止"), 0, 1, this);
//     progressDialog->reset();
//     auto prog_bt_cancel = new QPushButton(tr("完成"), progressDialog);
//     prog_bt_cancel->setEnabled(false);
//     progressDialog->setCancelButton(prog_bt_cancel);
//     progressDialog->setWindowFlag(Qt::WindowCloseButtonHint, false);
//     progressDialog->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
//     progressDialog->setAutoClose(false);

//     connect(importer, &AssetImporter::onStartAlignToOrigin, this, [=](int size) {
//         progressDialog->setLabelText(QString().asprintf("已转换 0 个, 剩余 %d 个, 请勿关闭导入程序", size));
//         progressDialog->setMaximum(size);
//         progressDialog->open();
//     });
//     connect(importer, &AssetImporter::onDoneAlignToOrigin, this, [=](int progress) {
//         progressDialog->setLabelText(QString().asprintf("已转换 %d 个, 剩余 %d 个, 请勿关闭导入程序", progress, progressDialog->maximum() - progress));
//         progressDialog->setValue(progress);
//     });
//     connect(importer, &AssetImporter::saveSuccessful, this, [=]() {
//         progressDialog->setLabelText(QString().asprintf("已转换 %d 个, 剩余 0 个.", progressDialog->maximum()));
//         prog_bt_cancel->setEnabled(true);
//         progressDialog->reset();

//         m_logging_widget->trace("保存成功");

//         previewWidget->refreshInfo();
//     });
//     return totalWidget;
// }

// QWidget *ImportWindow::setupModel_TransformCamera(AssetImporter *importer, WizardWidget* wizard)
// {
//     auto totalWidget = new QWidget(this);
//     auto ly_total = new QHBoxLayout(totalWidget);

//     auto infotable = new InfoTableWidget(importer->getInfoRef(), 1, false, true, totalWidget);

//     auto splitter = new QSplitter(Qt::Horizontal, totalWidget);
//     ly_total->addWidget(splitter);

// //    ly_total->addWidget(infotable, 1);
//     splitter->addWidget(infotable);

//     auto interact = new InteractiveWidget(totalWidget);
// //    ly_total->addWidget(interact, 2);
//     splitter->addWidget(interact);

//     splitter->setStretchFactor(0, 1);
//     splitter->setStretchFactor(1, 1);

//     connect(wizard, &WizardWidget::onSwitchStep, this, [=]() {
//         // 当进行到当前步骤时
//         if (totalWidget == wizard->currentStep()->widget) {
//             infotable->refresh();
//             auto renderer = interact->getPreviewPane()->getRenderWidget()->getRenderer();
//             importer->initAllTransform(renderer->getModelTransform(), renderer->getCameraTransform());
//             infotable->selectGroup(0);
//         }
//     });

//     connect(infotable, &InfoTableWidget::onGroupSelected, this, [=](const std::vector<int> &rows) {
//         auto previewPane = interact->getPreviewPane();
//         previewPane->doClear();
//         disconnect(previewPane, &PreviewPane::onSelectedPane, this, 0);
//         if (!rows.empty()) {
//             m_transform_row_selected = rows[0];
//             connect(previewPane, &PreviewPane::onSelectedPane, this, [=]() {
//                 infotable->jumpTo(m_transform_row_selected);
//             });
//             QString filePaht = importer->getFilePaths()[m_transform_row_selected];
//             QString fileInfo = importer->getFileNames({m_transform_row_selected})[0];

//             previewPane->doPreviewModel(filePaht, fileInfo, true);

//             auto renderer = interact->getPreviewPane()->getRenderWidget()->getRenderer();
//             auto trans_pair = importer->getTransform(m_transform_row_selected);
//             renderer->setModelTransform(trans_pair.first);
//             renderer->setCameraTransform(trans_pair.second);
//         }
//     });
//     connect(interact, &InteractiveWidget::onSaveTransform, this, [=](){
//         auto renderer = interact->getPreviewPane()->getRenderWidget()->getRenderer();
//         if (m_transform_row_selected >= 0) {
//             importer->saveTransform(m_transform_row_selected, renderer->getModelTransform(), renderer->getCameraTransform());
//         }
//     });

//     return totalWidget;
// }

// QWidget *ImportWindow::setupModel_Upload(AssetImporter *importer, WizardWidget* wizard)
// {
//     auto totalWidget = new QWidget(this);
//     auto ly_total = new QVBoxLayout(totalWidget);

//     auto ly_top = new QHBoxLayout();
// //    ly_top->setContentsMargins(12, 12, 12, 1);

//     auto bt_pull = new QPushButton(tr("拉取类型和标签信息"), totalWidget);
//     connect(bt_pull, &QPushButton::clicked, totalWidget, [=](){
//         importer->pullTypeAndTags();
//     });
//     ly_top->addWidget(bt_pull, 1);

//     auto bt_upload_simple = new QPushButton(tr("简单上传(不计算特征)"), totalWidget);
//     bt_upload_simple->setEnabled(false);
//     ly_top->addWidget(bt_upload_simple, 1);

//     auto bt_upload = new QPushButton(tr("上传"), totalWidget);
//     bt_upload->setEnabled(false);
//     ly_top->addWidget(bt_upload, 1);

//     ly_total->addLayout(ly_top);

//     PreviewWidget *previewWidget = new PreviewWidget(importer->getInfoRef(),
//                                                      2,
//                                                      3,
//                                                      static_cast<PreviewWidget::PreviewType>(importer->type()),
//                                                      Qt::Orientation::Horizontal,
//                                                      true,
//                                                      true,
//                                                      totalWidget);
//     ly_total->addWidget(previewWidget);

//     connect(previewWidget, &PreviewWidget::onPreview, totalWidget, [importer, previewWidget](const std::vector<int> &index) {
//         previewWidget->previewFiles(importer->getFilePaths(index), importer->getFileNames(index), true);
//         auto panes = previewWidget->getPreviewPane();
//         for (size_t i = 0; i < panes.size(); i++) {
//             if (i < index.size()) {
//                 auto renderer = panes[i]->getRenderWidget()->getRenderer();
//                 auto trans_pair = importer->getTransform(index[i]);
//                 renderer->setModelTransform(trans_pair.first);
//                 renderer->setCameraTransform(trans_pair.second);
//             }
//         }
//     });

//     connect(bt_upload_simple, &QPushButton::clicked, totalWidget, [=]() {
//         // 上传
//         bt_upload->setEnabled(false);
//         bt_upload_simple->setEnabled(false);
//         importer->upload_simple();
//     });

//     connect(bt_upload, &QPushButton::clicked, totalWidget, [=]() {
//         // 上传
//         bt_upload->setEnabled(false);
//         bt_upload_simple->setEnabled(false);
//         importer->upload();
//     });

//     connect(importer, &AssetImporter::onClear, this, [=](){
//         previewWidget->clearInfo();
//     });

//     connect(wizard, &WizardWidget::onSwitchStep, this, [=]() {
//         // 当进行到当前步骤时
//         if (totalWidget == wizard->currentStep()->widget) {
//             previewWidget->refreshInfo();
//             previewWidget->selectGroup(0);
//         }
//     });

//     connect(importer, &AssetImporter::onTypeAndTagsLoaded, this, [=](){
//         bt_upload->setEnabled(true);
//         bt_upload_simple->setEnabled(true);
//         m_logging_widget->trace("类型与标签拉取成功");
//         previewWidget->refreshInfo();
//     });

//     connect(importer, &AssetImporter::onUploadSuccessful, this, [=](){
//         bt_upload->setEnabled(false);
//         bt_upload_simple->setEnabled(false);
//         importer->clear();
//         // TOOD: 暂时写在日志输出里 (几何形状信息分析)
//         m_logging_widget->trace("几何形状信息分析成功");
//         // TOOD: 暂时写在日志输出里 (纹理信息分析)
//         m_logging_widget->trace("纹理信息分析成功");
//         // TOOD: 暂时写在日志输出里 (材质和局部光照明模型编码)
//         m_logging_widget->trace("材质和局部光照明模型编码成功!");
//         m_logging_widget->trace("上传成功");
//         QMessageBox::information(this, tr("模型资源上传"), tr("上传成功!"));
//     });

//     connect(importer, &AssetImporter::onUploadSimpleSuccessful, this, [=](){
//         bt_upload->setEnabled(false);
//         bt_upload_simple->setEnabled(false);
//         importer->clear();
//         // TOOD: 暂时写在日志输出里 (几何形状信息分析)
//         m_logging_widget->trace("几何形状信息分析成功");
//         // TOOD: 暂时写在日志输出里 (纹理信息分析)
//         m_logging_widget->trace("纹理信息分析成功");
//         // TOOD: 暂时写在日志输出里 (材质和局部光照明模型编码)
//         m_logging_widget->trace("材质和局部光照明模型编码成功!");
//         m_logging_widget->trace("简单上传成功");
//         QMessageBox::information(this, tr("模型资源简单上传"), tr("简单上传成功!"));
//     });

//     return totalWidget;
// }

// QWidget *ImportWindow::setupBVH_GeometryScale(AssetImporter *importer, WizardWidget *wizard)
// {
//     auto totalWidget = new QWidget(this);
//     auto ly_total = new QHBoxLayout(totalWidget);

//     auto infotable = new InfoTableWidget(importer->getInfoRef(), 1, false, true, totalWidget);

//     auto splitter = new QSplitter(Qt::Horizontal, totalWidget);
//     ly_total->addWidget(splitter);

//     //    ly_total->addWidget(infotable, 1);
//     splitter->addWidget(infotable);

//     auto interact = new InteractiveWidget(totalWidget);
//     //    ly_total->addWidget(interact, 2);
//     splitter->addWidget(interact);

//     splitter->setStretchFactor(0, 1);
//     splitter->setStretchFactor(1, 1);

//     connect(wizard, &WizardWidget::onSwitchStep, this, [=]() {
//         // 当进行到当前步骤时
//         if (totalWidget == wizard->currentStep()->widget) {
//             infotable->refresh();
//             auto renderer = interact->getPreviewPane()->getRenderWidget()->getRenderer();
//             importer->initAllTransform(renderer->getModelTransform(), renderer->getCameraTransform());
//             infotable->selectGroup(0);
//         }
//     });

//     connect(infotable, &InfoTableWidget::onGroupSelected, this, [=](const std::vector<int> &rows) {
//         auto previewPane = interact->getPreviewPane();
//         previewPane->doClear();
//         disconnect(previewPane, &PreviewPane::onSelectedPane, this, 0);
//         if (!rows.empty()) {
//             m_transform_row_selected = rows[0];
//             connect(previewPane, &PreviewPane::onSelectedPane, this, [=]() {
//                 infotable->jumpTo(m_transform_row_selected);
//             });
//             QString filePaht = importer->getFilePaths()[m_transform_row_selected];
//             QString fileInfo = importer->getFileNames({m_transform_row_selected})[0];

//             if (importer->type() == AssetImporter::ImportType::MODEL) {
//                 previewPane->doPreviewModel(filePaht, fileInfo, true);
//             } else if (importer->type() == AssetImporter::ImportType::BVH) {
//                 previewPane->doPreviewBVH(filePaht, fileInfo, true);
//             } else if (importer->type() == AssetImporter::ImportType::EFFECT) {
//                 // TODO: preview effect
//             }

//             auto renderer = interact->getPreviewPane()->getRenderWidget()->getRenderer();
//             auto trans_pair = importer->getTransform(m_transform_row_selected);
//             renderer->setModelTransform(trans_pair.first);
//             renderer->setCameraTransform(trans_pair.second);
//         }
//     });
//     connect(interact, &InteractiveWidget::onSaveTransform, this, [=](){
//         auto renderer = interact->getPreviewPane()->getRenderWidget()->getRenderer();
//         if (m_transform_row_selected >= 0) {
//             importer->saveTransform(m_transform_row_selected, renderer->getModelTransform(), renderer->getCameraTransform());
//         }
//     });

//     return totalWidget;
// }

// QWidget *ImportWindow::setupBVH_SampleFreq(AssetImporter *importer, WizardWidget *wizard)
// {
//     auto totalWidget = new QWidget(this);
//     auto ly_total = new QHBoxLayout(totalWidget);
//     PreviewWidget *previewWidget = new PreviewWidget(importer->getInfoRef(),
//                                                      1,
//                                                      1,
//                                                      static_cast<PreviewWidget::PreviewType>(importer->type()),
//                                                      Qt::Orientation::Horizontal,
//                                                      true,
//                                                      true,
//                                                      totalWidget);
//     auto sspeditor = new SSPEditor(previewWidget, totalWidget);
//     ly_total->addWidget(previewWidget, 3);
//     ly_total->addWidget(sspeditor, 1);

//     connect(previewWidget, &PreviewWidget::onPreview, totalWidget, [importer, previewWidget](const std::vector<int> &index) {
//         previewWidget->previewFiles(importer->getFilePaths(index), importer->getFileNames(index), false);
// //        auto panes = previewWidget->getPreviewPane();
// //        for (size_t i = 0; i < panes.size(); i++) {
// //            if (i < index.size()) {
// //                auto renderer = panes[i]->getRenderWidget()->getRenderer();
// //                auto trans_pair = importer->getTransform(index[i]);
// //                renderer->setModelTransform(trans_pair.first);
// //                renderer->setCameraTransform(trans_pair.second);
// //            }
// //        }
//     });

//     connect(wizard, &WizardWidget::onSwitchStep, this, [=]() {
//         // 当进行到当前步骤时
//         if (totalWidget == wizard->currentStep()->widget) {
//             previewWidget->refreshInfo();
//             previewWidget->selectGroup(0);
//         }
//     });

//     return totalWidget;
// }

// QWidget *ImportWindow::setupBVH_Upload(AssetImporter *importer, WizardWidget *wizard)
// {
//     auto totalWidget = new QWidget(this);
//     auto ly_total = new QVBoxLayout(totalWidget);

//     auto ly_top = new QHBoxLayout();
// //    ly_top->setContentsMargins(12, 12, 12, 1);

//     auto bt_pull = new QPushButton(tr("拉取类型和标签信息"), totalWidget);
//     connect(bt_pull, &QPushButton::clicked, totalWidget, [=](){
//         importer->pullTypeAndTags();
//     });
//     ly_top->addWidget(bt_pull, 1);

//     auto bt_upload = new QPushButton(tr("上传"), totalWidget);
//     bt_upload->setEnabled(false);
//     ly_top->addWidget(bt_upload, 1);

//     ly_total->addLayout(ly_top);

//     PreviewWidget *previewWidget = new PreviewWidget(importer->getInfoRef(),
//                                                      2,
//                                                      3,
//                                                      static_cast<PreviewWidget::PreviewType>(importer->type()),
//                                                      Qt::Orientation::Horizontal,
//                                                      true,
//                                                      true,
//                                                      totalWidget);
//     ly_total->addWidget(previewWidget);

//     connect(previewWidget, &PreviewWidget::onPreview, totalWidget, [importer, previewWidget](const std::vector<int> &index) {
//         previewWidget->previewFiles(importer->getFilePaths(index), importer->getFileNames(index), true);
//         auto panes = previewWidget->getPreviewPane();
//         for (size_t i = 0; i < panes.size(); i++) {
//             if (i < index.size()) {
//                 auto renderer = panes[i]->getRenderWidget()->getRenderer();
//                 auto trans_pair = importer->getTransform(index[i]);
//                 renderer->setModelTransform(trans_pair.first);
//                 renderer->setCameraTransform(trans_pair.second);
//             }
//         }
//     });

//     connect(bt_upload, &QPushButton::clicked, totalWidget, [=]() {
//         // 上传
//         bt_upload->setEnabled(false);
//         importer->upload();
//     });

//     connect(importer, &AssetImporter::onClear, this, [=](){
//         previewWidget->clearInfo();
//     });

//     connect(wizard, &WizardWidget::onSwitchStep, this, [=]() {
//         // 当进行到当前步骤时
//         if (totalWidget == wizard->currentStep()->widget) {
//             previewWidget->refreshInfo();
//             previewWidget->selectGroup(0);
//         }
//     });

//     connect(importer, &AssetImporter::onTypeAndTagsLoaded, this, [=](){
//         bt_upload->setEnabled(true);
//         m_logging_widget->trace("类型与标签拉取成功");
//         previewWidget->refreshInfo();
//     });

//     connect(importer, &AssetImporter::onUploadSuccessful, this, [=](){
//         bt_upload->setEnabled(false);
//         importer->clear();
//         m_logging_widget->trace("上传成功");
//         QMessageBox::information(this, tr("骨骼动画资源上传"), tr("上传成功!"));
//     });

//     return totalWidget;
// }

// QWidget *ImportWindow::setupEffect_Upload(AssetImporter *importer, WizardWidget *wizard)
// {
//     auto totalWidget = new QWidget(this);
//     auto ly_total = new QVBoxLayout(totalWidget);

//     auto ly_top = new QHBoxLayout();
// //    ly_top->setContentsMargins(12, 12, 12, 1);

//     auto bt_pull = new QPushButton(tr("拉取类型和标签信息"), totalWidget);
//     connect(bt_pull, &QPushButton::clicked, totalWidget, [=](){
//         importer->pullTypeAndTags();
//     });
//     ly_top->addWidget(bt_pull, 1);

//     auto bt_upload = new QPushButton(tr("上传"), totalWidget);
//     bt_upload->setEnabled(false);
//     ly_top->addWidget(bt_upload, 1);

//     ly_total->addLayout(ly_top);

//     PreviewWidget *previewWidget = new PreviewWidget(importer->getInfoRef(),
//                                                      2,
//                                                      3,
//                                                      static_cast<PreviewWidget::PreviewType>(importer->type()),
//                                                      Qt::Orientation::Horizontal,
//                                                      true,
//                                                      true,
//                                                      totalWidget);
//     ly_total->addWidget(previewWidget);

//     // TODO: 特效图形的预览暂时先写在这里,未来写进PreviewWidget
//     connect(previewWidget->getInfoTable(), &InfoTableWidget::onGroupSelected, this, [=](const std::vector<int> &index){
//         auto m_previewPanes = previewWidget->getPreviewPane();
//         for (size_t i = 0; i < m_previewPanes.size(); i++) {
//             disconnect(m_previewPanes[i], &PreviewPane::onSelectedPane, this, 0);
//             if (i < index.size()) {
//                 connect(m_previewPanes[i], &PreviewPane::onSelectedPane, this, [=]() {
//                     auto effectPaths = importer->getFilePaths(index);
//                     QDesktopServices::openUrl(effectPaths[i]);
//                 });
//             }
//         }
//     });

//     connect(previewWidget, &PreviewWidget::onPreview, totalWidget, [importer, previewWidget](const std::vector<int> &index) {
//         previewWidget->previewFiles(importer->getFilePaths(index), importer->getFileNames(index), true);
//     });

//     connect(bt_upload, &QPushButton::clicked, totalWidget, [=]() {
//         // 上传
//         bt_upload->setEnabled(false);
//         importer->upload();
//     });

//     connect(importer, &AssetImporter::onClear, this, [=](){
//         previewWidget->clearInfo();
//     });

//     connect(wizard, &WizardWidget::onSwitchStep, this, [=]() {
//         // 当进行到当前步骤时
//         if (totalWidget == wizard->currentStep()->widget) {
//             previewWidget->refreshInfo();
//             previewWidget->selectGroup(0);
//         }
//     });

//     connect(importer, &AssetImporter::onTypeAndTagsLoaded, this, [=](){
//         bt_upload->setEnabled(true);
//         m_logging_widget->trace("类型与标签拉取成功");
//         previewWidget->refreshInfo();
//     });

//     connect(importer, &AssetImporter::onUploadSuccessful, this, [=](){
//         bt_upload->setEnabled(false);
//         importer->clear();
//         m_logging_widget->trace("上传成功");
//         QMessageBox::information(this, tr("特效图形资源上传"), tr("上传成功!"));
//     });

//     return totalWidget;
// }

// void ImportWindow::closeEvent(QCloseEvent *event)
// {
//     LayoutManager::getInstance()->save(this, "ImportWindow");
//     QWidget::closeEvent(event);
// }

// void ImportWindow::showEvent(QShowEvent *event)
// {
//     if (_is_first_paint) {
//         _is_first_paint = false;
//         LayoutManager::getInstance()->restore(this, "ImportWindow");
//     }
//     repaint();
// }
// #include "indexwindow.h"

// #include <QDockWidget>
// #include <QTabWidget>
// #include <QGridLayout>
// #include <QHBoxLayout>
// #include <QVBoxLayout>
// #include <QPushButton>
// #include <QSpacerItem>
// #include <QShowEvent>
// #include <QCloseEvent>
// #include <QMenuBar>
// #include <QMenu>
// #include <QMessageBox>
// #include <QGroupBox>
// #include <QLabel>
// #include <QLineEdit>

// #include "gui/uicomponent/categoryselector.h"
// #include "gui/uicomponent/infotablewidget.h"
// #include "gui/uicomponent/previewwidget.h"
// #include "gui/uicomponent/loggingwidget.h"
// #include "function/layoutmanager.h"

// IndexWindow::IndexWindow(QWidget *parent)
//     : IFunctionWindow(tr("索引编辑"), {800, 600}, true, false, true, parent)
// {

//     // ----------------视图----------------
//     {
//         auto top_menubar = new QMenuBar(this);
//         auto menu_view = new QMenu(tr("视图(&V)"), this);

//         // ----------------保存布局----------------
//         menu_view->addAction(tr("保存布局"), this, [=]() {
//             LayoutManager::getInstance()->save(this, "IndexWindow");
//         });
//         // ----------------加载布局----------------
//         menu_view->addAction(tr("加载布局"), this, [=]() {
//             LayoutManager::getInstance()->restore(this, "IndexWindow");
//         });

//         top_menubar->addMenu(menu_view);
//         this->setMenuBar(top_menubar);
//     }

//     auto index_editor_cont = new IndexEditor(IndexEditor::IndexType::CONTENT, this);
//     auto index_editor_tags = new IndexEditor(IndexEditor::IndexType::TAGS,    this);
//     auto index_editor_type = new IndexEditor(IndexEditor::IndexType::TYPE,    this);

//     auto center_widget = centralWidget();

//     auto ly_total = new QGridLayout(center_widget);

//     m_index_infotable = new InfoTableWidget(index_editor_cont->getIndexInfo(), 1, false, true, center_widget);

//     auto tab_w = new QTabWidget(center_widget);
//     auto model_cont = initModelWidget(index_editor_cont);
//     auto model_tags = initModelWidget(index_editor_tags);
//     auto model_type = initModelWidget(index_editor_type);
//     tab_w->addTab(model_cont, tr("模型内容数据"));
//     tab_w->addTab(model_tags, tr("模型标签数据"));
//     tab_w->addTab(model_type, tr("模型类型数据"));

//     ly_total->addWidget(tab_w);

//     auto indexWidget = new QWidget(center_widget);

//     auto ly_index = new QHBoxLayout(indexWidget);

//     auto bt_sync = new QPushButton(tr("→\n→\n→\n同\n步\n修\n改\n到\n原\n始\n数\n据\n→\n→\n→\n"),center_widget);
//     {
//         bt_sync->setMaximumWidth(bt_sync->fontMetrics().averageCharWidth() * 4);
//         auto plc_bt_sync = bt_sync->sizePolicy();
//         plc_bt_sync.setVerticalPolicy(QSizePolicy::Expanding);
//         bt_sync->setSizePolicy(plc_bt_sync);
//     }
//     ly_index->addWidget(m_index_infotable, 1);
//     ly_index->addWidget(bt_sync,   0);

//     // 选中索引功能
//     connect(m_index_infotable, &InfoTableWidget::itemDoubleClicked, this, [=](QTableWidgetItem *item) {
//         auto row = item->row();
//         if (row < 0) {
//             qDebug() << "IndexWindow>>InfoTableWidget::itemDoubleClicked>> row < 0";
//             return;
//         }
//         m_selected_row = row;
//         if (model_cont == tab_w->currentWidget()) {
//             emit onIndexSelected(IndexEditor::IndexType::CONTENT);
//         } else if (model_tags == tab_w->currentWidget()) {
//             emit onIndexSelected(IndexEditor::IndexType::TAGS);
//         } else if (model_type == tab_w->currentWidget()) {
//             emit onIndexSelected(IndexEditor::IndexType::TYPE);
//         }
//     });

//     connect(bt_sync, &QPushButton::clicked, this, [=]() {
//         if (model_cont == tab_w->currentWidget()) {
//             index_editor_cont->syncToOrg();
//         } else if (model_tags == tab_w->currentWidget()) {
//             index_editor_tags->syncToOrg();
//         } else if (model_type == tab_w->currentWidget()) {
//             index_editor_type->syncToOrg();
//         }
//     });

//     connect(tab_w, &QTabWidget::currentChanged, this, [=](int index) {
//         if (model_cont == tab_w->widget(index)) {
//             m_index_infotable->resetInfo(index_editor_cont->getIndexInfo());
//         } else if (model_tags == tab_w->widget(index)) {
//             m_index_infotable->resetInfo(index_editor_tags->getIndexInfo());
//         } else if (model_type == tab_w->widget(index)) {
//             m_index_infotable->resetInfo(index_editor_type->getIndexInfo());
//         }
//     });

//     {
//         auto docker_index = new QDockWidget(tr("索引编辑"), center_widget);
//         docker_index->setWidget(indexWidget);
//         docker_index->setObjectName("docker_index");
//         docker_index->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
//         docker_index->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
//         addDockWidget(Qt::LeftDockWidgetArea, docker_index);
//     }

//     {
//         auto cate_sel = new CategorySelector(center_widget);
//         auto docker_cate = new QDockWidget(tr("索引编辑 - 大类选择器"), center_widget);
//         docker_cate->setObjectName("docker_cate");
//         docker_cate->setWidget(cate_sel);
//         docker_cate->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
//         docker_cate->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
//         addDockWidget(Qt::TopDockWidgetArea, docker_cate);
//     }

//     {
//         m_logging_widget = new LoggingWidget(center_widget);
//         auto docker_logging = new QDockWidget(tr("索引编辑 - 信息输出"), center_widget);
//         docker_logging->setObjectName("docker_logging");
//         docker_logging->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
//         docker_logging->setWidget(m_logging_widget);
//         docker_logging->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
//         addDockWidget(Qt::RightDockWidgetArea, docker_logging);
//     }
// }

// QWidget *IndexWindow::initModelWidget(IndexEditor *editor)
// {
//     auto modelWidget = new QWidget(centralWidget());

//     auto ly_total = new QGridLayout(modelWidget);

//     auto bt_pull = new QPushButton(tr("↓↓↓拉取数据↓↓↓"), modelWidget);

//     auto bt_push = new QPushButton(tr("↑↑↑推送数据↑↑↑"), modelWidget);

//     auto ly_pp = new QHBoxLayout();
//     ly_pp->addWidget(bt_pull);
//     ly_pp->addWidget(bt_push);

//     auto bt_compress = new QPushButton(tr("←\n←\n←\n压\n缩\n为\n索\n引\n←\n←\n←\n"), modelWidget);
//     {
//         bt_compress->setMaximumWidth(bt_compress->fontMetrics().averageCharWidth() * 4);
//         auto plc_bt_sync = bt_compress->sizePolicy();
//         plc_bt_sync.setVerticalPolicy(QSizePolicy::Expanding);
//         bt_compress->setSizePolicy(plc_bt_sync);
//     }

//     // 不可编辑
//     auto preview_w = new PreviewWidget(editor->getOriginInfo(), 1, 3, PreviewWidget::PreviewType::MODEL, Qt::Vertical, false, true, modelWidget);

//     // editor-modify
//     auto gb_modify = new QGroupBox(tr("索引编辑"), modelWidget);
//     gb_modify->setEnabled(false);

//     auto lb_modify_hint = new QLabel(tr("→更改为→"), gb_modify);
//     auto lb_toModify = new QLabel("\"\"", gb_modify);

//     auto le_modify_input = new QLineEdit(gb_modify);
//     le_modify_input->setPlaceholderText("请输入索引名称, 不能为空");

//     auto ly_modify_hint = new QHBoxLayout();

//     ly_modify_hint->addWidget(lb_toModify, 0);
//     ly_modify_hint->addWidget(lb_modify_hint, 0);
//     ly_modify_hint->addWidget(le_modify_input, 0);
//     ly_modify_hint->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding));

//     auto bt_modify = new QPushButton(tr("更改索引名"), gb_modify);

//     auto ly_gb_modify = new QVBoxLayout(gb_modify);
//     ly_gb_modify->addSpacerItem(
//         new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Expanding));
//     ly_gb_modify->addLayout(ly_modify_hint, 0);
//     ly_gb_modify->addWidget(bt_modify, 0);
//     ly_gb_modify->addSpacerItem(
//         new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Expanding));
//     ly_gb_modify->setStretch(0, 1);
//     ly_gb_modify->setStretch(3, 2);

//     ly_total->addWidget(gb_modify,   1, 0);
//     ly_total->addLayout(ly_pp,       0, 2);
//     ly_total->addWidget(bt_compress, 1, 1);
//     ly_total->addWidget(preview_w,   1, 2);
//     ly_total->setColumnStretch(0, 0);
//     ly_total->setColumnStretch(1, 0);
//     ly_total->setColumnStretch(2, 1);

//     connect(bt_pull, &QPushButton::clicked, this, [=]() {
//         editor->pull_org();
//     });
//     connect(bt_push, &QPushButton::clicked, this, [=]() {
//         editor->push_org();
//     });
//     connect(bt_compress, &QPushButton::clicked, this, [=]() {
//         editor->compressToIndex();
//     });
//     connect(preview_w, &PreviewWidget::onPreview, this, [=](const std::vector<int> &index) {
//         preview_w->previewFiles(editor->getFilePaths(index), editor->getPreviewInfo(index), false);
//     });

//     connect(editor, &IndexEditor::onResponsing, this, [=](const QString & info, bool is_continue){
//         is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
//     });
//     connect(editor, &IndexEditor::onPullSuccessful, this, [=]() {
//         preview_w->refreshInfo();
//         QMessageBox::information(this, tr("数据拉取"), tr("数据拉取成功!"));
//         // 自动压缩
//         bt_compress->click();
//     });
//     connect(editor, &IndexEditor::onPushSuccessful, this, [=]() {
//         preview_w->refreshInfo();
//         QMessageBox::information(this, tr("数据推送"), tr("数据推送成功!"));
//         // 自动拉取
//         bt_pull->click();
//     });
//     connect(editor, &IndexEditor::onCompressed, this, [=]() {
//         m_logging_widget->info(tr("压缩为索引成功"));
//         m_index_infotable->resetInfo(editor->getIndexInfo());
//         QMessageBox::information(this, tr("压缩为索引"), tr("压缩为索引成功!"));
//     });
//     connect(editor, &IndexEditor::onSyncToOrg, this, [=]() {
//         m_logging_widget->info(tr("索引同步成功"));
//         QMessageBox::information(this, tr("索引同步"), tr("索引同步成功!"));
//     });

//     // 更改索引功能
//     connect(this, &IndexWindow::onIndexSelected, this, [=](IndexEditor::IndexType type) {
//         if (type == editor->getType()) {
//             gb_modify->setEnabled(true);
//             lb_toModify->setText(QString().asprintf("\"%s\"", editor->index(m_selected_row).toStdString().c_str()));
//         }
//     });

//     connect(bt_modify, &QPushButton::clicked, this, [=]() {
//         if (m_selected_row == -1) {
//             auto info = tr("未选择要更改的标签, 更改失败! 请<b>点击目录根节点</b>进行选择!");
//             m_logging_widget->error(info);
//             QMessageBox::warning(this, tr("未选择要更改的标签, 更改失败"), info);
//             return;
//         }
//         auto tagName = le_modify_input->text();
//         if (tagName == "") {
//             auto info = tr("标签目录名<b>不能为空</b>, 更改失败!");
//             m_logging_widget->error(info);
//             QMessageBox::warning(this, tr("标签目录名为空, 更改失败"), info);
//             return;
//         }
//         editor->modify(m_selected_row, tagName);
//     });
//     connect(editor, &IndexEditor::onModifyRepeat, this, [=](const QString &scName) {
//         auto info = tr("无法更改为<b>同名标签</b>\" ") + scName + tr(" \", 更改失败!");
//                     m_logging_widget->error(info);
//         QMessageBox::warning(this, tr("无法更改同名标签, 更改失败"), info);
//     });
//     connect(editor, &IndexEditor::onModifySuccessed, this, [=]() {
//         preview_w->clearInfo();
//         preview_w->refreshInfo();
//         le_modify_input->clear();
//         lb_toModify->setText("\"\"");
//         m_selected_row = -1;
//         m_logging_widget->info(tr("标签更改成功，上传数据中..."));
//         QMessageBox::information(this, tr("标签更改"), tr("标签更改成功"));
//         editor->push_org();
//     });


//     return modelWidget;
// }

// void IndexWindow::closeEvent(QCloseEvent *event)
// {
//     LayoutManager::getInstance()->save(this, "IndexWindow");
//     QWidget::closeEvent(event);
// }

// void IndexWindow::showEvent(QShowEvent *event)
// {
//     if (_is_first_paint) {
//         _is_first_paint = false;
//         LayoutManager::getInstance()->restore(this, "IndexWindow");
//     }
//     update();
// }
// #include "scmanagerwindow.h"

// #include <QGroupBox>
// #include <QVBoxLayout>
// #include <QHBoxLayout>
// #include <QLineEdit>
// #include <QMenu>
// #include <QMenuBar>
// #include <QDockWidget>
// #include <QPushButton>
// #include <QMessageBox>
// #include <QLabel>
// #include <QSpacerItem>
// #include <QKeyEvent>
// #include <QShowEvent>
// #include <QCloseEvent>
// #include <QDebug>

// #include "gui/uicomponent/categoryselector.h"
// #include "gui/uicomponent/infotablewidget.h"
// #include "gui/uicomponent/loggingwidget.h"
// #include "function/layoutmanager.h"
// #include "function/scmanager.h"

// SCManagerWindow::SCManagerWindow(QWidget *parent)
//     : IFunctionWindow{tr("三维模型标准分类管理"), {800, 600}, true, false, true, parent}
// {

//     // ----------------视图----------------
//     {
//         auto top_menubar = new QMenuBar(this);
//         auto menu_view = new QMenu(tr("视图(&V)"), this);

//         // ----------------保存布局----------------
//         menu_view->addAction(tr("保存布局"), this, [=]() {
//             LayoutManager::getInstance()->save(this, "SCManagerWindow");
//         });
//         // ----------------加载布局----------------
//         menu_view->addAction(tr("加载布局"), this, [=]() {
//             LayoutManager::getInstance()->restore(this, "SCManagerWindow");
//         });

//         top_menubar->addMenu(menu_view);
//         this->setMenuBar(top_menubar);
//     }

//     auto center_widget = this->centralWidget();
//     auto gridLayout = new QGridLayout(center_widget);


//     m_scManager = new SCManager(this);

//     auto manager_w = initManagerWidget(m_scManager);

//     gridLayout->addWidget(manager_w);

//     initCategoryWidget();
//     initLoggingWidget();

//     connect(m_scManager, &SCManager::onResponsing, this, [=](const QString & info, bool is_continue){
//         is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
//     });

// }

// void SCManagerWindow::closeEvent(QCloseEvent *event)
// {
//     LayoutManager::getInstance()->save(this, "SCManagerWindow");
//     QWidget::closeEvent(event);
// }

// void SCManagerWindow::showEvent(QShowEvent *event)
// {
//     if (_is_first_paint) {
//         _is_first_paint = false;
//         LayoutManager::getInstance()->restore(this, "SCManagerWindow");
//     }
//     update();
// }

// QWidget *SCManagerWindow::initManagerWidget(SCManager *manager)
// {
//     auto total_widget = new QWidget(centralWidget());

//     auto ly_total = new QHBoxLayout(total_widget);

//     // info UI
//     auto ly_info = new QVBoxLayout();
//     auto ly_pull = new QHBoxLayout();

//     auto bt_pull = new QPushButton(tr("↓↓↓获取标准分类↓↓↓"), total_widget);
//     ly_pull->addWidget(bt_pull);

//     ly_info->addLayout(ly_pull);

//     auto infotable = new InfoTableWidget(manager->getSCInfo(), 1, false, true, total_widget);

//     ly_info->addWidget(infotable);
//     ly_total->addLayout(ly_info);

//     // editor UI
//     auto ly_editor = new QVBoxLayout();

//     // editor-add
//     auto gb_add = new QGroupBox(tr("添加标准分类"), total_widget);
//     gb_add->setEnabled(false);
//     auto ly_gb_add = new QHBoxLayout(gb_add);

//     auto le_add_input = new QLineEdit(gb_add);
//     le_add_input->setPlaceholderText("请输入标准分类名称, 不能为空");

//     auto bt_add = new QPushButton(tr("添加"), gb_add);

//     ly_gb_add->addWidget(bt_add);
//     ly_gb_add->addWidget(le_add_input);

//     ly_editor->addWidget(gb_add);

//     // editor-remove
//     auto gb_remove = new QGroupBox(tr("删除标准分类"), total_widget);
//     gb_remove->setEnabled(false);
//     auto ly_gb_remove = new QHBoxLayout(gb_remove);

//     auto bt_remove = new QPushButton(tr("删除"), gb_remove);

//     auto lb_remove_hint = new QLabel(tr("要删除的标准分类: "), gb_remove);
//     auto lb_toRemove = new QLabel("\"\"", gb_remove);

//     ly_gb_remove->addWidget(bt_remove, 0);
//     ly_gb_remove->addWidget(lb_remove_hint, 0);
//     ly_gb_remove->addWidget(lb_toRemove, 0);
//     ly_gb_remove->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding));

//     ly_editor->addWidget(gb_remove);

//     // editor-modify
//     auto gb_modify = new QGroupBox(tr("更改标准分类"), total_widget);
//     gb_modify->setEnabled(false);
//     auto ly_gb_modify = new QVBoxLayout(gb_modify);

//     auto bt_modify = new QPushButton(tr("更改"), gb_modify);

//     auto lb_modify_hint = new QLabel(tr("→更改为→"), gb_modify);
//     auto lb_toModify = new QLabel("\"\"", gb_modify);

//     auto le_modify_input = new QLineEdit(gb_modify);
//     le_modify_input->setPlaceholderText("请输入标准分类名称, 不能为空");

//     auto ly_modify_hint = new QHBoxLayout();

//     ly_modify_hint->addWidget(lb_toModify, 0);
//     ly_modify_hint->addWidget(lb_modify_hint, 0);
//     ly_modify_hint->addWidget(le_modify_input, 0);
//     ly_modify_hint->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding));

//     ly_gb_modify->addLayout(ly_modify_hint, 0);

//     ly_gb_modify->addWidget(bt_modify, 0);

//     ly_editor->addWidget(gb_modify);

//     ly_total->addLayout(ly_editor);

//     // 信息表选中功能
//     connect(infotable, &InfoTableWidget::itemDoubleClicked, this, [=](QTableWidgetItem *item) {
//         auto row = item->row();
//         if (row < 0) {
//             qDebug() << "SCManagerWindow>>InfoTableWidget::onGroupSelected>> item < 0";
//             return;
//         }
//         m_selected_row = row;
//         lb_toRemove->setText(QString().asprintf("\"%s\"", manager->sc(row).toStdString().c_str()));
//         lb_toModify->setText(QString().asprintf("\"%s\"", manager->sc(row).toStdString().c_str()));
//     });

//     // 拉取功能
//     connect(bt_pull, &QPushButton::clicked, this, [=](){
//         // 注意, 由于pull会覆盖掉现有表格, 原本选择的是无意义的, 故此清除选择状态
//         lb_toRemove->setText("\"\"");
//         lb_toModify->setText("\"\"");
//         m_selected_row  = -1;
//         // 拉取
//         manager->pull();
//     });
//     connect(manager, &SCManager::onPullSuccessful, this, [=]() {
//         infotable->refresh();
//         gb_add->setEnabled(true);
//         gb_remove->setEnabled(true);
//         gb_modify->setEnabled(true);
//     });

//     // 添加功能
//     connect(bt_add, &QPushButton::clicked, this, [=]() {
//         auto scName = le_add_input->text();
//         if (scName == "") {
//             auto info = tr("标准分类名<b>不能为空</b>, 添加失败!");
//             m_logging_widget->error(info);
//             QMessageBox::warning(this, tr("标准分类名为空, 添加失败"), info);
//             return;
//         }
//         manager->add(scName);
//     });
//     connect(manager, &SCManager::onAddRepeat, this, [=](const QString &scName) {
//         auto info = tr("标准分类名\" ") + scName + tr(" \"<b>重复</b>, 添加失败!");
//         m_logging_widget->error(info);
//         QMessageBox::warning(this, tr("标准分类名重复, 添加失败"), info);
//     });
//     connect(manager, &SCManager::onAddSuccessed, this, [=](){
//         infotable->refresh();
//         le_add_input->clear();
//         m_logging_widget->info(tr("标准分类添加成功，上传数据中..."));
//         QMessageBox::warning(this, tr("标准分类添加"), tr("标准分类添加成功"));
//         manager->push();
//     });

//     // 删除功能
//     connect(bt_remove, &QPushButton::clicked, this, [=]() {
//         int selected_row = -1;
//         selected_row = m_selected_row;
//         if (selected_row == -1) {
//             auto info = tr("未选择要删除的标准分类, 删除失败! 请<b>双击</b>标准分类进行选择!");
//             m_logging_widget->error(info);
//             QMessageBox::warning(this, tr("未选择要更改的标准分类, 删除失败"), info);
//             return;
//         }
//         manager->remove(selected_row);
//     });
//     connect(manager, &SCManager::onRemoveSuccessed, this, [=]() {
//         infotable->refresh();
//         lb_toRemove->setText("\"\"");
//         lb_toModify->setText("\"\"");
//         m_selected_row = -1;
//         m_logging_widget->info(tr("标准分类删除成功，上传数据中..."));
//         QMessageBox::warning(this, tr("标准分类删除"), tr("标准分类删除成功"));
//         manager->push();
//     });

//     // 更改功能
//     connect(bt_modify, &QPushButton::clicked, this, [=]() {
//         int selected_row = -1;
//         selected_row = m_selected_row;
//         if (selected_row == -1) {
//             auto info = tr("未选择要更改的标准分类, 更改失败! 请<b>双击</b>标准分类进行选择!");
//             m_logging_widget->error(info);
//             QMessageBox::warning(this, tr("未选择要更改的标准分类, 更改失败"), info);
//             return;
//         }
//         auto scName = le_modify_input->text();
//         if (scName == "") {
//             auto info = tr("标准分类名<b>不能为空</b>, 更改失败!");
//             m_logging_widget->error(info);
//             QMessageBox::warning(this, tr("标准分类名为空, 更改失败"), info);
//             return;
//         }
//         manager->modify(selected_row, scName);
//     });
//     connect(manager, &SCManager::onModifyRepeat, this, [=](const QString &scName) {
//         auto info = tr("无法更改为<b>同名标准分类</b>\" ") + scName + tr(" \", 更改失败!");
//         m_logging_widget->error(info);
//         QMessageBox::warning(this, tr("无法更改同名标准分类, 更改失败"), info);
//     });
//     connect(manager, &SCManager::onModifySuccessed, this, [=]() {
//         infotable->refresh();
//         le_modify_input->clear();
//         lb_toRemove->setText("\"\"");
//         lb_toModify->setText("\"\"");
//         m_selected_row = -1;
//         m_logging_widget->info(tr("标准分类更改成功，上传数据中..."));
//         QMessageBox::warning(this, tr("标准分类更改"), tr("标准分类更改成功"));
//         manager->push();
//     });

//     // 推送功能
//     connect(manager, &SCManager::onPushSuccessful, this, [=]() {
//         m_logging_widget->info(tr("数据上传成功"));
//     });

//     bt_pull->click();

//     return total_widget;
// }

// void SCManagerWindow::initCategoryWidget()
// {
//     auto center_widget = this->centralWidget();
//     auto cate_sel = new CategorySelector(center_widget);
//     auto docker_cate = new QDockWidget(tr("标准分类管理 - 大类选择器"), center_widget);
//     docker_cate->setObjectName("docker_cate");
//     docker_cate->setWidget(cate_sel);
//     docker_cate->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
//     docker_cate->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
//     addDockWidget(Qt::TopDockWidgetArea, docker_cate);
// }

// void SCManagerWindow::initLoggingWidget()
// {
//     auto center_widget = this->centralWidget();
//     m_logging_widget = new LoggingWidget(center_widget);
//     auto docker_logging = new QDockWidget(tr("标准分类管理 - 信息输出"), center_widget);
//     docker_logging->setObjectName("docker_logging");
//     docker_logging->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
//     docker_logging->setWidget(m_logging_widget);
//     docker_logging->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
//     addDockWidget(Qt::RightDockWidgetArea, docker_logging);
// }
// #include "tagsmanagerwindow.h"

// #include <QGridLayout>
// #include <QGroupBox>
// #include <QHBoxLayout>
// #include <QHeaderView>
// #include <QLineEdit>
// #include <QMainWindow>
// #include <QMenu>
// #include <QMenuBar>
// #include <QDockWidget>
// #include <QPushButton>
// #include <QTabWidget>
// #include <QVBoxLayout>
// #include <QMessageBox>
// #include <QLabel>
// #include <QSpacerItem>
// #include <QKeyEvent>
// #include <QShowEvent>
// #include <QCloseEvent>

// #include "gui/uicomponent/infotablewidget.h"
// #include "gui/uicomponent/loggingwidget.h"
// #include "gui/uicomponent/categoryselector.h"
// #include "function/tagsmanager.h"
// #include "function/layoutmanager.h"

// TagsManagerWindow::TagsManagerWindow(QWidget *parent)
//     : IFunctionWindow("", parent ? parent->size() / 2 : QSize{1000, 900}, true, false, false, parent)
// {
//     this->setWindowTitle("标签管理");
//     this->setMinimumSize({1000, 900});

//     auto center_widget = this->centralWidget();
//     auto gridLayout = new QGridLayout(center_widget);
//     auto tabWidget = new QTabWidget(center_widget);
//     gridLayout->addWidget(tabWidget);

//     m_modelManager  = new TagsManager(TagsManager::AssetType::MODEL,  this);
//     m_bvhManager    = new TagsManager(TagsManager::AssetType::BVH,    this);
//     m_effectManager = new TagsManager(TagsManager::AssetType::EFFECT, this);

//     tabWidget->addTab(initManagerWidget(m_modelManager), tr("模型标签管理"));
//     tabWidget->addTab(initManagerWidget(m_bvhManager), tr("骨骼动画标签管理"));
//     tabWidget->addTab(initManagerWidget(m_effectManager), tr("特效资源标签管理"));

//     initCategoryWidget();
//     initLoggingWidget();

//     connect(m_modelManager, &TagsManager::onResponsing, this, [=](const QString & info, bool is_continue){
//         is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
//     });
//     connect(m_bvhManager, &TagsManager::onResponsing, this, [=](const QString & info, bool is_continue){
//         is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
//     });
//     connect(m_effectManager, &TagsManager::onResponsing, this, [=](const QString & info, bool is_continue){
//         is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
//     });

// }

// TagsManagerWindow::~TagsManagerWindow()
// {
// }

// void TagsManagerWindow::keyPressEvent(QKeyEvent *ev)
// {
//     // TODO: 方便调试
//     if(ev->key() == Qt::Key::Key_Escape) {
//         close();
//     }
// }

// void TagsManagerWindow::closeEvent(QCloseEvent *event)
// {
//     LayoutManager::getInstance()->save(this, "TagsManagerWindow");
//     QWidget::closeEvent(event);
// }

// void TagsManagerWindow::showEvent(QShowEvent *event)
// {
//     if (_is_first_paint) {
//         _is_first_paint = false;
//         LayoutManager::getInstance()->restore(this, "TagsManagerWindow");
//     }
//     update();
// }

// QWidget *TagsManagerWindow::initManagerWidget(TagsManager *manager)
// {
//     auto tab = new QWidget();

//     auto ly_tab_info = new QHBoxLayout(tab);

//     // info
//     auto ly_info = new QVBoxLayout();
//     auto ly_pullpush = new QHBoxLayout();

//     auto bt_pull = new QPushButton(tr("↓↓↓拉取标签↓↓↓"), tab);
//     ly_pullpush->addWidget(bt_pull);

//     auto bt_push = new QPushButton(tr("↑↑↑推送标签↑↑↑"), tab);
//     ly_pullpush->addWidget(bt_push);
//     bt_push->setEnabled(false);

//     ly_info->addLayout(ly_pullpush);

//     auto infotable = new InfoTableWidget(manager->getTags(), 1, false, true, tab);

//     ly_info->addWidget(infotable);
//     ly_tab_info->addLayout(ly_info);

//     // editor
//     auto ly_editor = new QVBoxLayout();
//     auto gb_add = new QGroupBox(tr("添加标签"), tab);
//     auto ly_gb_add = new QHBoxLayout(gb_add);

//     auto le_input = new QLineEdit(gb_add);
//     le_input->setPlaceholderText("请输入标签名称, 不能为空");
//     le_input->setEnabled(false);

//     auto bt_add = new QPushButton(tr("添加"), gb_add);
//     bt_add->setEnabled(false);

//     ly_gb_add->addWidget(bt_add);
//     ly_gb_add->addWidget(le_input);

//     ly_editor->addWidget(gb_add);

//     auto gb_remove = new QGroupBox(tr("删除标签"), tab);
//     auto ly_gb_remove = new QHBoxLayout(gb_remove);

//     auto bt_remove = new QPushButton(tr("删除"), gb_remove);
//     bt_remove->setEnabled(false);

//     auto lb_remove_hint = new QLabel(tr("要删除的标签: "), gb_remove);
//     auto lb_toRemove = new QLabel("", gb_remove);

//     ly_gb_remove->addWidget(bt_remove, 0);
//     ly_gb_remove->addWidget(lb_remove_hint, 0);
//     ly_gb_remove->addWidget(lb_toRemove, 0);
//     ly_gb_remove->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding));

//     ly_editor->addWidget(gb_remove);

//     ly_tab_info->addLayout(ly_editor);

//     // 标签信息表功能
//     connect(infotable, &InfoTableWidget::itemDoubleClicked, this, [=](QTableWidgetItem *item) {
//         auto row = item->row();
//         if (row < 0) {
//             qDebug() << "TypeManagerWindow>>InfoTableWidget::onGroupSelected>> item < 0";
//             return;
//         }
//         switch (manager->getType()) {
//         case TagsManager::AssetType::MODEL:   m_selected_row_model  = row; break;
//         case TagsManager::AssetType::BVH:     m_selected_row_bvh    = row; break;
//         case TagsManager::AssetType::EFFECT:  m_selected_row_effect = row; break;
//         }
//         lb_toRemove->setText(manager->tags(row));
//     });

//     // 拉取功能
//     connect(bt_pull, &QPushButton::clicked, this, [=](){
//         // 注意, 由于pull会覆盖掉现有表格, 原本选择的标签是无意义的, 故此清除选择状态
//         lb_toRemove->setText("");
//         switch (manager->getType()) {
//         case TagsManager::AssetType::MODEL:   m_selected_row_model = -1;  break;
//         case TagsManager::AssetType::BVH:     m_selected_row_bvh = -1;    break;
//         case TagsManager::AssetType::EFFECT:  m_selected_row_effect = -1; break;
//         }
//         // 拉取
//         manager->pull();
//     });
//     connect(manager, &TagsManager::onPullSuccessful, this, [=]() {
//         infotable->refresh();
//         bt_push->setEnabled(true);
//         bt_add->setEnabled(true);
//         bt_remove->setEnabled(true);
//         le_input->setEnabled(true);
//     });

//     // 推送功能
//     connect(bt_push, &QPushButton::clicked, this, [=]() {
//         manager->push();
//     });
//     connect(manager, &TagsManager::onPushSuccessful, this, [=]() {
//         bt_push->setEnabled(false);
//     });

//     // 添加标签功能
//     connect(bt_add, &QPushButton::clicked, this, [=]() {
//         auto typeName = le_input->text();
//         if (typeName == "") {
//             auto info = tr("标签名不能为空, 添加失败!");
//             m_logging_widget->error(info);
//             QMessageBox::warning(this, tr("标签添加失败"), info);
//             return;
//         }
//         manager->add(typeName);
//     });
//     connect(manager, &TagsManager::onAddRepeat, this, [=](const QString &typeName) {
//         auto info = tr("标签名\" ") + typeName + tr(" \"重复, 添加失败!");
//         m_logging_widget->error(info);
//         QMessageBox::warning(this, tr("标签添加失败"), info);
//     });
//     connect(manager, &TagsManager::onAddSuccessed, this, [=](){
//         infotable->refresh();
//         le_input->clear();
//         m_logging_widget->info(tr("标签添加成功!"));
//     });

//     // 删除标签功能
//     connect(bt_remove, &QPushButton::clicked, this, [=]() {
//         int selected_row = -1;
//         switch (manager->getType()) {
//         case TagsManager::AssetType::MODEL:   selected_row = m_selected_row_model;  break;
//         case TagsManager::AssetType::BVH:     selected_row = m_selected_row_bvh;    break;
//         case TagsManager::AssetType::EFFECT:  selected_row = m_selected_row_effect; break;
//         }
//         if (selected_row == -1) {
//             auto info = tr("未选择要删除的标签, 删除失败! 请双击标签进行选择!");
//             m_logging_widget->error(info);
//             QMessageBox::warning(this, tr("标签删除失败"), info);
//             return;
//         }
//         manager->remove(selected_row);
//     });
//     connect(manager, &TagsManager::onRemoveSuccessed, this, [=]() {
//         infotable->refresh();
//         lb_toRemove->setText("");
//         switch (manager->getType()) {
//         case TagsManager::AssetType::MODEL:   m_selected_row_model = -1;  break;
//         case TagsManager::AssetType::BVH:     m_selected_row_bvh = -1;    break;
//         case TagsManager::AssetType::EFFECT:  m_selected_row_effect = -1; break;
//         }
//         m_logging_widget->info(tr("标签删除成功!"));
//     });

//     return tab;
// }

// void TagsManagerWindow::initCategoryWidget()
// {
//     auto center_widget = this->centralWidget();
//     auto cate_sel = new CategorySelector(center_widget);
//     auto docker_cate = new QDockWidget(tr("标签管理 - 大类选择器"), center_widget);
//     docker_cate->setObjectName("docker_cate");
//     docker_cate->setWidget(cate_sel);
//     docker_cate->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
//     docker_cate->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
//     addDockWidget(Qt::TopDockWidgetArea, docker_cate);

// }

// void TagsManagerWindow::initLoggingWidget()
// {
//     auto center_widget = this->centralWidget();
//     m_logging_widget = new LoggingWidget(center_widget);
//     auto docker_logging = new QDockWidget(tr("标签管理 - 信息输出"), center_widget);
//     docker_logging->setObjectName("docker_logging");
//     docker_logging->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
//     docker_logging->setWidget(m_logging_widget);
//     docker_logging->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
//     addDockWidget(Qt::RightDockWidgetArea, docker_logging);
// }
// #include "typemanagerwindow.h"

// #include <QGridLayout>
// #include <QGroupBox>
// #include <QHBoxLayout>
// #include <QHeaderView>
// #include <QLineEdit>
// #include <QMainWindow>
// #include <QMenu>
// #include <QMenuBar>
// #include <QDockWidget>
// #include <QPushButton>
// #include <QTabWidget>
// #include <QVBoxLayout>
// #include <QMessageBox>
// #include <QLabel>
// #include <QSpacerItem>
// #include <QKeyEvent>
// #include <QShowEvent>
// #include <QCloseEvent>

// #include "gui/uicomponent/infotablewidget.h"
// #include "gui/uicomponent/loggingwidget.h"
// #include "gui/uicomponent/categoryselector.h"
// #include "function/typemanager.h"
// #include "function/layoutmanager.h"

// TypeManagerWindow::TypeManagerWindow(QWidget *parent)
//     : IFunctionWindow("", parent ? parent->size() / 2 : QSize{1000, 900}, true, false, false, parent)
// {
//     this->setWindowTitle("类型管理");
//     this->setMinimumSize({1000, 900});

//     auto center_widget = this->centralWidget();
//     auto gridLayout = new QGridLayout(center_widget);
//     auto tabWidget = new QTabWidget(center_widget);
//     gridLayout->addWidget(tabWidget);

//     m_modelManager  = new TypeManager(TypeManager::AssetType::MODEL,  this);
//     m_bvhManager    = new TypeManager(TypeManager::AssetType::BVH,    this);
//     m_effectManager = new TypeManager(TypeManager::AssetType::EFFECT, this);

//     tabWidget->addTab(initManagerWidget(m_modelManager), tr("模型类型管理"));
//     tabWidget->addTab(initManagerWidget(m_bvhManager), tr("骨骼动画类型管理"));
//     tabWidget->addTab(initManagerWidget(m_effectManager), tr("特效资源类型管理"));

//     initCategoryWidget();
//     initLoggingWidget();

//     connect(m_modelManager, &TypeManager::onResponsing, this, [=](const QString & info, bool is_continue){
//         is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
//     });
//     connect(m_bvhManager, &TypeManager::onResponsing, this, [=](const QString & info, bool is_continue){
//         is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
//     });
//     connect(m_effectManager, &TypeManager::onResponsing, this, [=](const QString & info, bool is_continue){
//         is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
//     });

// }

// TypeManagerWindow::~TypeManagerWindow()
// {
// }

// void TypeManagerWindow::keyPressEvent(QKeyEvent *ev)
// {
//     // TODO: 方便调试
//     if(ev->key() == Qt::Key::Key_Escape) {
//         close();
//     }
// }

// void TypeManagerWindow::closeEvent(QCloseEvent *event)
// {
//     LayoutManager::getInstance()->save(this, "TypeManagerWindow");
//     QWidget::closeEvent(event);
// }

// void TypeManagerWindow::showEvent(QShowEvent *event)
// {
//     if (_is_first_paint) {
//         _is_first_paint = false;
//         LayoutManager::getInstance()->restore(this, "TypeManagerWindow");
//     }
//     update();
// }

// QWidget *TypeManagerWindow::initManagerWidget(TypeManager *manager)
// {
//     auto tab = new QWidget();

//     auto ly_tab_info = new QHBoxLayout(tab);

//     // info
//     auto ly_info = new QVBoxLayout();
//     auto ly_pullpush = new QHBoxLayout();

//     auto bt_pull = new QPushButton(tr("↓↓↓拉取类型↓↓↓"), tab);
//     ly_pullpush->addWidget(bt_pull);

//     auto bt_push = new QPushButton(tr("↑↑↑推送类型↑↑↑"), tab);
//     ly_pullpush->addWidget(bt_push);
//     bt_push->setEnabled(false);

//     ly_info->addLayout(ly_pullpush);

//     auto infotable = new InfoTableWidget(manager->getTypes(), 1, false, true, tab);

//     ly_info->addWidget(infotable);
//     ly_tab_info->addLayout(ly_info);

//     // editor
//     auto ly_editor = new QVBoxLayout();
//     auto gb_add = new QGroupBox(tr("添加类型"), tab);
//     auto ly_gb_add = new QHBoxLayout(gb_add);

//     auto le_input = new QLineEdit(gb_add);
//     le_input->setPlaceholderText("请输入类型名称, 不能为空");
//     le_input->setEnabled(false);

//     auto bt_add = new QPushButton(tr("添加"), gb_add);
//     bt_add->setEnabled(false);

//     ly_gb_add->addWidget(bt_add);
//     ly_gb_add->addWidget(le_input);

//     ly_editor->addWidget(gb_add);

//     auto gb_remove = new QGroupBox(tr("删除类型"), tab);
//     auto ly_gb_remove = new QHBoxLayout(gb_remove);

//     auto bt_remove = new QPushButton(tr("删除"), gb_remove);
//     bt_remove->setEnabled(false);

//     auto lb_remove_hint = new QLabel(tr("要删除的类型: "), gb_remove);
//     auto lb_toRemove = new QLabel("", gb_remove);

//     ly_gb_remove->addWidget(bt_remove, 0);
//     ly_gb_remove->addWidget(lb_remove_hint, 0);
//     ly_gb_remove->addWidget(lb_toRemove, 0);
//     ly_gb_remove->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding));

//     ly_editor->addWidget(gb_remove);

//     ly_tab_info->addLayout(ly_editor);

//     // 类型信息表功能
//     connect(infotable, &InfoTableWidget::itemDoubleClicked, this, [=](QTableWidgetItem *item) {
//         auto row = item->row();
//         if (row < 0) {
//             qDebug() << "TypeManagerWindow>>InfoTableWidget::onGroupSelected>> item < 0";
//             return;
//         }
//         switch (manager->getType()) {
//         case TypeManager::AssetType::MODEL:   m_selected_row_model  = row; break;
//         case TypeManager::AssetType::BVH:     m_selected_row_bvh    = row; break;
//         case TypeManager::AssetType::EFFECT:  m_selected_row_effect = row; break;
//         }
//         lb_toRemove->setText(manager->type(row));
//     });

//     // 拉取功能
//     connect(bt_pull, &QPushButton::clicked, this, [=](){
//         // 注意, 由于pull会覆盖掉现有表格, 原本选择的类型是无意义的, 故此清除选择状态
//         lb_toRemove->setText("");
//         switch (manager->getType()) {
//         case TypeManager::AssetType::MODEL:   m_selected_row_model = -1;  break;
//         case TypeManager::AssetType::BVH:     m_selected_row_bvh = -1;    break;
//         case TypeManager::AssetType::EFFECT:  m_selected_row_effect = -1; break;
//         }
//         // 拉取
//         manager->pull();
//     });
//     connect(manager, &TypeManager::onPullSuccessful, this, [=]() {
//         infotable->refresh();
//         bt_push->setEnabled(true);
//         bt_add->setEnabled(true);
//         bt_remove->setEnabled(true);
//         le_input->setEnabled(true);
//     });

//     // 推送功能
//     connect(bt_push, &QPushButton::clicked, this, [=]() {
//         manager->push();
//     });
//     connect(manager, &TypeManager::onPushSuccessful, this, [=]() {
//         bt_push->setEnabled(false);
//     });

//     // 添加类型功能
//     connect(bt_add, &QPushButton::clicked, this, [=]() {
//         auto typeName = le_input->text();
//         if (typeName == "") {
//             auto info = tr("类型名不能为空, 添加失败!");
//             m_logging_widget->error(info);
//             QMessageBox::warning(this, tr("类型添加失败"), info);
//             return;
//         }
//         manager->add(typeName);
//     });
//     connect(manager, &TypeManager::onAddRepeat, this, [=](const QString &typeName) {
//         auto info = tr("类型名\" ") + typeName + tr(" \"重复, 添加失败!");
//         m_logging_widget->error(info);
//         QMessageBox::warning(this, tr("类型添加失败"), info);
//     });
//     connect(manager, &TypeManager::onAddSuccessed, this, [=](){
//         infotable->refresh();
//         le_input->clear();
//         m_logging_widget->info(tr("类型添加成功!"));
//     });

//     // 删除类型功能
//     connect(bt_remove, &QPushButton::clicked, this, [=]() {
//         int selected_row = -1;
//         switch (manager->getType()) {
//         case TypeManager::AssetType::MODEL:   selected_row = m_selected_row_model;  break;
//         case TypeManager::AssetType::BVH:     selected_row = m_selected_row_bvh;    break;
//         case TypeManager::AssetType::EFFECT:  selected_row = m_selected_row_effect; break;
//         }
//         if (selected_row == -1) {
//             auto info = tr("未选择要删除的类型, 删除失败! 请双击类型进行选择!");
//             m_logging_widget->error(info);
//             QMessageBox::warning(this, tr("类型删除失败"), info);
//             return;
//         }
//         manager->remove(selected_row);
//     });
//     connect(manager, &TypeManager::onRemoveSuccessed, this, [=]() {
//         infotable->refresh();
//         lb_toRemove->setText("");
//         switch (manager->getType()) {
//         case TypeManager::AssetType::MODEL:   m_selected_row_model = -1;  break;
//         case TypeManager::AssetType::BVH:     m_selected_row_bvh = -1;    break;
//         case TypeManager::AssetType::EFFECT:  m_selected_row_effect = -1; break;
//         }
//         m_logging_widget->info(tr("类型删除成功!"));
//     });

//     return tab;
// }

// void TypeManagerWindow::initCategoryWidget()
// {
//     auto center_widget = this->centralWidget();
//     auto cate_sel = new CategorySelector(center_widget);
//     auto docker_cate = new QDockWidget(tr("类型管理 - 大类选择器"), center_widget);
//     docker_cate->setObjectName("docker_cate");
//     docker_cate->setWidget(cate_sel);
//     docker_cate->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
//     docker_cate->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
//     addDockWidget(Qt::TopDockWidgetArea, docker_cate);

// }

// void TypeManagerWindow::initLoggingWidget()
// {
//     auto center_widget = this->centralWidget();
//     m_logging_widget = new LoggingWidget(center_widget);
//     auto docker_logging = new QDockWidget(tr("类型管理 - 信息输出"), center_widget);
//     docker_logging->setObjectName("docker_logging");
//     docker_logging->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
//     docker_logging->setWidget(m_logging_widget);
//     docker_logging->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
//     addDockWidget(Qt::RightDockWidgetArea, docker_logging);
// }
// #include "directorywindow.h"

// #include <QDockWidget>
// #include <QTabWidget>
// #include <QGridLayout>
// #include <QHBoxLayout>
// #include <QVBoxLayout>
// #include <QPushButton>
// #include <QSpacerItem>
// #include <QShowEvent>
// #include <QCloseEvent>
// #include <QMenuBar>
// #include <QMenu>
// #include <QMessageBox>
// #include <QGroupBox>
// #include <QLabel>
// #include <QLineEdit>

// #include "gui/uicomponent/categoryselector.h"
// #include "gui/uicomponent/infotreewidget.h"
// #include "gui/uicomponent/infotablewidget.h"
// #include "gui/uicomponent/previewwidget.h"
// #include "gui/uicomponent/loggingwidget.h"
// #include "function/layoutmanager.h"
// #include "function/indexeditor.h"

// ////////////////////////
// //// DirectoryWindow
// ////////////////////////

// DirectoryWindow::DirectoryWindow(const QString &title, QWidget *parent)
//     : IFunctionWindow{title, {800, 600}, true, false, true, parent}
// {

// }

// ////////////////////////
// //// SCDirWindow
// ////////////////////////

// SCDirWindow::SCDirWindow(QWidget *parent)
//     : DirectoryWindow{tr("三维模型标准分类目录管理"), parent}
// {

//     // ----------------视图----------------
//     {
//         auto top_menubar = new QMenuBar(this);
//         auto menu_view = new QMenu(tr("视图(&V)"), this);

//         // ----------------保存布局----------------
//         menu_view->addAction(tr("保存布局"), this, [=]() {
//             LayoutManager::getInstance()->save(this, "SCDirWindow");
//         });
//         // ----------------加载布局----------------
//         menu_view->addAction(tr("加载布局"), this, [=]() {
//             LayoutManager::getInstance()->restore(this, "SCDirWindow");
//         });

//         top_menubar->addMenu(menu_view);
//         this->setMenuBar(top_menubar);
//     }

//     auto editor = new IndexEditor(IndexEditor::IndexType::CONTENT, this);

//     auto center_widget = centralWidget();

//     auto ly_total = new QGridLayout(center_widget);

//     auto dirWidget = new QWidget(center_widget);

//     auto ly_dir = new QHBoxLayout(dirWidget);

//     m_dir_tree = new InfoTreeWidget(editor->getIndexInfo(),
//                                     editor->getOriginInfo(),
//                                     true,
//                                     dirWidget);

//     ly_dir->addWidget(m_dir_tree, 1);

//     auto modelWidget = initModelWidget(editor);
//     ly_total->addWidget(modelWidget);

//     auto bt_sync = new QPushButton(tr("→\n→\n→\n同\n步\n修\n改\n到\n原\n始\n数\n据\n→\n→\n→\n"),center_widget);
//     {
//         bt_sync->setMaximumWidth(bt_sync->fontMetrics().averageCharWidth() * 4);
//         auto plc_bt_sync = bt_sync->sizePolicy();
//         plc_bt_sync.setVerticalPolicy(QSizePolicy::Expanding);
//         bt_sync->setSizePolicy(plc_bt_sync);
//     }

//     ly_dir->addWidget(bt_sync,  0);

//     connect(bt_sync, &QPushButton::clicked, this, [=]() {
//         editor->syncToOrg();
//     });

//     {
//         auto docker_index = new QDockWidget(tr("标签目录编辑"), center_widget);
//         docker_index->setWidget(dirWidget);
//         docker_index->setObjectName("docker_index");
//         docker_index->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
//         docker_index->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
//         addDockWidget(Qt::LeftDockWidgetArea, docker_index);
//     }

//     {
//         auto cate_sel = new CategorySelector(center_widget);
//         auto docker_cate = new QDockWidget(tr("索引编辑 - 大类选择器"), center_widget);
//         docker_cate->setObjectName("docker_cate");
//         docker_cate->setWidget(cate_sel);
//         docker_cate->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
//         docker_cate->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
//         addDockWidget(Qt::TopDockWidgetArea, docker_cate);
//     }

//     {
//         m_logging_widget = new LoggingWidget(center_widget);
//         auto docker_logging = new QDockWidget(tr("索引编辑 - 信息输出"), center_widget);
//         docker_logging->setObjectName("docker_logging");
//         docker_logging->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
//         docker_logging->setWidget(m_logging_widget);
//         docker_logging->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
//         addDockWidget(Qt::RightDockWidgetArea, docker_logging);
//     }
// }

// void SCDirWindow::closeEvent(QCloseEvent *event)
// {
//     LayoutManager::getInstance()->save(this, "SCDirWindow");
//     QWidget::closeEvent(event);
// }

// void SCDirWindow::showEvent(QShowEvent *event)
// {
//     if (_is_first_paint) {
//         _is_first_paint = false;
//         LayoutManager::getInstance()->restore(this, "SCDirWindow");
//     }
//     update();
// }

// QWidget *SCDirWindow::initModelWidget(IndexEditor *editor)
// {
//     auto modelWidget = new QWidget(centralWidget());

//     auto ly_total = new QGridLayout(modelWidget);

//     auto bt_pull = new QPushButton(tr("↓↓↓拉取数据↓↓↓"), modelWidget);

// //    auto bt_push = new QPushButton(tr("↑↑↑推送数据↑↑↑"), modelWidget);

//     auto ly_pp = new QHBoxLayout();
//     ly_pp->addWidget(bt_pull);
// //    ly_pp->addWidget(bt_push);

//     auto bt_compress = new QPushButton(tr("←\n←\n←\n创\n建\n标\n准\n分\n类\n目\n录\n←\n←\n←\n"), modelWidget);
//     {
//         bt_compress->setMaximumWidth(bt_compress->fontMetrics().averageCharWidth() * 4);
//         auto plc_bt_sync = bt_compress->sizePolicy();
//         plc_bt_sync.setVerticalPolicy(QSizePolicy::Expanding);
//         bt_compress->setSizePolicy(plc_bt_sync);
//     }

//     auto preview_w = new PreviewWidget(editor->getOriginInfo(), 1, 3, PreviewWidget::PreviewType::MODEL, Qt::Vertical, true, true, modelWidget);

//     // editor-modify
//     auto gb_modify = new QGroupBox(tr("标准分类目录编辑"), modelWidget);
//     gb_modify->setEnabled(false);

//     auto lb_modify_hint = new QLabel(tr("→更改为→"), gb_modify);
//     auto lb_toModify = new QLabel("\"\"", gb_modify);

//     auto le_modify_input = new QLineEdit(gb_modify);
//     le_modify_input->setPlaceholderText("请输入标准分类名称, 不能为空");

//     auto ly_modify_hint = new QHBoxLayout();

//     ly_modify_hint->addWidget(lb_toModify, 0);
//     ly_modify_hint->addWidget(lb_modify_hint, 0);
//     ly_modify_hint->addWidget(le_modify_input, 0);
//     ly_modify_hint->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding));

//     auto bt_modify = new QPushButton(tr("更改标准分类名"), gb_modify);

//     auto ly_gb_modify = new QVBoxLayout(gb_modify);
//     ly_gb_modify->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Expanding));
//     ly_gb_modify->addLayout(ly_modify_hint, 0);
//     ly_gb_modify->addWidget(bt_modify, 0);
//     ly_gb_modify->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Expanding));
//     ly_gb_modify->setStretch(0, 1);
//     ly_gb_modify->setStretch(3, 2);

//     ly_total->addWidget(gb_modify,   1, 0);
//     ly_total->addLayout(ly_pp,       0, 2);
//     ly_total->addWidget(bt_compress, 1, 1);
//     ly_total->addWidget(preview_w,   1, 2);
//     ly_total->setColumnStretch(0, 0);
//     ly_total->setColumnStretch(1, 0);
//     ly_total->setColumnStretch(2, 1);

//     connect(bt_pull, &QPushButton::clicked, this, [=]() {
//         editor->pull_org();
//     });
// //    connect(bt_push, &QPushButton::clicked, this, [=]() {
// //        editor->push_org();
// //    });
//     connect(bt_compress, &QPushButton::clicked, this, [=]() {
//         editor->compressToIndex();
//     });
//     connect(preview_w, &PreviewWidget::onPreview, this, [=](const std::vector<int> &index) {
//         preview_w->previewFiles(editor->getFilePaths(index), editor->getPreviewInfo(index), false);
//     });

//     connect(editor, &IndexEditor::onResponsing, this, [=](const QString & info, bool is_continue){
//         is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
//     });
//     connect(editor, &IndexEditor::onPullSuccessful, this, [=]() {
//         preview_w->refreshInfo();
// //        QMessageBox::information(this, tr("数据拉取"), tr("数据拉取成功!"));
//         // 自动压缩
//         bt_compress->click();
//     });
//     connect(editor, &IndexEditor::onPushSuccessful, this, [=]() {
//         preview_w->refreshInfo();
//         QMessageBox::information(this, tr("数据推送"), tr("数据推送成功!"));
//         // 自动更新
//         bt_pull->click();
//     });
//     connect(editor, &IndexEditor::onCompressed, this, [=]() {
//         m_logging_widget->info(tr("创建标准分类目录成功"));
//         m_dir_tree->clearTree();
//         m_dir_tree->refreshTree();
//         QMessageBox::information(this, tr("创建标准分类目录"), tr("创建标准分类目录成功!"));
//     });
//     connect(editor, &IndexEditor::onSyncToOrg, this, [=]() {
//         m_logging_widget->info(tr("标准分类目录同步成功"));
//         QMessageBox::information(this, tr("标准分类目录同步"), tr("标准分类目录同步成功!"));
//     });

//     // 更改标签功能
//     connect(m_dir_tree, &InfoTreeWidget::onRootSelected, this, [=](int row) {
//         m_selected_row = row;
//         gb_modify->setEnabled(true);
//         lb_toModify->setText(QString().asprintf("\"%s\"", editor->index(row).toStdString().c_str()));
//     });

//     // 更改功能
//     connect(bt_modify, &QPushButton::clicked, this, [=]() {
//         if (m_selected_row == -1) {
//             auto info = tr("未选择要更改的标准分类, 更改失败! 请<b>点击目录根节点</b>进行选择!");
//             m_logging_widget->error(info);
//             QMessageBox::warning(this, tr("未选择要更改的标准分类, 更改失败"), info);
//             return;
//         }
//         auto tagName = le_modify_input->text();
//         if (tagName == "") {
//             auto info = tr("标准分类目录名<b>不能为空</b>, 更改失败!");
//             m_logging_widget->error(info);
//             QMessageBox::warning(this, tr("标准分类目录名为空, 更改失败"), info);
//             return;
//         }
//         editor->modify(m_selected_row, tagName);
//     });
//     connect(editor, &IndexEditor::onModifyRepeat, this, [=](const QString &scName) {
//         auto info = tr("无法更改为<b>同名标准分类</b>\" ") + scName + tr(" \", 更改失败!");
//         m_logging_widget->error(info);
//         QMessageBox::warning(this, tr("无法更改同名标准分类, 更改失败"), info);
//     });
//     connect(editor, &IndexEditor::onModifySuccessed, this, [=]() {
//         m_dir_tree->clearTree();
//         m_dir_tree->refreshTree();
//         le_modify_input->clear();
//         lb_toModify->setText("\"\"");
//         m_selected_row = -1;
//         m_logging_widget->info(tr("标准分类更改成功，上传数据中..."));
//         editor->push_org();
//         QMessageBox::information(this, tr("标准分类更改"), tr("标准分类更改成功"));
//     });

//     return modelWidget;
// }

// ////////////////////////
// //// TagDirWindow
// ////////////////////////

// TagDirWindow::TagDirWindow(QWidget *parent)
//     : DirectoryWindow{tr("三维模型标签目录管理"), parent}
// {

//     // ----------------视图----------------
//     {
//         auto top_menubar = new QMenuBar(this);
//         auto menu_view = new QMenu(tr("视图(&V)"), this);

//         // ----------------保存布局----------------
//         menu_view->addAction(tr("保存布局"), this, [=]() {
//             LayoutManager::getInstance()->save(this, "TagDirWindow");
//         });
//         // ----------------加载布局----------------
//         menu_view->addAction(tr("加载布局"), this, [=]() {
//             LayoutManager::getInstance()->restore(this, "TagDirWindow");
//         });

//         top_menubar->addMenu(menu_view);
//         this->setMenuBar(top_menubar);
//     }

//     auto editor = new IndexEditor(IndexEditor::IndexType::TAGS, this);

//     auto center_widget = centralWidget();

//     auto ly_total = new QGridLayout(center_widget);

//     auto dirWidget = new QWidget(center_widget);

//     auto ly_dir = new QHBoxLayout(dirWidget);

//     m_dir_tree = new InfoTreeWidget(editor->getIndexInfo(),
//                                     editor->getOriginInfo(),
//                                     true,
//                                     dirWidget);

//     ly_dir->addWidget(m_dir_tree, 1);

//     auto modelWidget = initModelWidget(editor);
//     ly_total->addWidget(modelWidget);

//     auto bt_sync = new QPushButton(tr("→\n→\n→\n同\n步\n修\n改\n到\n原\n始\n数\n据\n→\n→\n→\n"),center_widget);
//     {
//         bt_sync->setMaximumWidth(bt_sync->fontMetrics().averageCharWidth() * 4);
//         auto plc_bt_sync = bt_sync->sizePolicy();
//         plc_bt_sync.setVerticalPolicy(QSizePolicy::Expanding);
//         bt_sync->setSizePolicy(plc_bt_sync);
//     }

//     ly_dir->addWidget(bt_sync,  0);

//     connect(bt_sync, &QPushButton::clicked, this, [=]() {
//         editor->syncToOrg();
//     });

//     {
//         auto docker_index = new QDockWidget(tr("标签目录编辑"), center_widget);
//         docker_index->setWidget(dirWidget);
//         docker_index->setObjectName("docker_index");
//         docker_index->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
//         docker_index->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
//         addDockWidget(Qt::LeftDockWidgetArea, docker_index);
//     }

//     {
//         auto cate_sel = new CategorySelector(center_widget);
//         auto docker_cate = new QDockWidget(tr("索引编辑 - 大类选择器"), center_widget);
//         docker_cate->setObjectName("docker_cate");
//         docker_cate->setWidget(cate_sel);
//         docker_cate->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
//         docker_cate->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
//         addDockWidget(Qt::TopDockWidgetArea, docker_cate);
//     }

//     {
//         m_logging_widget = new LoggingWidget(center_widget);
//         auto docker_logging = new QDockWidget(tr("索引编辑 - 信息输出"), center_widget);
//         docker_logging->setObjectName("docker_logging");
//         docker_logging->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
//         docker_logging->setWidget(m_logging_widget);
//         docker_logging->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
//         addDockWidget(Qt::RightDockWidgetArea, docker_logging);
//     }
// }

// void TagDirWindow::closeEvent(QCloseEvent *event)
// {
//     LayoutManager::getInstance()->save(this, "TagDirWindow");
//     QWidget::closeEvent(event);
// }

// void TagDirWindow::showEvent(QShowEvent *event)
// {
//     if (_is_first_paint) {
//         _is_first_paint = false;
//         LayoutManager::getInstance()->restore(this, "TagDirWindow");
//     }
//     update();
// }

// QWidget *TagDirWindow::initModelWidget(IndexEditor *editor)
// {
//     auto modelWidget = new QWidget(centralWidget());

//     auto ly_total = new QGridLayout(modelWidget);

//     auto bt_pull = new QPushButton(tr("↓↓↓拉取数据↓↓↓"), modelWidget);

// //    auto bt_push = new QPushButton(tr("↑↑↑推送数据↑↑↑"), modelWidget);

//     auto ly_pp = new QHBoxLayout();
//     ly_pp->addWidget(bt_pull);
// //    ly_pp->addWidget(bt_push);

//     auto bt_compress = new QPushButton(tr("←\n←\n←\n创\n建\n标\n签\n目\n录\n←\n←\n←\n"), modelWidget);
//     {
//         bt_compress->setMaximumWidth(bt_compress->fontMetrics().averageCharWidth() * 4);
//         auto plc_bt_sync = bt_compress->sizePolicy();
//         plc_bt_sync.setVerticalPolicy(QSizePolicy::Expanding);
//         bt_compress->setSizePolicy(plc_bt_sync);
//     }

//     auto preview_w = new PreviewWidget(editor->getOriginInfo(), 1, 3, PreviewWidget::PreviewType::MODEL, Qt::Vertical, true, true, modelWidget);

//     // editor-modify
//     auto gb_modify = new QGroupBox(tr("标签目录编辑"), modelWidget);
//     gb_modify->setEnabled(false);

//     auto lb_modify_hint = new QLabel(tr("→更改为→"), gb_modify);
//     auto lb_toModify = new QLabel("\"\"", gb_modify);

//     auto le_modify_input = new QLineEdit(gb_modify);
//     le_modify_input->setPlaceholderText("请输入标签名称, 不能为空");

//     auto ly_modify_hint = new QHBoxLayout();

//     ly_modify_hint->addWidget(lb_toModify, 0);
//     ly_modify_hint->addWidget(lb_modify_hint, 0);
//     ly_modify_hint->addWidget(le_modify_input, 0);
//     ly_modify_hint->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding));

//     auto bt_modify = new QPushButton(tr("更改标签名"), gb_modify);

//     auto ly_gb_modify = new QVBoxLayout(gb_modify);
//     ly_gb_modify->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Expanding));
//     ly_gb_modify->addLayout(ly_modify_hint, 0);
//     ly_gb_modify->addWidget(bt_modify, 0);
//     ly_gb_modify->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Expanding));
//     ly_gb_modify->setStretch(0, 1);
//     ly_gb_modify->setStretch(3, 2);

//     ly_total->addWidget(gb_modify,   1, 0);
//     ly_total->addLayout(ly_pp,       0, 2);
//     ly_total->addWidget(bt_compress, 1, 1);
//     ly_total->addWidget(preview_w,   1, 2);
//     ly_total->setColumnStretch(0, 0);
//     ly_total->setColumnStretch(1, 0);
//     ly_total->setColumnStretch(2, 1);

//     connect(bt_pull, &QPushButton::clicked, this, [=]() {
//         editor->pull_org();
//     });
// //    connect(bt_push, &QPushButton::clicked, this, [=]() {
// //        editor->push_org();
// //    });
//     connect(bt_compress, &QPushButton::clicked, this, [=]() {
//         editor->compressToIndex();
//     });
//     connect(preview_w, &PreviewWidget::onPreview, this, [=](const std::vector<int> &index) {
//         preview_w->previewFiles(editor->getFilePaths(index), editor->getPreviewInfo(index), false);
//     });

//     connect(editor, &IndexEditor::onResponsing, this, [=](const QString & info, bool is_continue){
//         is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
//     });
//     connect(editor, &IndexEditor::onPullSuccessful, this, [=]() {
//         preview_w->refreshInfo();
// //        QMessageBox::information(this, tr("数据拉取"), tr("数据拉取成功!"));
//         // 自动压缩
//         bt_compress->click();
//     });
//     connect(editor, &IndexEditor::onPushSuccessful, this, [=]() {
//         preview_w->refreshInfo();
//         QMessageBox::information(this, tr("数据推送"), tr("数据推送成功!"));
//         // 自动更新
//         bt_pull->click();
//     });
//     connect(editor, &IndexEditor::onCompressed, this, [=]() {
//         m_logging_widget->info(tr("创建标签目录成功"));
//         m_dir_tree->clearTree();
//         m_dir_tree->refreshTree();
//         QMessageBox::information(this, tr("创建标签目录"), tr("创建标签目录成功!"));
//     });
//     connect(editor, &IndexEditor::onSyncToOrg, this, [=]() {
//         m_logging_widget->info(tr("标签目录同步成功"));
//         QMessageBox::information(this, tr("标签目录同步"), tr("标签目录同步成功!"));
//     });

//     // 更改标签功能
//     connect(m_dir_tree, &InfoTreeWidget::onRootSelected, this, [=](int row) {
//         m_selected_row = row;
//         gb_modify->setEnabled(true);
//         lb_toModify->setText(QString().asprintf("\"%s\"", editor->index(row).toStdString().c_str()));
//     });

//     connect(bt_modify, &QPushButton::clicked, this, [=]() {
//         if (m_selected_row == -1) {
//             auto info = tr("未选择要更改的标签, 更改失败! 请<b>点击目录根节点</b>进行选择!");
//             m_logging_widget->error(info);
//             QMessageBox::warning(this, tr("未选择要更改的标签, 更改失败"), info);
//             return;
//         }
//         auto tagName = le_modify_input->text();
//         if (tagName == "") {
//             auto info = tr("标签目录名<b>不能为空</b>, 更改失败!");
//             m_logging_widget->error(info);
//             QMessageBox::warning(this, tr("标签目录名为空, 更改失败"), info);
//             return;
//         }
//         editor->modify(m_selected_row, tagName);
//     });
//     connect(editor, &IndexEditor::onModifyRepeat, this, [=](const QString &scName) {
//         auto info = tr("无法更改为<b>同名标签</b>\" ") + scName + tr(" \", 更改失败!");
//         m_logging_widget->error(info);
//         QMessageBox::warning(this, tr("无法更改同名标签, 更改失败"), info);
//     });
//     connect(editor, &IndexEditor::onModifySuccessed, this, [=]() {
//         m_dir_tree->clearTree();
//         m_dir_tree->refreshTree();
//         le_modify_input->clear();
//         lb_toModify->setText("\"\"");
//         m_selected_row = -1;
//         m_logging_widget->info(tr("标签更改成功，上传数据中..."));
//         QMessageBox::information(this, tr("标签更改"), tr("标签更改成功"));
//         editor->push_org();
//     });

//     return modelWidget;
// }
// #include "dbtablewindow.h"

// #include <QHBoxLayout>
// #include <QVBoxLayout>
// #include <QDockWidget>
// #include <QMessageBox>
// #include <QCloseEvent>
// #include <QShowEvent>
// #include <QPushButton>
// #include <QTabWidget>

// #include "gui/uicomponent/loggingwidget.h"
// #include "gui/uicomponent/infotablewidget.h"
// #include "function/layoutmanager.h"
// #include "function/dbtableviewer.h"

// DBTableWindow::DBTableWindow(QWidget *parent)
//     : IFunctionWindow("", {800, 600}, true, false, true, parent)
// {
//     auto center_widget = centralWidget();

//     m_logging_widget = new LoggingWidget(center_widget);
//     auto docker_logging = new QDockWidget(tr("模型数据表预览 - 信息输出"), center_widget);
//     docker_logging->setObjectName("docker_logging");
//     docker_logging->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
//     docker_logging->setWidget(m_logging_widget);
//     docker_logging->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
//     addDockWidget(Qt::RightDockWidgetArea, docker_logging);

//     auto ly_total = new QVBoxLayout(center_widget);

//     auto tabw = new QTabWidget(center_widget);
//     ly_total->addWidget(tabw);

//     auto tb_model_viewer = new DBTableViewer(DBTableViewer::TableType::MODEL, this);
//     auto tb_bvh_viewer = new DBTableViewer(DBTableViewer::TableType::BVH, this);
//     auto tb_effect_viewer = new DBTableViewer(DBTableViewer::TableType::EFFECT, this);

//     auto modelTableW = setupTableWidget(tb_model_viewer);
//     auto bvhTableW = setupTableWidget(tb_bvh_viewer);
//     auto effectTableW = setupTableWidget(tb_effect_viewer);

//     tabw->addTab(modelTableW, tr("模型资源数据表"));
//     tabw->addTab(bvhTableW, tr("骨骼动画资源数据表"));
//     tabw->addTab(effectTableW, tr("特效资源数据表"));
// }

// void DBTableWindow::closeEvent(QCloseEvent *event)
// {
//     LayoutManager::getInstance()->save(this, "DBTableWindow");
//     QWidget::closeEvent(event);
// }

// void DBTableWindow::showEvent(QShowEvent *event)
// {
//     if (_is_first_paint) {
//         _is_first_paint = false;
//         LayoutManager::getInstance()->restore(this, "DBTableWindow");
//     }
//     update();
// }

// QWidget *DBTableWindow::setupTableWidget(DBTableViewer * viewer)
// {
//     auto center_widget = centralWidget();

//     auto totalW = new QWidget(center_widget);
//     auto ly_total = new QVBoxLayout(totalW);

//     auto bt_pullTB = new QPushButton(tr("拉取数据表信息"), this);
//     ly_total->addWidget(bt_pullTB, 0);

//     auto infotable = new InfoTableWidget(viewer->getTableInfo(), 1, false, false, this);
//     ly_total->addWidget(infotable, 1);

//     connect(bt_pullTB, &QPushButton::clicked, this, [=](){
//         infotable->clearInfos();
//         bt_pullTB->setEnabled(false);
//         viewer->pullTBInfo();
//     });

//     connect(viewer, &DBTableViewer::onTBPullSuccessful, this, [=](){
//         infotable->refresh();
//         infotable->selectGroup(0);
//         bt_pullTB->setEnabled(true);
//         m_logging_widget->trace("拉取数据表信息成功");
//     });

//     connect(viewer, &DBTableViewer::onResponsing, this, [=](const QString & info, bool is_continue){
//         is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
//     });

//     return totalW;
// }
// #include "infotablewidget.h"

// #include <QJsonObject>
// #include <QJsonArray>
// #include <QHeaderView>
// #include <QDebug>
// #include <QBrush>
// #include <QComboBox>

// const QColor InfoTableWidget::m_editable_col = QColor{235, 255, 235, 255};
// const QColor InfoTableWidget::m_uneditable_col = QColor{255, 245, 235, 255};
// const QColor InfoTableWidget::m_group_col = QColor{235, 245, 255, 255};

// InfoTableWidget::InfoTableWidget(QJsonObject *info, int spanNum, bool info_editable, bool resizeAsContent, QWidget *parent)
//     : QTableWidget(parent), m_spanNum(spanNum), m_info(info), m_info_editable(info_editable), m_resizeAsContent(resizeAsContent)
// {
//     this->setStyleSheet("border:1px solid #8f8f8f;");
//     horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Interactive);
//     horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
//     connect(this, &QTableWidget::itemDoubleClicked, this, [this](QTableWidgetItem *item) {
//         std::vector<int> rows;
//         if (item->column() == 0) {
//             int groupEnd = item->row() + m_spanNum;
//             for (int r = item->row(); r < this->rowCount() && r < groupEnd; r++) {
//                 rows.push_back(r);
//             }
//         }
//         if (rows.size() > 0)
//             emit this->onGroupSelected(rows);
//     });
//     if (m_info_editable) {
//         connect(this, &QTableWidget::itemChanged, this, [this](QTableWidgetItem *cur) {
//             auto newText = cur->text();
//             auto R = cur->row();
//             auto C = cur->column();
//             if (C <= 0 )
//                 return;
//             auto headerName = this->horizontalHeaderItem(C)->text();
//             auto headers = (*this->m_info)["headers"].toArray();
//             auto vals = (*this->m_info)["data"].toArray();
//             if (R >= vals.size()) {
//                 return;
//             }
//             auto row = vals[R].toObject();
//             if (!row.contains(headerName)) {
//                 qDebug() << "InfoTableWidget>>QTableWidget::itemChanged>> m_info doesn't contain header " << headerName;
//                 return;
//             }
//             QString preText;
//             bool is_array = false;
//             for (auto header : headers) {
//                 if (header.toObject()["name"].toString() == headerName) {
//                     is_array = header.toObject()["is_array"].toBool();
//                 }
//             }
//             if (is_array) {
//                 auto row_obj = row[headerName].toObject();
//                 preText = row_obj["value"].toString();
//                 row_obj["value"] = newText;
//                 row[headerName] = row_obj;
//             } else {
//                 preText = row[headerName].toString();
//                 row[headerName] = newText;
//             }
//             vals[R] = row;
//             (*this->m_info)["data"]=vals;
// //            qDebug() << "InfoTableWidget>>QTableWidget::itemChanged>>, "
// //                     << "from" << preText
// //                     << "to" << newText
// //                     << ", in table[" << R <<", " << C << "]";
//         });
//     }
// }

// void InfoTableWidget::refresh()
// {
//     auto headers = (*this->m_info)["headers"].toArray();
//     if (headers.size() == 0) {
//         qDebug() << "InfoTableWidget::refresh>> headers.size()==0";
//         return;
//     }

//     auto rows = (*this->m_info)["data"].toArray();
//     setRowCount(rows.size());
//     if (rows.size() == 0) {
//         qDebug() << "InfoTableWidget::refresh>> data.size()==0";
//         return;
//     }

//     QStringList headers_str({tr("预览组")});
//     for (const auto &header : headers) {
//         QJsonObject header_ele = header.toObject();
//         if (header_ele["visible"].toBool())
//             headers_str.append(header_ele["name"].toString());
//     }
//     setColumnCount(headers_str.size());
//     setHorizontalHeaderLabels(headers_str);

//     for (int row = 0; row < rows.size(); row++) {
//         auto key2value = rows[row].toObject();
//         if (key2value.size() == 0) {
//             qDebug() << "InfoTableWidget::refresh>> element.size()==0";
//             return;
//         }
//         auto previewItem = new QTableWidgetItem(std::to_string(row / m_spanNum + 1).c_str());
//         previewItem->setToolTip(tr("双击以预览组"));
//         previewItem->setFlags(previewItem->flags() ^= Qt::ItemIsEditable);
//         QBrush brush = previewItem->background();
//         brush.setStyle(Qt::BrushStyle::SolidPattern);
//         brush.setColor(m_group_col);
//         previewItem->setBackground(brush);
//         setItem(row, 0, previewItem);
//         int col = 1;
//         for (const auto &header : headers) {
//             QJsonObject header_ele = header.toObject();
//             if (header_ele["visible"].toBool()) {
//                 QTableWidgetItem *dataItem;
//                 if (header_ele["is_array"].toBool()) {
//                     dataItem = new QTableWidgetItem("");
//                     auto data_array = key2value[header_ele["name"].toString()].toObject()["array"].toArray();
//                     auto combo = new QComboBox(this);
//                     for (const auto &d : data_array) {
//                         combo->addItem(d.toString());
//                     }
//                     connect(combo, &QComboBox::currentTextChanged, this, [=](const QString &t) {
//                         dataItem->setText(t);
//                     });
//                     if (data_array.size() > 0) {
//                         combo->setCurrentIndex(0);
//                         dataItem->setText(combo->itemText(0));
//                     }
//                     combo->setStyleSheet(QString().asprintf("background-color: #%04X; color:black;", m_editable_col.rgb()));
//                     this->setCellWidget(row, col, combo);
//                 } else {
//                     dataItem = new QTableWidgetItem(key2value[header_ele["name"].toString()].toString());
//                 }
//                 if (!m_info_editable || !header_ele["editable"].toBool()) {
//                     dataItem->setFlags(dataItem->flags() ^= Qt::ItemIsEditable);
//                     QBrush brush = dataItem->background();
//                     brush.setStyle(Qt::BrushStyle::SolidPattern);
//                     brush.setColor(m_uneditable_col);
//                     dataItem->setBackground(brush);
//                     dataItem->setToolTip(tr("不可编辑"));
//                 } else {
//                     QBrush brush = dataItem->background();
//                     brush.setStyle(Qt::BrushStyle::SolidPattern);
//                     brush.setColor(m_editable_col);
//                     dataItem->setBackground(brush);
//                     dataItem->setToolTip(tr("可编辑"));
//                 }
//                 setItem(row, col, dataItem);
//                 col++;
//             }
//         }
//     }

//     // 每m_spanNum合并一次首列单元格
//     for (int i = 0; i < rows.size(); i += m_spanNum) {
//         auto spanOffset = std::min(m_spanNum, static_cast<int>(rows.size()) - i);
//         if (spanOffset > 1) {
//             setSpan(i, 0, spanOffset, 1);
//         }
//     }

//     if (m_resizeAsContent)
//         horizontalHeader()->resizeSections(QHeaderView::ResizeMode::ResizeToContents);
//     else
//         horizontalHeader()->resizeSections(QHeaderView::ResizeMode::Interactive);
// }

// void InfoTableWidget::jumpTo(int row)
// {
//     selectRow(row);
//     this->item(row, 0)->setSelected(false);
//     scrollToItem(item(row, 0));
// }

// void InfoTableWidget::selectGroup(int group)
// {
//     if (this->rowCount() > 0)
//         emit itemDoubleClicked(item(std::min(group * m_spanNum, rowCount() - 1), 0));
// }

// void InfoTableWidget::clearInfos()
// {
//     this->clearContents();
//     std::vector<int> empty_row;
//     emit onGroupSelected(empty_row);
// }

// void InfoTableWidget::resetInfo(QJsonObject *info)
// {
//     m_info = info;
//     clearInfos();
//     refresh();
// }
// #include "renderwidget.h"

// #include <QOpenGLContext>
// #include <QDebug>
// #include <QTimerEvent>

// #include "resource/model.h"
// #include "resource/bvh.h"

// #include "function/renderer/renderer.h"

// RenderWidget::RenderWidget(QWidget *parent) : QOpenGLWidget(parent)
// {
//     this->setFocusPolicy(Qt::WheelFocus);
//     m_renderer = new Renderer(size().width(), size().height(), this);
// }

// RenderWidget::~RenderWidget()
// {
// //    if (!m_renderer) {
// ////        qDebug() << "RenderWidget::~RenderWidget";
// //        delete m_renderer;
// //        m_renderer = nullptr;
// //    }
// }

// void RenderWidget::initializeGL()
// {
//     if (!m_renderer->is_initialized()) {
//         //    qDebug() << "initializeGL";
//         makeCurrent();
//         m_renderer->initialize();
//         doneCurrent();

//         m_timer.start(16, this);
//         start_time = std::chrono::steady_clock::now();
//     }
// }

// void RenderWidget::resizeGL(int w, int h)
// {
//     if (m_renderer->is_initialized()) {
//         m_renderer->resize(QOpenGLContext::currentContext(), w, h);
//     }
// }

// void RenderWidget::paintGL()
// {
//     auto currentContext = QOpenGLContext::currentContext();
//     auto deltaTime = m_dt;
//     if (m_renderer->is_initialized()) {
//         m_renderer->logic_tick(deltaTime);
//         m_renderer->render_tick(currentContext);
//         m_renderer->pop_input();
//         if (m_renderer->hasRendererData())
//             emit onFrameUpdate();
//     }
// }

// void RenderWidget::timerEvent(QTimerEvent *event)
// {
//     *const_cast<float *>(&m_dt) = std::chrono::duration<float>(std::chrono::steady_clock::now() - start_time).count() * 1000.f;
//     start_time = std::chrono::steady_clock::now();
//     update();
// }

// void RenderWidget::keyPressEvent(QKeyEvent *event)
// {
//     if (m_renderer->is_initialized()) {
//         InputData input{InputData::InputType::KEY};
//         input.key.press = true;
//         switch (event->key()) {
//         case Qt::Key_W: // front
//         {
//             input.keyCode = InputData::W;
//             m_renderer->push_input(input);
//             break;
//         }
//         case Qt::Key_A: // left
//         {
//             input.keyCode = InputData::A;
//             m_renderer->push_input(input);
//             break;
//         }
//         case Qt::Key_S: // back
//         {
//             input.keyCode = InputData::S;
//             m_renderer->push_input(input);
//             break;
//         }
//         case Qt::Key_D: // right
//         {
//             input.keyCode = InputData::D;
//             m_renderer->push_input(input);
//             break;
//         }
//         case Qt::Key_R: // reset
//         {
//             input.keyCode = InputData::R;
//             m_renderer->push_input(input);
//             break;
//         }
//         case Qt::Key_Escape: // reset
//         {
//             clearFocus();
//             break;
//         }
//         }
//     }
// }

// void RenderWidget::keyReleaseEvent(QKeyEvent *event)
// {
//     if (m_renderer->is_initialized()) {
//         InputData input{InputData::InputType::KEY};
//         input.key.release = true;
//         switch (event->key()) {
//         case Qt::Key_W: // front
//         {
//             input.keyCode = InputData::W;
//             m_renderer->push_input(input);
//             break;
//         }
//         case Qt::Key_A: // left
//         {
//             input.keyCode = InputData::A;
//             m_renderer->push_input(input);
//             break;
//         }
//         case Qt::Key_S: // back
//         {
//             input.keyCode = InputData::S;
//             m_renderer->push_input(input);
//             break;
//         }
//         case Qt::Key_D: // right
//         {
//             input.keyCode = InputData::D;
//             m_renderer->push_input(input);
//             break;
//         }
//         case Qt::Key_R: // reset
//         {
//             input.keyCode = InputData::R;
//             m_renderer->push_input(input);
//             break;
//         }
//         case Qt::Key_Escape: // reset
//         {
//             clearFocus();
//             break;
//         }
//         }
//     }
// }

// void RenderWidget::wheelEvent(QWheelEvent *event)
// {
//     if (m_renderer->is_initialized()) {
//         InputData input{InputData::InputType::WHEEL};
//         input.scrollx = event->angleDelta().x();
//         input.scrolly = event->angleDelta().y();
//         m_renderer->push_input(input);
//     }
// }

// void RenderWidget::mousePressEvent(QMouseEvent *event)
// {
//     if (m_renderer->is_initialized()) {
//         if (event->button() == Qt::MouseButton::LeftButton) {
//             InputData input{InputData::InputType::MOUSE};
//             input.mouse.press = true;
//             input.mouseButton = InputData::MouseButton::LEFT;
//             input.pos = event->pos();
//             m_renderer->push_input(input);
//         } else if (event->button() == Qt::MouseButton::RightButton) {
//             InputData input{InputData::InputType::MOUSE};
//             input.mouse.press = true;
//             input.mouseButton = InputData::MouseButton::RIGHT;
//             input.pos = event->pos();
//             m_renderer->push_input(input);
//         } else if (event->button() == Qt::MouseButton::MiddleButton) {
//             InputData input{InputData::InputType::MOUSE};
//             input.mouse.press = true;
//             input.mouseButton = InputData::MouseButton::MIDDLE;
//             input.pos = event->pos();
//             m_renderer->push_input(input);
//         }
//     }
//     event->ignore();
// }

// void RenderWidget::mouseReleaseEvent(QMouseEvent *event)
// {
//     if (m_renderer->is_initialized()) {
//         if (event->button() == Qt::MouseButton::LeftButton) {
//             InputData input{InputData::InputType::MOUSE};
//             input.mouse.release = true;
//             input.mouseButton = InputData::MouseButton::LEFT;
//             input.pos = event->pos();
//             m_renderer->push_input(input);
//         } else if (event->button() == Qt::MouseButton::RightButton) {
//             InputData input{InputData::InputType::MOUSE};
//             input.mouse.release = true;
//             input.mouseButton = InputData::MouseButton::RIGHT;
//             input.pos = event->pos();
//             m_renderer->push_input(input);
//         } else if (event->button() == Qt::MouseButton::MiddleButton) {
//             InputData input{InputData::InputType::MOUSE};
//             input.mouse.release = true;
//             input.mouseButton = InputData::MouseButton::MIDDLE;
//             input.pos = event->pos();
//             m_renderer->push_input(input);
//         }
//     }
//     event->ignore();
// }

// void RenderWidget::mouseMoveEvent(QMouseEvent *event)
// {
//     if (m_renderer->is_initialized()) {
//         InputData input{InputData::InputType::MOUSE};
//         if (event->buttons() & Qt::MouseButton::LeftButton) {
//             input.mouse.move = true;
//             input.mouseButtons = InputData::MouseButton::LEFT;
//         }
//         if (event->buttons() & Qt::MouseButton::RightButton) {
//             input.mouse.move = true;
//             input.mouseButtons |= InputData::MouseButton::RIGHT;
//         }
//         if (event->buttons() & Qt::MouseButton::MiddleButton) {
//             input.mouse.move = true;
//             input.mouseButtons |= InputData::MouseButton::MIDDLE;
//         }
//         input.pos = event->pos();
//         if (input.mouse.move) {
//             m_renderer->push_input(input);
//         }
//     }
// }

// void RenderWidget::leaveEvent(QEvent *event)
// {
//     this->clearFocus();
// }

// void RenderWidget::doModelRendering(const QString &filePath)
// {
//     if (ModelManager::getInstance()->has(filePath.toStdString())) {
// //        qDebug() << "RenderWidget::doModelRendering>> Render Model" << filePath;
//         auto model = ModelManager::getInstance()->get(filePath.toStdString());
//         if (m_renderer->is_initialized()) {
//             m_renderer->setRenderData(model);
//         }
//     } else {
//         qDebug() << "RenderWidget::doModelRendering>> Render Model Not Found" << filePath;
//     }
// }

// void RenderWidget::doBVHRendering(const QString &filePath)
// {
//     if (BVHManager::getInstance()->has(filePath.toStdString())) {
//         auto bvh = BVHManager::getInstance()->get(filePath.toStdString());
//         if (m_renderer->is_initialized()) {
//             m_renderer->setRenderData(bvh);
//         }
//     } else {
//         qDebug() << "RenderWidget::doModelRendering>> Render Model Not Found" << filePath;
//     }
// }

// void RenderWidget::doModelRendering(std::shared_ptr<res::Model> model)
// {
//     if (m_renderer->is_initialized()) {
//         m_renderer->setRenderData(model);
//     }
// }

// void RenderWidget::doBVHRendering(std::shared_ptr<res::BVH> bvh)
// {
//     if (m_renderer->is_initialized()) {
//         m_renderer->setRenderData(bvh);
//     }
// }

// void RenderWidget::stopRendering()
// {
//     if (m_renderer->is_initialized()) {
//         m_renderer->clearRenderData();
//     }
// }
// #include "previewwidget.h"

// #include <QVBoxLayout>
// #include <QGridLayout>
// #include <QLabel>
// #include <QSplitter>
// #include <QHeaderView>
// #include <QFileInfo>
// #include <QJsonObject>
// #include <QJsonArray>
// #include <QJsonDocument>

// #include "gui/uicomponent/previewpane.h"
// #include "gui/uicomponent/infotablewidget.h"

// PreviewWidget::PreviewWidget(QJsonObject *info, int row, int column, PreviewType type, Qt::Orientation split_orientation, bool info_editable, bool info_resizeAsContent, QWidget *parent)
//     : QWidget{parent}, m_previewNum(row * column), m_type(type)
// {
//     auto ly_total = new QVBoxLayout(this);
//     this->setLayout(ly_total);

//     auto ly_pane = new QGridLayout();

//     auto panesWidget = new QWidget(this);
//     panesWidget->setStyleSheet("border:1px solid #8f8f8f;");
//     panesWidget->setLayout(ly_pane);

//     m_infoTable = new InfoTableWidget(info, m_previewNum, info_editable, info_resizeAsContent, this);

//     for (int r = 0; r < row; r++) {
//         for (int c = 0; c < column; c++) {
//             auto previewPane = new PreviewPane(panesWidget);
//             ly_pane->addWidget(previewPane, r, c);
//             m_previewPanes.push_back(previewPane);
//         }
//     }

//     auto splitter = new QSplitter(this);
//     splitter->setOrientation(split_orientation);
//     if (split_orientation == Qt::Orientation::Horizontal) {
//         splitter->addWidget(m_infoTable);
//         splitter->setStretchFactor(0, 1);
//         splitter->addWidget(panesWidget);
//         splitter->setStretchFactor(1, 2);
//     } else if (split_orientation == Qt::Orientation::Vertical) {
//         splitter->addWidget(panesWidget);
//         splitter->setStretchFactor(0, 1);
//         splitter->addWidget(m_infoTable);
//         splitter->setStretchFactor(1, 1);
//     }

//     ly_total->addWidget(splitter);

// }

// void PreviewWidget::refreshInfo()
// {
//     emit onRefresh();
//     disconnect(m_infoTable, &InfoTableWidget::onGroupSelected, this, 0);
//     connect(m_infoTable, &InfoTableWidget::onGroupSelected, this, &PreviewWidget::doPreviewPrepare);
//     m_infoTable->refresh();
// }

// void PreviewWidget::clearInfo()
// {
//     emit onClear();
//     m_infoTable->clearInfos();
//     disconnect(m_infoTable, &InfoTableWidget::onGroupSelected, this, 0);
// }

// void PreviewWidget::selectGroup(int group)
// {
//     m_infoTable->selectGroup(group);
// }

// void PreviewWidget::previewFiles(const QStringList &filePaths, const QStringList &assetNames, bool cache)
// {
//     for (int i = 0; i < m_previewNum; i++) {
//         if (i < filePaths.size()) {
//             if (m_type == PreviewType::MODEL) {
//                 m_previewPanes[i]->doPreviewModel(filePaths[i], assetNames[i], cache);
//             } else if (m_type == PreviewType::BVH) {
//                 m_previewPanes[i]->doPreviewBVH(filePaths[i], assetNames[i], cache);
//             } else if (m_type == PreviewType::EFFECT) {
//                 m_previewPanes[i]->doPreviewEffect(filePaths[i], assetNames[i]);
//             }
//         }
//     }
// }

// std::vector<PreviewPane *> PreviewWidget::getPreviewPane() const
// {
//     return m_previewPanes;
// }

// InfoTableWidget *PreviewWidget::getInfoTable() const
// {
//     return m_infoTable;
// }

// void PreviewWidget::doPreviewPrepare(const std::vector<int> &index)
// {
//     for (size_t i = 0; i < static_cast<size_t>(m_previewNum); i++) {
//         disconnect(m_previewPanes[i], &PreviewPane::onSelectedPane, this, 0);
//         m_previewPanes[i]->doClear();
//         if (i < index.size()) {
//             auto row = index[i];
//             connect(m_previewPanes[i], &PreviewPane::onSelectedPane, this, [row, this]() {
//                 m_infoTable->jumpTo(row);
//             });
//         }
//     }
//     emit onPreview(index);
// }
// #include "previewpane.h"

// #include <QFileInfo>
// #include <QVBoxLayout>
// #include <QDir>

// #include <QMouseEvent>
// #include <QDebug>

// #include "function/assetloader/modelloader.h"
// #include "function/assetloader/bvhloader.h"
// #include "gui/uicomponent/renderwidget.h"

// PreviewPane::PreviewPane(QWidget *parent, bool linkTableItem)
//     : QWidget(parent), m_linkTableItem(linkTableItem)
// {
//     m_renderWidget = new RenderWidget(this);
//     m_renderWidget->setStyleSheet("border: 1px solid #8f8f8f;");

//     m_default = new QLabel(this);
//     m_default->setAlignment(Qt::AlignmentFlag::AlignCenter);
//     m_default->setStyleSheet("border: 1px solid #8f8f8f; font-size: 26px;");

//     m_info = new QLabel(this);
//     m_info->setStyleSheet("border: 1px solid #8f8f8f;");
//     m_info->setAlignment(Qt::AlignmentFlag::AlignCenter);
//     m_info->setContentsMargins(0, 5, 0, 5);

//     auto ly_v = new QVBoxLayout(this);
//     ly_v->setSpacing(2);
//     ly_v->addWidget(m_renderWidget,1);
//     ly_v->addWidget(m_default, 1);
//     ly_v->addWidget(m_info, 0);
//     this->setLayout(ly_v);

//     doShowDefault();

//     connect(this, &PreviewPane::onModelCached, m_renderWidget, QOverload<const QString&>::of(&RenderWidget::doModelRendering), Qt::QueuedConnection);
//     connect(this, &PreviewPane::onBVHCached, m_renderWidget, QOverload<const QString&>::of(&RenderWidget::doBVHRendering), Qt::QueuedConnection);

//     qRegisterMetaType<std::shared_ptr<res::Model>>("std::shared_ptr<res::Model>");
//     qRegisterMetaType<std::shared_ptr<res::BVH>>("std::shared_ptr<res::BVH>");
//     connect(this, &PreviewPane::onModelLoaded, m_renderWidget, QOverload<std::shared_ptr<res::Model>>::of(&RenderWidget::doModelRendering), Qt::QueuedConnection);
//     connect(this, &PreviewPane::onBVHLoaded, m_renderWidget, QOverload<std::shared_ptr<res::BVH>>::of(&RenderWidget::doBVHRendering), Qt::QueuedConnection);

//     connect(this, &PreviewPane::onPreviewPrepared, this, &PreviewPane::doShowRenderer, Qt::QueuedConnection);
//     connect(this, &PreviewPane::onEffectPrepared, this, &PreviewPane::doShowEffect, Qt::QueuedConnection);
//     connect(this, &PreviewPane::onPreviewFailed, this, &PreviewPane::doShowError, Qt::QueuedConnection);

// }

// RenderWidget *PreviewPane::getRenderWidget() const { return m_renderWidget; }

// void PreviewPane::mouseDoubleClickEvent(QMouseEvent *event)
// {
//     if (m_linkTableItem) {
//         if (event->button() == Qt::MouseButton::LeftButton) {
//             emit onSelectedPane();
//         }
//     }
// }

// void PreviewPane::doShowRenderer(const QString& previewInfo)
// {
//     m_info->setText(previewInfo);

//     m_default->hide();
//     m_renderWidget->show();
// }

// void PreviewPane::doShowEffect(const QString &previewInfo)
// {
//     m_default->setText(tr("双击此处\n打开特效文件"));
//     m_info->setText(previewInfo);

//     m_renderWidget->hide();
//     m_default->show();
// }

// void PreviewPane::doShowPreparing(const QString& previewInfo)
// {
//     m_default->setText(tr("资源正在载入..."));
//     m_info->setText(previewInfo);

//     m_renderWidget->hide();
//     m_default->show();
// }

// void PreviewPane::doShowError(const QString &previewInfo)
// {
//     m_default->setText(tr("资源载入失败"));
//     m_info->setText(previewInfo);

//     m_renderWidget->hide();
//     m_default->show();
// }

// void PreviewPane::doShowDefault()
// {
//     m_default->setText(tr("无资源"));
//     m_info->setText(tr("无名称"));

//     m_renderWidget->hide();
//     m_default->show();
// }

// void PreviewPane::doPreviewModel(const QString &filePath, const QString& previewInfo, bool cache)
// {
//     doShowPreparing(previewInfo);
//     if (cache) {
//         ModelLoader::getInstance()->cachedAsyncLoad(filePath, [filePath, previewInfo, this](bool is_loaded) {
//             if (is_loaded) {
//                 emit onPreviewPrepared(previewInfo);
//                 emit onModelCached(filePath);
//             } else {
//                 emit onPreviewFailed(previewInfo);
//             }
//         });
//     } else {
//         ModelLoader::getInstance()->tempAsyncLoad(filePath, [filePath, previewInfo, this](std::shared_ptr<res::Model> model) {
//             if (nullptr != model) {
//                 emit onPreviewPrepared(previewInfo);
//                 emit onModelLoaded(model);
//             } else {
//                 emit onPreviewFailed(previewInfo);
//             }
//         });
//     }
// }

// void PreviewPane::doPreviewBVH(const QString &filePath, const QString &previewInfo, bool cache)
// {
//     doShowPreparing(previewInfo);
//     if (cache) {
//         BVHLoader::getInstance()->cachedAsyncLoad(filePath, [filePath, previewInfo, this](bool is_loaded) {
//             if (is_loaded) {
//                 emit onPreviewPrepared(previewInfo);
//                 emit onBVHCached(filePath);
//             } else {
//                 emit onPreviewFailed(previewInfo);
//             }
//         });
//     } else {
//         BVHLoader::getInstance()->tempAsyncLoad(filePath, [filePath, previewInfo, this](std::shared_ptr<res::BVH> bvh) {
//             if (nullptr != bvh) {
//                 emit onPreviewPrepared(previewInfo);
//                 emit onBVHLoaded(bvh);
//             } else {
//                 emit onPreviewFailed(previewInfo);
//             }
//         });
//     }
// }

// void PreviewPane::doPreviewEffect(const QString &filePath, const QString &previewInfo)
// {
//     doShowPreparing(previewInfo);
//     QDir dir(filePath);
//     qDebug() << dir;
//     if (dir.exists(filePath))
//     {
//         emit onEffectPrepared(previewInfo);
//     }
//     else
//     {
//         emit onPreviewFailed(previewInfo);
//     }
// }

// void PreviewPane::doClear()
// {
//     doShowDefault();
//     m_renderWidget->stopRendering();
// }
// #include "modelsearchwidget.h"
// #include "ui_modelsearchwidget.h"

// #include <QFileDialog>
// #include <QSplitter>
// #include <QDebug>
// #include <QMessageBox>

// #include "gui/uicomponent/previewwidget.h"
// #include "function/configer/configmanager.h"
// #include "gui/uicomponent/previewpane.h"
// #include "gui/uicomponent/renderwidget.h"
// #include "function/renderer/renderer.h"

// ModelSearchWidget::ModelSearchWidget(ModelSearch::SearchType searchType, QWidget *parent) :
//     QWidget(parent), ui(new Ui::ModelSearchWidget), m_single_preview(nullptr)
// {
//     ui->setupUi(this);

//     m_modelSearch = new ModelSearch(searchType, this);
//     connect(m_modelSearch, &ModelSearch::onResultUpdate, this, [=](){
//         m_preview->clearInfo();
//         m_preview->refreshInfo();
//         m_preview->selectGroup(0);
//     });
//     connect(m_modelSearch, &ModelSearch::onResultClear, this, [=](){
//         m_preview->clearInfo();
//     });
//     connect(m_modelSearch, &ModelSearch::onResponsing, this, [=](const QString&, bool is_continue){
//         ui->bt_search->setEnabled(!is_continue);
//     });
//     connect(m_modelSearch, &ModelSearch::onSearchSuccessful, this, [=](){
//         switch (searchType) {
//         case ModelSearch::SearchType::CONTENT:
//             QMessageBox::information(this, tr("模型内容检索"), tr("内容检索成功!"));
//             break;
//         case ModelSearch::SearchType::TYPE:
//             QMessageBox::information(this, tr("模型类型检索"), tr("类型检索成功!"));
//             break;
//         case ModelSearch::SearchType::TAGS:
//             QMessageBox::information(this, tr("模型标签检索"), tr("标签检索成功!"));
//             break;
//         }
//     });

//     m_preview = new PreviewWidget(m_modelSearch->getResultsModelInfo(),
//                                   3,
//                                   3,
//                                   PreviewWidget::PreviewType::MODEL,
//                                   Qt::Orientation::Horizontal,
//                                   false,
//                                   true,
//                                   this);

//     connect(m_preview, &PreviewWidget::onPreview, this, [this](const std::vector<int> &index) {
//         m_preview->previewFiles(m_modelSearch->getFilePaths(index), m_modelSearch->getPreviewInfo(index), true);
//         auto panes = m_preview->getPreviewPane();
//         for (size_t i = 0; i < panes.size(); i++) {
//             if (i < index.size()) {
//                 auto renderer = panes[i]->getRenderWidget()->getRenderer();
//                 auto trans_pair = m_modelSearch->getTransform(index[i]);
//                 renderer->setModelTransform(trans_pair.first);
//                 renderer->setCameraTransform(trans_pair.second);
//             }
//         }
//     });

//     if (searchType == ModelSearch::SearchType::CONTENT) {
//         // 显示浏览按钮
//         ui->bt_browser->setHidden(false);
//         // 添加单个模型预览界面
//         m_single_preview = new PreviewWidget(m_modelSearch->getSearchModelInfo(),
//                                              1,
//                                              1,
//                                              PreviewWidget::PreviewType::MODEL,
//                                              Qt::Orientation::Vertical,
//                                              false,
//                                              this);
//         auto split_preview = new QSplitter(this);
//         split_preview->setOrientation(Qt::Orientation::Horizontal);
//         split_preview->addWidget(m_single_preview);
//         split_preview->setStretchFactor(0, 1);
//         split_preview->addWidget(m_preview);
//         split_preview->setStretchFactor(1, 2);
//         ui->ly_preview->addWidget(split_preview);
//         ui->le_input->setEnabled(false);

//         connect(m_single_preview, &PreviewWidget::onPreview, this, [this](const std::vector<int> &index) {
//             if (m_selectedFile != "" && index.size() > 0) {
//                 m_single_preview->previewFiles({m_selectedFile}, {tr("模型预览")}, true);
//             }
//         });
//     } else {
//         ui->bt_browser->setHidden(true);
//         ui->ly_preview->addWidget(m_preview);
//         ui->le_input->setEnabled(true);
//     }

// //    emit m_preview->onPreview({0});

// }

// ModelSearchWidget::~ModelSearchWidget()
// {
//     delete ui;
// }

// void ModelSearchWidget::on_le_input_textChanged(const QString &input)
// {
//     if (m_modelSearch->getType() != ModelSearch::SearchType::CONTENT) {
//         m_modelSearch->setSearchInfo(input);
//     }
// }

// void ModelSearchWidget::on_bt_browser_clicked()
// {
//     QString open_dir = ConfigManager::getInstance()->getConfig("FileBrowser/ModelSearch").toString();
//     m_selectedFile = QFileDialog::getOpenFileName(this, tr("浏览模型文件"), open_dir, "*.obj;*.fbx");
//     if (m_selectedFile == "")
//         return;
//     if (nullptr == m_single_preview) {
//         qDebug() << "ModelSearchWidget::on_bt_browser_clicked>> m_single_preview is null";
//         return;
//     }
//     ui->le_input->setText(m_selectedFile);
//     ConfigManager::getInstance()->setConfig("FileBrowser/ModelSearch", QFileInfo(m_selectedFile).absolutePath());
//     // 设置搜索信息
//     m_modelSearch->setSearchInfo(m_selectedFile);
//     m_single_preview->clearInfo();
//     m_single_preview->refreshInfo();
//     // 预览该模型
//     m_single_preview->selectGroup(0);
// }


// void ModelSearchWidget::on_bt_search_clicked()
// {
//     ui->bt_search->setEnabled(false);
//     m_modelSearch->clearResults();
//     if (m_modelSearch->getType() != ModelSearch::SearchType::CONTENT) {
//         m_modelSearch->setSearchInfo(ui->le_input->text());
//     }
//     m_modelSearch->search();
// }

// #include "infotreewidget.h"
// #include <QHeaderView>
// #include <QJsonArray>
// #include <QDebug>

// InfoTreeWidget::InfoTreeWidget(QJsonObject *root, QJsonObject *detail, bool resizeAsContent, QWidget *parent)
//     : QTreeWidget{parent}, m_root{root}, m_detail{detail}, m_resizeAsContent{resizeAsContent}
// {
// //    this->header()->setSectionResizeMode(QHeaderView::ResizeMode::Interactive);
//     this->header()->setDefaultAlignment(Qt::AlignLeft);
//     connect(this, &QTreeWidget::itemClicked, this, [=](QTreeWidgetItem *item, int column) {
//         if (nullptr == item->parent())
//         {
//             qDebug() << "QTreeWidget::itemClicked>> 点击了: " << item->text(column)
//                      << ", index: " << indexOfTopLevelItem(item);
//             emit onRootSelected(indexOfTopLevelItem(item));
//         }
//     });
// }

// void InfoTreeWidget::refreshTree()
// {
//     auto root_headers = (*this->m_root)["headers"].toArray();
//     if (root_headers.size() != 1) {
//         qDebug() << "InfoTreeWidget::refreshTree>> m_root headers size is not 1";
//         return;
//     }

//     auto detail_headers = (*this->m_detail)["headers"].toArray();
//     if (detail_headers.size() == 0) {
//         qDebug() << "InfoTreeWidget::refreshTree>> m_detail headers size is 0";
//         return;
//     }

//     QStringList headerLabels;
//     for (const auto &h : root_headers) {
//         const auto& hobj = h.toObject();
//         if (hobj["visible"].toBool())
//             headerLabels << hobj["name"].toString();
//     }
//     for (const auto & h : detail_headers) {
//         const auto& hobj = h.toObject();
//         if (hobj["visible"].toBool())
//             headerLabels << h.toObject()["name"].toString();
//     }

//     setHeaderLabels(headerLabels);

//     auto root_rows = (*this->m_root)["data"].toArray();
//     if (root_rows.size() == 0) {
//         qDebug() << "InfoTreeWidget::refreshTree>> root_rows size is 0";
//         return;
//     }

//     auto detail_rows = (*this->m_detail)["data"].toArray();

//     for (const auto &rroot : root_rows) {
//         const auto &rObj = rroot.toObject();
//         auto keyName = headerLabels[0];
//         auto valName = rObj[headerLabels[0]].toString();
//         auto topItem = new QTreeWidgetItem({valName});
//         addTopLevelItem(topItem);
//         for (const auto &rdetail : detail_rows) {
//             const auto &dObj = rdetail.toObject();
//             if (dObj[keyName] == valName) {
//                 QStringList detailInfo;
//                 detailInfo << dObj["name"].toString();
//                 for (int i = 1; i < headerLabels.size(); ++i) {
//                     detailInfo << dObj[headerLabels[i]].toString();
//                 }
//                 auto detailItem = new QTreeWidgetItem(detailInfo);
//                 topItem->addChild(detailItem);
//             }
//         }
//         topItem->setExpanded(true);
//     }

//     if (m_resizeAsContent)
//     {
//         header()->resizeSections(QHeaderView::ResizeMode::ResizeToContents);
//     }
//     else
//     {
//         header()->resizeSections(QHeaderView::ResizeMode::Interactive);
//     }
// }

// void InfoTreeWidget::clearTree()
// {
//     this->clear();
//     emit onRootSelected(-1);
// }

// void InfoTreeWidget::resetTree(QJsonObject *root, QJsonObject *detail)
// {
//     m_root = root;
//     m_detail = detail;
//     refreshTree();
// }
// #include "renderer.h"

// #include <QDebug>

// #include "renderapi.h"
// #include "renderdata.h"
// #include "resource/model.h"
// #include "resource/bvh.h"
// #include "arcball.h"
// #include "camera.h"
// #include "function/configer/configmanager.h"

// const auto defaultCameraTrans = Transform{QVector3D{.0f, .0f, 1.0f},
//                                          QQuaternion{1.f, QVector3D{.0f, .0f, .0f}},
//                                          QVector3D{.0f, .0f, .0f}};

// const auto defaultModelTrans = Transform({.0f, .0f, -1.2f}, QQuaternion{1.f, {.0f, .0f, .0f}}, {1.f, 1.f, 1.f});

// Renderer::Renderer(int sw, int sh, QObject *parent)
//     : QObject(parent)
//     , m_sw(sw)
//     , m_sh(sh)
//     , m_is_initialized(false)
// {
//     trans_camera = defaultCameraTrans;
//     trans_model = defaultModelTrans;

//     m_renderConfig.skyColor = QVector3D{0.2f, 0.3f, 0.48f};
//     m_renderConfig.groundColor = QVector3D{0.4f, 0.3f, 0.2f};
//     m_renderConfig.arcball.moveSpeed = 0.005f;
//     m_renderConfig.arcball.rotateSpeed = 1.0f;
//     m_renderConfig.arcball.drag = 0.01f;
//     m_renderConfig.fpscamera.moveSpeed = 0.002f;
//     m_renderConfig.fpscamera.rotateSpeed = 0.002f;

//     m_arcBall = std::make_shared<ArcBall>(&trans_model, m_sw, m_sh, m_renderConfig.arcball.moveSpeed, m_renderConfig.arcball.rotateSpeed, m_renderConfig.arcball.drag);

//     m_fpsCamera = std::make_shared<FPSCameraController>(&trans_camera, m_renderConfig.fpscamera.moveSpeed, m_renderConfig.fpscamera.rotateSpeed);

//     auto doRendererConfigModified = [this]() {
//         auto configM = ConfigManager::getInstance();
//         auto val = configM->getConfig("Render/SkyColor", m_renderConfig.skyColor);
//         if (!val.canConvert<QVector3D>()) {
//             qDebug() << "Renderer::Renderer>> SkyColor" << val << "is not Vec3";
//             val = m_renderConfig.skyColor;
//         }
//         m_renderConfig.skyColor = val.value<QVector3D>();
//         val = configM->getConfig("Render/GroundColor", m_renderConfig.groundColor);
//         if (!val.canConvert<QVector3D>()) {
//             qDebug() << "Renderer::Renderer>> GroundColor" << val << "is not Vec3";
//             val = m_renderConfig.groundColor;
//         }
//         m_renderConfig.groundColor = val.value<QVector3D>();

//         m_renderConfig.arcball.moveSpeed = configM->getConfig("Render/ArcBall_moveSpeed", m_renderConfig.arcball.moveSpeed).toFloat();
//         m_renderConfig.arcball.rotateSpeed = configM->getConfig("Render/ArcBall_rotateSpeed", m_renderConfig.arcball.rotateSpeed).toFloat();
//         m_renderConfig.arcball.drag = configM->getConfig("Render/ArcBall_drag", m_renderConfig.arcball.drag).toFloat();
//         m_renderConfig.fpscamera.moveSpeed = configM->getConfig("Render/FPSCamera_moveSpeed", m_renderConfig.fpscamera.moveSpeed).toFloat();
//         m_renderConfig.fpscamera.rotateSpeed = configM->getConfig("Render/FPSCamera_rotateSpeed", m_renderConfig.fpscamera.rotateSpeed).toFloat();

//         m_arcBall->setSclar(m_renderConfig.arcball.rotateSpeed);
//         m_arcBall->setDrag(m_renderConfig.arcball.drag);

//         m_fpsCamera->setMoveSpeed(m_renderConfig.fpscamera.moveSpeed);
//         m_fpsCamera->setRotateSpeed(m_renderConfig.fpscamera.rotateSpeed);

//         if (this->is_initialized()) {
//             m_skyData->sky = m_renderConfig.skyColor;
//             m_skyData->ground = m_renderConfig.groundColor;
//         }
//     };
//     connect(ConfigManager::getInstance(), &ConfigManager::onConfModified, this, doRendererConfigModified);
//     doRendererConfigModified();
// }

// Renderer::~Renderer()
// {
//     //    qDebug() << "Renderer::~Renderer";
// }

// void Renderer::initialize()
// {
//     RenderAPI::getInstance()->init();

//     m_is_initialized = true;
//     m_staticShader   = new StaticModelLightShader("Static", this);
//     m_animationShader = new AnimationModelLightShader("Animation", this);
//     m_skyShader     = new SkyShader("Skybox", this);
//     m_skyData = std::make_shared<SkyData>(m_renderConfig.skyColor, m_renderConfig.groundColor);
// }

// void Renderer::resize(QOpenGLContext *context, int w, int h)
// {
//     m_sw = w;
//     m_sh = h;

//     if (m_meshesData)
//         m_meshesData->camera->aspect = static_cast<float>(w) / h;
//     m_arcBall->resize(w, h);

//     RenderAPI::getInstance()->resize(context, w, h);
// }

// void Renderer::push_input(const InputData &input)
// {
//     switch (input.type) {
//     case InputData::InputType::KEY: {
//         m_input.keyCode = input.keyCode;
//         for (int i = 0; i < m_input.nKeyEvents; i++) {
//             m_input.key.key_event[i] |= input.key.key_event[i];
//         }
//         break;
//     }
//     case InputData::InputType::MOUSE: {
//         m_input.pos = input.pos;
//         m_input.mouseButton = input.mouseButton;
//         m_input.mouseButtons |= input.mouseButtons;
//         for (int i = 0; i < m_input.nMouseEvents; i++) {
//             m_input.mouse.mouse_event[i] |= input.mouse.mouse_event[i];
//         }
//         break;
//     }
//     case InputData::InputType::WHEEL: {
//         m_input.scrollx = input.scrollx;
//         m_input.scrolly = input.scrolly;
//         break;
//     }
//     }
// }

// void Renderer::logic_tick(float dt)
// {
//     if (m_meshesData) {
//         // 绘制数据处理
//         m_meshesData->tick(dt);
//         // 缩放处理
//         auto zoom = m_input.scrolly;
//         auto scale = &trans_model.scale;
//         scale->setX(std::max(.5f, std::min(6.f, scale->x() + zoom * dt * 0.0003f)));
//         scale->setY(std::max(.5f, std::min(6.f, scale->y() + zoom * dt * 0.0003f)));
//         scale->setZ(std::max(.5f, std::min(6.f, scale->z() + zoom * dt * 0.0003f)));
//         // FPSCameraController 处理
//         m_fpsCamera->tick(m_input, dt);
//         // ArcBall处理
//         m_arcBall->tick(m_input, m_fpsCamera->getFront(), dt);
//     }
// }

// void Renderer::render_tick(QOpenGLContext *context)
// {
//     context = QOpenGLContext::currentContext();

//     RenderAPI::getInstance()->enableDepth(context, true);
//     RenderAPI::getInstance()->clearAll(context);
//     if (nullptr == m_meshesData)
//         return;

//     // 渲染天空
//     auto shSky = m_skyShader->sprog;
//     shSky->bind();
//     shSky->setUniformValue("_fov_rad", m_meshesData->camera->fov * 3.1415926f / 180.f);
//     shSky->setUniformValue("_sky", m_skyData->sky);
//     shSky->setUniformValue("_ground", m_skyData->ground);
//     shSky->setUniformValue("_pitch", trans_camera.rotation.toEulerAngles().x() * 3.1415926f / 180.f);
//     m_skyData->triangleData->bind(shSky.get());
//     RenderAPI::getInstance()->drawTriangle(context, m_skyData->triangleData->triangle_nums);

//     RenderAPI::getInstance()->clearDepth(context);

//     std::shared_ptr<QOpenGLShaderProgram> shPhong = nullptr;
//     if (m_meshesData->triangleDatas.size() > 0 && m_meshesData->boneData == nullptr) {
//         // 渲染模型
//         shPhong = m_staticShader->sprog;
//     } else {
//         shPhong = m_animationShader->sprog;
//     }
//     shPhong->bind();
//     // 模型变换
//     auto modelMat = trans_model.get_trans_mat();
//     shPhong->setUniformValue("_model", modelMat);
//     shPhong->setUniformValue("_normal", modelMat.normalMatrix());
//     // 相机变换
//     auto camera = m_meshesData->camera;
//     shPhong->setUniformValue("_view", camera->get_view_mat(trans_camera));
//     shPhong->setUniformValue("_proj", camera->get_projection_mat());
//     // 材质
//     shPhong->setUniformValue("_material.albedo", {0.8f, 0.8f, 0.8f});
//     shPhong->setUniformValue("_material.diffuse", {1.0f, 1.0f, 1.0f});
//     shPhong->setUniformValue("_material.specular", {1.0f, 1.0f, 1.0f});
//     shPhong->setUniformValue("_material.shininess", 16.f);
//     // 直射光1
//     shPhong->setUniformValue("_light_1.intensity", 1.0f);
//     shPhong->setUniformValue("_light_1.ambient", {0.4f, 0.4f, 0.4f});
//     shPhong->setUniformValue("_light_1.diffuse", {0.8f, 0.8f, 0.8f});
//     shPhong->setUniformValue("_light_1.specular", {0.9f, 0.9f, 0.9f});
//     shPhong->setUniformValue("_light_1.position", {0.1f, 0.5f, 1.5f});
//     shPhong->setUniformValue("_light_1._constant",  1.0f);
//     shPhong->setUniformValue("_light_1._linear",    0.22f);
//     shPhong->setUniformValue("_light_1._quadratic", 0.2f);
//     // 直射光2
//     shPhong->setUniformValue("_light_2.intensity", 1.0f);
//     shPhong->setUniformValue("_light_2.ambient", {0.1f, 0.1f, 0.1f});
//     shPhong->setUniformValue("_light_2.diffuse", {0.2f, 0.3f, 0.48f});
//     shPhong->setUniformValue("_light_2.specular", {0.2f, 0.3f, 0.48f});
//     shPhong->setUniformValue("_light_2.position", {-0.2f, 2.3f, 0.5f});
//     shPhong->setUniformValue("_light_2._constant",  1.0f);
//     shPhong->setUniformValue("_light_2._linear",    0.22f);
//     shPhong->setUniformValue("_light_2._quadratic", 0.2f);
//     // 直射光3
//     shPhong->setUniformValue("_light_3.intensity", 1.0f);
//     shPhong->setUniformValue("_light_3.ambient", {0.1f, 0.1f, 0.1f});
//     shPhong->setUniformValue("_light_3.diffuse", {0.4f, 0.3f, 0.2f});
//     shPhong->setUniformValue("_light_3.specular", {0.4f, 0.3f, 0.2f});
//     shPhong->setUniformValue("_light_3.position", {0.2f, -2.3f, 0.5f});
//     shPhong->setUniformValue("_light_3._constant",  1.0f);
//     shPhong->setUniformValue("_light_3._linear",    0.22f);
//     shPhong->setUniformValue("_light_3._quadratic", 0.2f);
//     if (m_meshesData->triangleDatas.size() > 0 && m_meshesData->boneData == nullptr) {
//         // 绘制静态模型
//         for (auto tri_data : m_meshesData->triangleDatas) {
//             tri_data->bind(shPhong.get());
//             RenderAPI::getInstance()->drawTriangle(context, tri_data->triangle_nums);
//         }
//     } else {
//         // 绘制动画
//         m_meshesData->boneData->bind(shPhong.get());
//         RenderAPI::getInstance()->drawTriangle(context, m_meshesData->boneData->triangle_nums);
//     }
// }

// void Renderer::pop_input()
// {
//     m_input = InputData{};
// }

// void Renderer::setRenderData(std::shared_ptr<res::Model> model)
// {
//     m_meshesData.reset();
//     m_meshesData = std::make_shared<RenderData>(model, m_sw, m_sh);
//     emit onRenderDataSet();
// }

// void Renderer::setRenderData(std::shared_ptr<res::BVH> bvh)
// {
//     m_meshesData.reset();
//     m_meshesData = std::make_shared<RenderData>(bvh, m_sw, m_sh);
//     emit onRenderDataSet();
// }

// void Renderer::clearRenderData()
// {
//     m_meshesData.reset();
//     m_meshesData = nullptr;
//     emit onRenderDataCleared();
// }

// void Renderer::resetAllTransform()
// {
//     trans_camera = defaultCameraTrans;
//     trans_model = defaultModelTrans;
//     m_arcBall->reset();
// }

// Transform Renderer::getModelTransform() const
// {
//     return trans_model;
// }

// Transform Renderer::getCameraTransform() const
// {
//     return trans_camera;
// }

// void Renderer::setModelTransform(const Transform &trans)
// {
//     trans_model = trans;
//     m_arcBall->reset();
// }

// void Renderer::setCameraTransform(const Transform &trans)
// {
//     trans_camera = trans;
//     m_fpsCamera->reset();
// }

// float Renderer::getBVHSampleFreq() const
// {
//     if (m_meshesData)
//         return m_meshesData->boneData ? m_meshesData->boneData->bvh->ssp : 1.0f;
//     else
//         return 1.0f;
// }

// void Renderer::setBVHSampleFreq(float spp)
// {
//     if (m_meshesData->boneData)
//     {
//         m_meshesData->boneData->bvh->ssp = spp;
//     }
// }

// int Renderer::getBVHFrames() const
// {
//     if (m_meshesData)
//         return m_meshesData->boneData ? m_meshesData->boneData->bvh->nFrames : 0;
//     else
//         return 0;
// }

// void Renderer::setBVHFrames(int frames)
// {
//     if (m_meshesData->boneData)
//     {
//         m_meshesData->boneData->bvh->nFrames = frames;
//     }
// }
// #include "assetimporter.h"

// #include <QJsonArray>
// #include <QJsonDocument>
// #include <QDebug>
// #include <QFileInfo>

// #include "function/network/client.h"
// #include "function/network/protocal.h"
// #include "function/metacategory.h"
// #include "function/assetloader/modelloader.h"
// #include "function/assetloader/modelconverter.h"
// #include "utils/jobsystem.h"

// AssetImporter::AssetImporter(ImportType type, QObject *parent)
//     : QObject{parent}, m_type(type)
// {
//     m_client = new Client(this);

//     connect(m_client, &Client::onSendingStateChanged, this, &AssetImporter::onResponsing);

//     connect(m_client, &Client::onReadOver, this, [=](const QJsonObject &data, const QByteArray &extraData) {
//         auto upload_cost = std::chrono::duration<float>(std::chrono::steady_clock::now() - m_upload_start).count();
//         auto response_type = static_cast<Protocal::HeaderField>(data["type"].toInt());
//         auto status = data["status"].toString();
//         if (Protocal::HeaderField::RESPONSE_ERROR == response_type) {
//             emit onResponsing(tr("服务请求错误. Info:") + status, false);
//         } else {
//             if (!status.isEmpty()) {
//                 if (Protocal::HeaderField::RESPONSE_UPLOADMODEL == response_type) {
//                     emit onResponsing(tr("上传失败. Info: ") + status, false);
//                 } else if (Protocal::HeaderField::REQUEST_UPLOADMODEL_SIMPLE == response_type){
//                     emit onResponsing(tr("简单上传失败. Info: ") + status, false);
//                 }
//             } else {
//                 if (Protocal::HeaderField::RESPONSE_UPLOADMODEL == response_type) {
//                     emit onResponsing(tr("上传成功, 耗时 ") + QString().asprintf("%f", upload_cost) + tr(" 秒"), false);
//                     emit onUploadSuccessful();
//                 } else if (Protocal::HeaderField::RESPONSE_UPLOADMODEL_SIMPLE == response_type){
//                     emit onResponsing(tr("简单上传成功, 耗时 ") + QString().asprintf("%f", upload_cost) + tr(" 秒"), false);
//                     emit onUploadSimpleSuccessful();
//                 }
//             }
//         }
//     });

//     m_typeManager = new TypeManager(type, this);
//     m_tagsManager = new TagsManager(type, this);

//     connect(m_typeManager, &TypeManager::onResponsing, this, &AssetImporter::onResponsing);
//     connect(m_tagsManager, &TagsManager::onResponsing, this, &AssetImporter::onResponsing);

//     connect(m_typeManager, &TypeManager::onPullSuccessful, this, [=](){
//         is_typeLoaded = true;
//         if (is_tagsLoaded) {
//             addTypeAndTagsData();
//         }
//     }, Qt::QueuedConnection);
//     connect(m_tagsManager, &TagsManager::onPullSuccessful, this, [=](){
//         is_tagsLoaded = true;
//         if (is_typeLoaded) {
//             addTypeAndTagsData();
//         }
//     }, Qt::QueuedConnection);

//     m_info.insert("category", MetaCategory::getInstance()->getCategoryInt());
//     // TODO: 连接MetaCategory修改信号
//     connect(MetaCategory::getInstance(), &MetaCategory::onCategoryModyfied, this, [=](){
//         this->m_info["category"] = MetaCategory::getInstance()->getCategoryInt();
//     });

//     QJsonArray headers;
//     headers << res::AssetInfo::toHeaderElement("name",          false,  false,  true);
//     headers << res::AssetInfo::toHeaderElement("type",          true,   true,   false);
//     headers << res::AssetInfo::toHeaderElement("tags",          true,   true,   false);
//     headers << res::AssetInfo::toHeaderElement("fileType",      false,  false,  true);
//     headers << res::AssetInfo::toHeaderElement("path",          false,  false,  true);
//     headers << res::AssetInfo::toHeaderElement("trans_model",   false,  false,  false);
//     headers << res::AssetInfo::toHeaderElement("trans_camera",      false,  false,  false);
//     QJsonArray data;
//     m_info.insert("headers", headers);
//     m_info.insert("data", data);

//     m_info["assetType"] = static_cast<int>(m_type);
// }

// AssetImporter::~AssetImporter()
// {
// //    qDebug() << "AssetImporter::~AssetImporter";
// }

// void AssetImporter::addPathsNotExist(const QStringList &filePaths)
// {
//     int addition_count = 0;
//     for (const auto &filePath : filePaths) {
//         if (!has(filePath)) {
//             m_filePathDict.insert(filePath.toStdString());
//             m_filePaths << filePath;
//             auto data = m_info["data"].toArray();

//             auto fileInfo = QFileInfo(filePath);
//             QJsonObject item;
//             item.insert("name", fileInfo.baseName());

//             QString fileType;
//             if (m_type == ImportType::MODEL || m_type == ImportType::BVH) {
//                 fileType = fileInfo.filePath().split('.').back();
//             } else {
//                 fileType = fileInfo.filePath().split('.').back();
//             }
//             item.insert("fileType", fileType);
//             item.insert("path", filePath);

//             data << item;
//             m_info["data"] = data;
//             addition_count++;
//         }
//     }
// //    qDebug() << m_info;
//     if (addition_count)
//         emit onAddPaths();
// }

// void AssetImporter::clear()
// {
//     m_filePathDict.clear();
//     m_filePaths.clear();
//     m_info["data"] = QJsonArray();
//     emit onClear();
// }

// void AssetImporter::alignToOrigin()
// {
//     if (m_type == ImportType::EFFECT) {
//         emit onResponsing("AssetImporter::alignToOrigin>> 特效资源不可执行对齐原点操作", false);
//         qDebug() << "AssetImporter::alignToOrigin>> 特效资源不可执行对齐原点操作";
//         return;
//     }

//     emit onResponsing(QString().asprintf("开始执行对齐原点操作, 共%d个模型", m_filePaths.size()), false);
//     emit onStartAlignToOrigin(m_filePaths.size());

//     for (auto &filePath : m_filePaths) {
//         auto info = QFileInfo(filePath);
//         auto baseName = info.baseName();
//         auto path = info.absolutePath();
//         auto savedName = path + '/' + baseName + ".obj";

//         m_filePathDict.erase(m_filePathDict.find(filePath.toStdString()));
//         m_filePathDict.insert(savedName.toStdString());

//         filePath = savedName;

//         JobSystem::getInstance()->submit([=](){
//             auto convertResult = ModelConverter::toOBJ(info.absoluteFilePath(), savedName);

//             int doneCount = m_saveCount.load() + 1;
//             this->m_saveCount++;

//             if (!convertResult) {
//                 emit onResponsing(QString().asprintf("模型 '%s' 转换失败", info.absoluteFilePath().toStdString().c_str()), false);
//             }

//             emit onResponsing(QString().asprintf("完成 %d / %d 个模型 '%s'", doneCount, m_filePaths.size(), baseName.toStdString().c_str()), true);

//             emit onDoneAlignToOrigin(doneCount);

//             if (m_saveCount == m_filePaths.size()) {
//                 auto data = m_info["data"].toArray();

//                 for (int i = 0; i < data.size(); i++) {
//                     auto row = data[i].toObject();
//                     row["path"] = m_filePaths[i];
//                     row["fileType"] = "obj";
//                     data[i] = row;
//                 }

//                 m_info["data"] = data;

//                 emit saveSuccessful();
//             }
//         });

//     }
// }

// void AssetImporter::initAllTransform(const Transform &trans_model, const Transform &trans_camera)
// {
//     auto data = m_info["data"].toArray();
//     for (auto row : data) {
//         auto obj = row.toObject();
//         obj["trans_model"] = Transform::toJson(trans_model);
//         obj["trans_camera"] = Transform::toJson(trans_camera);
//         row = obj;
//     }
//     m_info["data"] = data;
// }

// void AssetImporter::saveTransform(int row, const Transform &trans_model, const Transform &trans_camera)
// {
//     auto data = m_info["data"].toArray();
//     auto obj = data[row].toObject();
//     obj["trans_model"] = Transform::toJson(trans_model);
//     obj["trans_camera"] = Transform::toJson(trans_camera);
//     data[row] = obj;
//     m_info["data"] = data;
// }

// std::pair<Transform, Transform> AssetImporter::getTransform(int row) const
// {
//     auto data = m_info["data"].toArray();
//     auto obj = data[row].toObject();
//     return {
//         Transform::fromJson(obj["trans_model" ].toArray()),
//         Transform::fromJson(obj["trans_camera"].toArray())
//     };
// }

// void AssetImporter::pullTypeAndTags()
// {
//     auto headers = m_info["headers"].toArray();
//     for (auto header : headers) {
//         auto obj = header.toObject();
//         if (obj["name"].toString() == "type" || obj["name"].toString() == "tags") {
//             obj["visible"] = true;
//             header = obj;
//         }
//     }
//     m_info["headers"] = headers;
//     m_typeManager->pull();
//     m_tagsManager->pull();
// }

// void AssetImporter::upload()
// {
//     m_info["type"] = static_cast<int>(Protocal::HeaderField::REQUEST_UPLOADMODEL);
//     __upload();
// }

// void AssetImporter::upload_simple()
// {
//     m_info["type"] = static_cast<int>(Protocal::HeaderField::REQUEST_UPLOADMODEL_SIMPLE);
//     __upload();
// }

// QStringList AssetImporter::getFilePaths(const std::vector<int> &index) const
// {
//     QStringList filePaths;
//     for (const auto &id : index) {
//         filePaths << m_filePaths[id];
//     }
//     return filePaths;
// }

// QStringList AssetImporter::getFileNames(const std::vector<int> &index) const
// {
//     QStringList fileNames;
//     for (const auto &id : index) {
//         auto fileInfo = QFileInfo(m_filePaths[id]);
//         fileNames << fileInfo.baseName();
//     }
//     return fileNames;
// }

// void AssetImporter::addTypeAndTagsData()
// {
//     auto data = m_info["data"].toArray();
//     for (auto row : data) {
//         auto obj = row.toObject();
//         obj["type"] = QJsonObject{{"array", m_typeManager->getTypesNameList()},  {"value", ""}};
//         obj["tags"] = QJsonObject{{"array", m_tagsManager->getTagsNameList()},   {"value", ""}};
//         row = obj;
//     }
//     m_info["data"] = data;
//     emit onTypeAndTagsLoaded();
// }

// void AssetImporter::__upload()
// {
//     m_upload_start = std::chrono::steady_clock::now();
//     switch (m_type) {
//     case ImportType::MODEL:  emit onResponsing(tr("模型资源简单上传开始"), true); break;
//     case ImportType::BVH:    emit onResponsing(tr("骨骼动画资源简单上传开始"), true); break;
//     case ImportType::EFFECT: emit onResponsing(tr("特效资源简单上传开始"), true); break;
//     }
//     m_client->sendData(m_info);
// }

// #include "bvhloader.h"

// #include <cmath>
// #include <QMatrix4x4>
// #include <QTransform>
// #include <QFileInfo>
// #include <QDebug>

// #include "assimp/Importer.hpp"
// #include "assimp/scene.h"
// #include "assimp/postprocess.h"
// #include "resource/bvh.h"
// #include "utils/jobsystem.h"

// BVHLoader::BVHLoader(QObject *parent) : AssetCache(parent)
// {
//     connect(this, &AssetCache::onCacheTimeout, this, [this](const QString& filePath) {
//         auto filePath_std = filePath.toStdString();
//         auto model_asset = BVHManager::getInstance()->get(filePath_std);
//         if (model_asset.use_count() <= 2) {
//             BVHManager::getInstance()->remove(filePath_std);
//             this->removeCache(filePath);
//         } else {
//             this->restartCache(filePath);
//         }
//     });
// }

// void readNode(aiNode *node, const std::function<void(aiNode *)> &processor)
// {
//     // 先序遍历
//     processor(node);
//     for (size_t i = 0; i < node->mNumChildren; i++) {
//         readNode(node->mChildren[i], processor);
//     }
// }

// void readBone(res::Bone *bone, const std::function<void(res::Bone *)> &proc)
// {
//     proc(bone);
//     for (auto child : bone->children) {
//         readBone(child, proc);
//     }
// }

// aiBone* findChildBone(aiBone *bone, std::unordered_map<std::string, aiNode *> &node_map, std::unordered_map<std::string, aiBone *> &bone_map)
// {
//     if (node_map.find(bone->mName.C_Str()) != node_map.end()) {
//         auto child_node = node_map[bone->mName.C_Str()];
//         for (size_t k = 0; k < child_node->mNumChildren; k++) {
//             auto child_name = child_node->mChildren[k]->mName.C_Str();
//             if (bone_map.find(child_name) != bone_map.end()) {
//                 return bone_map[child_name];
//             }
//         }
//     }
//     return nullptr;
// }

// std::shared_ptr<res::BVH> BVHLoader::loadBVH(const QString &filePath)
// {
//     Assimp::Importer importer;
//     auto importFlag = aiProcessPreset_TargetRealtime_Fast;

//     auto scene = importer.ReadFile(filePath.toStdString(), importFlag);

//     if (!scene) {
//         qDebug() << "BVHLoader>> Assimp Scene" << filePath << "Load Failed";
//         return nullptr;
//     }

//     if (!scene->HasMeshes()) {
//         qDebug() << "BVHLoader>> Assimp Scene" << filePath << "DONNT Have Any Meshes";
//         return nullptr;
//     }

//     auto bvh = std::make_shared<res::BVH>();

//     std::unordered_map<std::string, aiNode *> node_map;
//     std::unordered_map<std::string, aiNodeAnim *> joint_map;
//     std::unordered_map<std::string, std::shared_ptr<res::Bone>> bone_map;

//     readNode(scene->mRootNode, [=, &node_map](aiNode *node){
//         node_map.insert({node->mName.C_Str(), node});
//     });

//     unsigned int nFrames = 0;

//     if (scene->mNumAnimations < 1) {
//         qDebug("BVHLoader>> Assimp Scene Don't Have Any Animation Data.");
//         return nullptr;
//     }

//     auto animation = scene->mAnimations[0];

// //    qDebug("Duration=%lf, Ticks=%lf", animation->mDuration, animation->mTicksPerSecond);

//     for (size_t j = 0; j < animation->mNumChannels; j++) {
//         auto channel = animation->mChannels[j];
//         joint_map.insert({channel->mNodeName.C_Str(), channel});
//         nFrames = std::max(nFrames, channel->mNumPositionKeys);
//         nFrames = std::max(nFrames, channel->mNumRotationKeys);
//         nFrames = std::max(nFrames, channel->mNumScalingKeys);
//     }

//     bvh->ssp = animation->mTicksPerSecond;
//     bvh->nFrames = nFrames;

//     res::Bone *rootBone = nullptr;

//     QVector3D pmin{std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
//     QVector3D pmax{std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min()};

//     readNode(scene->mRootNode, [=, &pmin, &pmax, &rootBone, &joint_map, &bone_map](aiNode *node) {
//         // If the node is marked as necessary, copy it into the skeleton and check its children.
//         // If the node is marked as not necessary, skip it and do not iterate over its children.
//         if (joint_map.find(node->mName.C_Str()) != joint_map.end()) {
//             auto cur_joint = joint_map[node->mName.C_Str()];
//             auto bone = std::make_shared<res::Bone>();
//             if (!rootBone)
//                 rootBone = bone.get();
//             bone_map.insert({node->mName.C_Str(), bone});

//             aiNode *parent = node->mParent;
//             while (parent) {
//                 // Find parent joint
//                 if (bone_map.find(parent->mName.C_Str()) != bone_map.end()) {
//                     break;
//                 }
//                 parent = parent->mParent;
//             }
//             std::shared_ptr<res::Bone> bone_parent = parent ? bone_map[parent->mName.C_Str()] : nullptr;
//             if (bone_parent)
//                 bone_parent->children.emplace_back(bone.get());

//             for (unsigned int k = 0; k < nFrames; k++) {
//                 unsigned int pk = std::min(k, cur_joint->mNumPositionKeys - 1);
//                 QVector3D position {cur_joint->mPositionKeys[pk].mValue.x, cur_joint->mPositionKeys[pk].mValue.y, cur_joint->mPositionKeys[pk].mValue.z};
//                 unsigned int rk = std::min(k, cur_joint->mNumRotationKeys - 1);
//                 QQuaternion rotation {cur_joint->mRotationKeys[rk].mValue.w, cur_joint->mRotationKeys[rk].mValue.x, cur_joint->mRotationKeys[rk].mValue.y,cur_joint->mRotationKeys[rk].mValue.z};
//                 unsigned int sk = std::min(k, cur_joint->mNumScalingKeys - 1);
//                 QVector3D scale    {cur_joint->mScalingKeys[sk].mValue.x,  cur_joint->mScalingKeys[sk].mValue.y,  cur_joint->mScalingKeys[sk].mValue.z};

//                 QMatrix4x4 trans;
//                 trans.translate(position);
//                 trans.rotate(rotation);
//                 trans.scale(scale);
//                 if (nullptr != bone_parent)
//                     trans = bone_parent->trans_mats[k] * trans;
//                 bone->trans_mats.emplace_back(trans);

//                 QVector3D pos{trans.data()[12],trans.data()[13],trans.data()[14]};
//                 pmin.setX(std::min(pos.x(), pmin.x()));
//                 pmin.setY(std::min(pos.y(), pmin.y()));
//                 pmin.setZ(std::min(pos.z(), pmin.z()));
//                 pmax.setX(std::max(pos.x(), pmax.x()));
//                 pmax.setY(std::max(pos.y(), pmax.y()));
//                 pmax.setZ(std::max(pos.z(), pmax.z()));

//             }
//         }
//     });

//     QVector3D centroid = (pmin + pmax) * 0.5f;
//     float inv_semi_diagonal = 1.f / ((pmax - pmin).length() * 0.5f);
//     QMatrix4x4 obj2org;
//     obj2org.scale({inv_semi_diagonal, inv_semi_diagonal, inv_semi_diagonal});
//     obj2org.translate(-centroid);
//     obj2org.rotate({.0f, .0f, .0f, .0f});

//     for (size_t k = 0; k < nFrames; k++) {
//         auto boneMesh = std::make_shared<res::BoneMesh>(bone_map.size());
//         readBone(rootBone, [=](res::Bone *bone) {
//             for (auto child : bone->children) {
//                 (*boneMesh) << res::BoneMesh(obj2org * bone->trans_mats[k], obj2org * child->trans_mats[k]);
//             }
//         });
//         bvh->boneMeshes.emplace_back(boneMesh);
//     }

//     return bvh;
// }

// void BVHLoader::cachedAsyncLoad(const QString &filePath, std::function<void (bool)> loadCallBack)
// {
//     if (!BVHManager::getInstance()->has(filePath.toStdString())) {
//         JobSystem::getInstance()->submit([filePath, loadCallBack, this]() {
//             auto fileExt = filePath.split('.').back();
//             std::shared_ptr<res::BVH> bvh;
//             if (fileExt == "bvh") {
//                 bvh = BVHLoader::getInstance()->loadBVH(filePath);
//             } else {
//                 qDebug() << "BVHLoader::cachedAsyncLoad>> Unsupported BVH" << filePath << "(.bvh is Legal)";
//                 loadCallBack(false);
//                 return;
//             }
//             if (bvh == nullptr) {
//                 qDebug() << "BVHLoader::cachedAsyncLoad>> BVH" << filePath << "Load Failed";
//                 loadCallBack(false);
//                 return;
//             }
//             BVHManager::getInstance()->add(filePath.toStdString(), bvh);
//             emit onAssetLoaded(filePath);
//             loadCallBack(true);
//         });
//     } else {
//         emit onAssetLoaded(filePath);
//         loadCallBack(true);
//     }
// }

// void BVHLoader::tempAsyncLoad(const QString &filePath, std::function<void (std::shared_ptr<res::BVH>)> loadCallBack)
// {
//     JobSystem::getInstance()->submit([filePath, loadCallBack]() {
//         auto fileExt = filePath.split('.').back();
//         std::shared_ptr<res::BVH> model = nullptr;
//         if (fileExt == "bvh") {
//             model = BVHLoader::getInstance()->loadBVH(filePath);
//         } else {
//             qDebug() << "BVHLoader::tempAsyncLoad>> Unsupported BVH" << filePath << "(.bvh is Legal)";
//         }
//         if (model == nullptr) {
//             qDebug() << "BVHLoader::tempAsyncLoad>> BVH" << filePath << "Load Failed";
//         }
//         loadCallBack(model);
//     });
// }

// #include "modelloader.h"

// #include <cmath>
// #include <QMatrix4x4>
// #include <QFileInfo>
// #include <QDebug>

// #include "utils/jobsystem.h"
// #include "assimp/Importer.hpp"
// #include "assimp/scene.h"
// #include "assimp/postprocess.h"

// using namespace res;

// ModelLoader::ModelLoader(QObject *parent) : AssetCache(parent)
// {
//     connect(this, &AssetCache::onCacheTimeout, this, [this](const QString& filePath) {
//         auto filePath_std = filePath.toStdString();
//         auto model_asset = ModelManager::getInstance()->get(filePath_std);
//         if (model_asset.use_count() <= 2) {
//             ModelManager::getInstance()->remove(filePath_std);
//             this->removeCache(filePath);
//         } else {
//             // 尝试重新清理
//             this->restartCache(filePath);
//         }
//     });
// }

// std::shared_ptr<Model> ModelLoader::loadFBX(const QString &filePath, bool doGlobalTransform, bool doNormalize)
// {
//     Assimp::Importer importer;
//     if (doNormalize)
//         importer.SetPropertyBool(AI_CONFIG_PP_PTV_NORMALIZE, true);
//     auto importFlag = aiProcess_CalcTangentSpace |
//                       aiProcess_GenNormals |
//                       aiProcess_Triangulate |
//                       aiProcess_JoinIdenticalVertices |
//                       aiProcess_SortByPType;
//     if (doGlobalTransform)
//         importFlag |= aiProcess_PreTransformVertices;
//     auto scene = importer.ReadFile(filePath.toStdString(), importFlag);

//     if (!scene) {
//         qDebug() << "ModelLoader::loadFBX>> Assimp Scene" << filePath << "Load Failed";
//         return nullptr;
//     }

//     if (!scene->HasMeshes()) {
//         qDebug() << "ModelLoader::loadFBX>> Assimp Scene" << filePath << "DONNT Have Any Meshes";
//         return nullptr;
//     }

//     auto model = std::make_shared<Model>();

//     auto nMeshes = scene->mNumMeshes;

//     for (unsigned int k = 0; k < nMeshes; k++) {
//         auto mesh = std::make_shared<Mesh>();

//         auto cur_mesh = scene->mMeshes[k];

//         auto nVert = cur_mesh->mNumVertices;
//         auto nIndi = cur_mesh->mNumFaces * cur_mesh->mFaces->mNumIndices;

//         mesh->vertices = std::vector<QVector3D>(nVert);
//         mesh->normals = std::vector<QVector3D>(nVert);
//         mesh->uvs = std::vector<QVector2D>(nVert);
//         mesh->indices = std::vector<unsigned int>(nIndi);

//         auto faces = cur_mesh->mFaces;

//         for (unsigned int i = 0; i < cur_mesh->mNumVertices; i++) {
//             mesh->vertices[i].setX(cur_mesh->mVertices[i].x);
//             mesh->vertices[i].setY(cur_mesh->mVertices[i].y);
//             mesh->vertices[i].setZ(cur_mesh->mVertices[i].z);
//             mesh->normals [i].setX(cur_mesh->mNormals [i].x);
//             mesh->normals [i].setY(cur_mesh->mNormals [i].y);
//             mesh->normals [i].setZ(cur_mesh->mNormals [i].z);
//         }

//         for (size_t i = 0; i < cur_mesh->mNumFaces; i++) {
//             auto face = faces[i];
//             mesh->indices[i * 3    ] = face.mIndices[0];
//             mesh->indices[i * 3 + 1] = face.mIndices[1];
//             mesh->indices[i * 3 + 2] = face.mIndices[2];
//         }

//         model->meshes.push_back(mesh);
//     }

//     return model;
// }

// std::shared_ptr<Model> ModelLoader::loadOBJ(const QString &filePath, bool doNormalize)
// {
//     return loadFBX(filePath, true, doNormalize);
// }

// std::shared_ptr<Model> ModelLoader::loadOFF(const QString &filePath, bool doNormalize)
// {
//     return loadFBX(filePath, true, doNormalize);
// }

// void ModelLoader::cachedAsyncLoad(const QString &filePath, std::function<void(bool)> loadCallBack)
// {
//     if (!ModelManager::getInstance()->has(filePath.toStdString())) {
//         // load FBX file in async Job
//         JobSystem::getInstance()->submit([filePath, loadCallBack, this]() {
//             auto fileExt = filePath.split('.').back();
//             std::shared_ptr<res::Model> model;
//             if (fileExt == "fbx") {
//                 model = ModelLoader::getInstance()->loadFBX(filePath, true, false);
//             } else if (fileExt == "obj") {
//                 model = ModelLoader::getInstance()->loadOBJ(filePath, false);
//             } else if (fileExt == "off") {
//                 model = ModelLoader::getInstance()->loadOFF(filePath, false);
//             } else {
//                 qDebug() << "ModelLoader::asyncLoad>> Unsupported Model" << filePath << "(.fbx, .obj, .off is Legal)";
//                 loadCallBack(false);
//                 return;
//             }
//             if (model == nullptr) {
//                 qDebug() << "ModelLoader::asyncLoad>> Model" << filePath << "Load Failed";
//                 loadCallBack(false);
//                 return;
//             }
//             ModelManager::getInstance()->add(filePath.toStdString(), model);
//             emit onAssetLoaded(filePath);
//             loadCallBack(true);
//         });
//     } else {
//         emit onAssetLoaded(filePath);
//         loadCallBack(true);
//     }
// }

// void ModelLoader::tempAsyncLoad(const QString &filePath, std::function<void (std::shared_ptr<res::Model>)> loadCallBack)
// {
//     JobSystem::getInstance()->submit([filePath, loadCallBack]() {
//         auto fileExt = filePath.split('.').back();
//         std::shared_ptr<res::Model> model = nullptr;
//         if (fileExt == "fbx") {
//             model = ModelLoader::getInstance()->loadFBX(filePath, true, false);
//         } else if (fileExt == "obj") {
//             model = ModelLoader::getInstance()->loadOBJ(filePath, false);
//         } else if (fileExt == "off") {
//             model = ModelLoader::getInstance()->loadOFF(filePath, false);
//         } else {
//             qDebug() << "ModelLoader::tempAsyncLoad>> Unsupported Model" << filePath << "(.fbx, .obj, .off is Legal)";
//         }
//         if (model == nullptr) {
//             qDebug() << "ModelLoader::tempAsyncLoad>> Model" << filePath << "Load Failed";
//         }
//         loadCallBack(model);
//     });
// }
// #include "assetcache.h"
// #include "function/configer/configmanager.h"

// #include <QDebug>

// AssetCache::AssetCache(QObject *parent) : QObject(parent)
// {
//     m_cache_timeout = 1000 * 60 * ConfigManager::getInstance()->getConfig("Asset/CacheTimeout", 1.f).toInt();
//     m_cache_timeout_retry = 1000 * 60 * ConfigManager::getInstance()->getConfig("Asset/CacheTimeoutRetry", .5f).toInt();

//     connect(this, &AssetCache::onAssetLoaded, this, &AssetCache::cacheStart);

//     connect(ConfigManager::getInstance(), &ConfigManager::onConfModified, this, [this](){
//         m_cache_timeout = 1000 * 60 * ConfigManager::getInstance()->getConfig("Asset/CacheTimeout").toInt();
//         m_cache_timeout_retry = 1000 * 60 * ConfigManager::getInstance()->getConfig("Asset/CacheTimeoutRetry").toInt();
//     });

// }

// void AssetCache::removeCache(const QString &filePath)
// {
//     remove(filePath.toStdString());
// }

// void AssetCache::restartCache(const QString &filePath)
// {
//     get(filePath.toStdString())->start(m_cache_timeout_retry);
// }


// void AssetCache::cacheStart(const QString &filePath)
// {
// //    qDebug() << "AssetCache::cacheStart>>" << filePath;
//     auto filePath_std = filePath.toStdString();
//     QTimer *timer;
//     if (this->has(filePath_std)) {
//         timer = this->get(filePath_std).get();
//     } else {
//         timer = this->add(filePath_std, std::make_shared<QTimer>()).get();
//         connect(timer, &QTimer::timeout, this, [=]() {
// //            qDebug() << "AssetCache::QTimer::timeout>> " << filePath;
//             timer->stop();
//             emit this->onCacheTimeout(filePath);
//         }, Qt::DirectConnection);
//     }
//     timer->stop();
//     timer->start(m_cache_timeout);
// }
// #include "client.h"

// #include <QDebug>
// #include <QTimerEvent>

// #include "utils/jobsystem.h"
// #include "function/configer/configmanager.h"
// #include "protocal.h"

// Client::Client(QObject *parent) : QObject(parent)
// {
//     m_socket = new QTcpSocket(this);
//     auto server_info_raw = ConfigManager::getInstance()->getConfigs({"Server/Addr", "Server/Port"});
//     m_host = server_info_raw[0].toString();
//     // TODO: port的配置最好都改成uint
//     m_port = server_info_raw[1].toInt();
//     connect(ConfigManager::getInstance(), &ConfigManager::onConfModified, this, [=](){
//         auto server_info_raw = ConfigManager::getInstance()->getConfigs({"Server/Addr", "Server/Port"});
//         m_host = server_info_raw[0].toString();
//         m_port =server_info_raw[1].toInt();
//     });

// //    connect(m_socket, &QTcpSocket::stateChanged, this, [=](QAbstractSocket::SocketState state) {
// //        qDebug() << "QTcpSocket::stateChanged>>" << state << "at port[" << m_socket->localPort() << "]";
// //    }, Qt::QueuedConnection);

//     connect(m_socket, &QTcpSocket::connected, this, [=]() {
//         emit onConnectSuccessful();
//     }, Qt::QueuedConnection);

// #if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
//     connect(m_socket,  QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error), this, [=](QAbstractSocket::SocketError error) {
//         emit onConnectError(error, m_socket->errorString());
//     }, Qt::QueuedConnection);
// #else
//     connect(m_socket,  &QTcpSocket::errorOccurred, this, [=](QAbstractSocket::SocketError error) {
//             emit onConnectError(error, m_socket->errorString());
//         }, Qt::QueuedConnection);
// #endif

//     connect(m_socket, &QTcpSocket::readyRead, this, [=](){
//         emit onReadReady();
//     });

//     connect(this, &Client::onConnectSuccessful, this, [=]() {
//         emit onSendingStateChanged(tr("服务器连接成功, 发送数据..."), true);
//     });
//     connect(this, &Client::onConnectError, this, [=](QAbstractSocket::SocketError, const QString &info) {
//         emit onSendingStateChanged(tr("与服务器断开连接, 信息: ") + info, false);
//     });
//     connect(this, &Client::onSendSuccessful, this, [=]() {
//         emit onSendingStateChanged(tr("数据发送成功, 接受服务响应..."), true);
//     });
//     connect(this, &Client::onSendFailed, this, [=]() {
//         emit onSendingStateChanged(tr("数据发送失败"), false);
//     });
//     connect(this, &Client::onReadReady, this, [=]() {
//         emit onSendingStateChanged(tr("正在接受服务响应..."), true);
//     });
//     connect(this, &Client::onReadOver, this, [=](const QJsonObject &data, const QByteArray &extraData) {
//         emit onSendingStateChanged(tr("服务响应接受成功"), true);
//     });

// }

// Client::~Client()
// {
//     m_socket->close();
// //    qDebug() << "Client::~Client";
// }

// void Client::sendData(const QJsonObject &data, const QByteArray &extraData)
// {

//     if (QTcpSocket::UnconnectedState != m_socket->state()) {
//         qDebug() << "Client::sendData>> 还没发完别急!!!!!!!!!!";
//         return;
//     }

//     ClientSession *session = new ClientSession(this);

//     connect(this, &Client::onConnectSuccessful, session, [=]() {
//         doSendData(data, extraData);
//     });

//     connect(this, &Client::onConnectError, session, [=](QAbstractSocket::SocketError, const QString &info) {
//         qDebug() << "Client::sendData>>Client::onConnectedFailed>>" << info;
//         session->deleteLater();
//     });

//     connect(this, &Client::onSendFailed, session, [=](){
//         qDebug() << "Client::onSendFailed";
//         m_socket->disconnectFromHost();
//         session->deleteLater();
//     });

//     connect(this, &Client::onReadReady, session, [=](){
//         doReceiveData(session);
//     });

//     connect(this, &Client::onReadOver, session, [=](){
//         session->deleteLater();
//     });

//     emit onSendingStateChanged(tr("正在连接服务器..."), true);
//     m_socket->connectToHost(m_host, m_port);

// }

// void Client::doSendData(const QJsonObject &data, const QByteArray &extraData)
// {
//     auto packed = Protocal::pack(data);
//     QByteArray json_bytes = QJsonDocument(packed).toJson();

//     // 先发送数据长度
//     uint data_len = json_bytes.size(), extra_len = extraData.size();
//     m_socket->write(Protocal::toByteArray(data_len) + Protocal::toByteArray(extra_len));

//     // 再发送真正的数据
//     uint len_sended = 0, len = data_len + extra_len;

// //    qDebug() << "length to send" << len;

//     while (len_sended < len) {
//         int ret = -1;

//         if (len_sended < data_len) {
//             ret = m_socket->write(json_bytes.data() + len_sended, data_len - len_sended);
//         } else {
//             ret = m_socket->write(extraData.data() + len_sended - data_len, len - len_sended);
//         }

//         if (-1 == ret) {
//             emit onSendFailed();
//             return;
//         }

//         len_sended += ret;

// //        qDebug() << "length sended" << len;

//         m_socket->flush();
//     }

//     emit onSendSuccessful();
// }

// void Client::doReceiveData(ClientSession *session)
// {
//     if (!session->head_received) // 首先读取数据长度信息
//     {
//         if (m_socket->bytesAvailable() >= 8) // 不能小于头字节数
//         {
//             // 读取头信息
//             QByteArray data_len_b, extra_len_b;
//             data_len_b = m_socket->read(4);
//             extra_len_b = m_socket->read(4);

//             session->data_len = Protocal::fromByteArray<uint>(data_len_b);
//             session->extra_len = Protocal::fromByteArray<uint>(extra_len_b);

// //            qDebug() << "length to receive " << session->data_len << session->extra_len;

//             if (0 == session->data_len && 0 == session->extra_len) {
//                 // 没有后续数据
//                 return;
//             }

//             // 读取头信息并标注头信息已经被读取
//             session->head_received = true;
//         } else {
//             return;
//         }
//     }

//     // 读取实际的数据
//     if (session->data.size() < session->data_len) {
//         session->data.append(m_socket->read(session->data_len - session->data.size()));
//     } else if (session->extra.size() < session->extra_len) {
//         session->extra.append(m_socket->read(session->extra_len - session->extra.size()));
//     }

// //    qDebug() << "length received " << session->data.size() << session->extra.size();

//     // 如果没有读完的话，还会再次进入槽函数
//     if (session->data.size() < session->data_len || session->extra.size() < session->extra_len)
//         return;

//     auto data_obj = QJsonDocument::fromJson(session->data).object();
//     auto data = Protocal::unpack(data_obj);
//     emit onReadOver(data, session->extra);
// }
// #include "protocal.h"

// #include "resource/assetinfo.h"

// Protocal::Protocal(QObject *parent) : QObject(parent)
// {

// }

// QJsonObject Protocal::pack(const QJsonObject &org)
// {
//     auto org_category   = org[  "category"  ].toInt();
//     auto org_type       = org[  "type"      ].toInt();
//     auto org_headers    = org[  "headers"   ].toArray();
//     auto org_data       = org[  "data"      ].toArray();

//     QJsonObject packed = org;

//     packed.insert( "category",   org_category);
//     packed.insert( "type",       org_type);

//     QJsonArray headers;
//     QJsonArray data;
//     for (const auto &header : org_headers) {
//         headers.append(header.toObject()["name"]);
//     }

//     for (const auto& row : org_data) {
//         QJsonObject packedRow;
//         for (const auto& header : org_headers) {
//             QString headerName = header.toObject()["name"].toString();
//             if (header.toObject()["is_array"].toBool()) {
//                 packedRow.insert(headerName, row.toObject()[headerName].toObject()["value"]);
//             } else {
//                 packedRow.insert(headerName, row.toObject()[headerName]);
//             }
//         }
//         data << packedRow;
//     }

//     packed.insert( "titles",     headers);
//     packed.insert( "values",     data);

//     QJsonArray block_size;
//     for (int i = 0; i < org_data.size(); i++) {
//         block_size.append(0);
//     }

//     packed.insert( "block_size", block_size);

//     return packed;
// }

// QJsonObject Protocal::unpack(const QJsonObject &packed)
// {
//     auto packed_type    =   packed          [   "type"      ].toInt();
//     auto packed_status  =   packed          [   "status"    ].toString();
//     auto packed_models  =   packed          [   "models"    ].toObject();

//     QJsonObject unpacked;

//     unpacked.insert( "type",    packed_type);
//     unpacked.insert( "status",  packed_status);

//     if (packed_models.size() == 0){
//         return unpacked;
//     }

//     auto packed_titles  =   packed_models   [   "titles"    ].toArray();
//     auto packed_values  =   packed_models   [   "values"    ].toArray();

//     QJsonArray data;

//     for (int i = 0; i < packed_values.size(); i++) {
//         auto value = packed_values[i].toArray();
//         QJsonObject data_element;
//         for (int j = 0; j < packed_titles.size(); j++) {
//             data_element.insert(packed_titles[j].toString(), value[j]);
//         }
//         data << data_element;
//     }

//     unpacked.insert( "headers",  packed_titles);
//     unpacked.insert( "data",  data);
//     return unpacked;
// }
