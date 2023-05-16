#include "homewindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <QDebug>
#include <QDialog>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QShowEvent>
#include <QDockWidget>

#include "optionwindow.h"
#include "importwindow.h"
#include "gui/uicomponent/modelsearchwidget.h"
#include "gui/uicomponent/categoryselector.h"
#include "gui/uicomponent/loggingwidget.h"
#include "gui/indexwindow.h"
#include "function/layoutmanager.h"
#include "gui/typemanagerwindow.h"
#include "gui/tagsmanagerwindow.h"

HomeWindow::HomeWindow(QWidget* parent, QApplication* current_app) : QMainWindow(parent)
{
    this->setWindowTitle(tr("主页 - 模型检索"));

    setWindowIcon(QIcon(":/assets/assets/icon/icon.jpg"));

    auto centerWidget = new QWidget(this);
    this->setCentralWidget(centerWidget);

    auto ly_total = new QVBoxLayout(centerWidget);

    makeMenu();

    auto cate_sel = new CategorySelector(centerWidget);
//    ly_total->addWidget(cate_sel);
    auto docker_cate = new QDockWidget(tr("检索 - 大类选择器"), centerWidget);
    docker_cate->setObjectName("docker_cate");
    docker_cate->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    docker_cate->setWidget(cate_sel);
    docker_cate->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::TopDockWidgetArea, docker_cate);

    auto logging_widget = new LoggingWidget(centerWidget);
    auto docker_logging = new QDockWidget(tr("检索 - 信息输出"), centerWidget);
    docker_logging->setObjectName("docker_logging");
    docker_logging->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    docker_logging->setWidget(logging_widget);
    docker_logging->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::RightDockWidgetArea, docker_logging);

    m_tabw = new QTabWidget(centerWidget);
    ly_total->addWidget(m_tabw);

    auto sw_content = new ModelSearchWidget(ModelSearch::SearchType::CONTENT,centerWidget);
    m_tabw->addTab(sw_content, "模型内容检索");
    auto sw_label = new ModelSearchWidget(ModelSearch::SearchType::TAGS,centerWidget);
    m_tabw->addTab(sw_label, "模型标签检索");
    auto sw_type = new ModelSearchWidget(ModelSearch::SearchType::TYPE,centerWidget);
    m_tabw->addTab(sw_type, "模型类型检索");

    connect(sw_content->getSearcher(), &ModelSearch::onResponsing, this, [=](const QString & info, bool is_continue){
        is_continue ? logging_widget->info(info) : logging_widget->warning(info);
    });
    connect(sw_label->getSearcher(), &ModelSearch::onResponsing, this, [=](const QString & info, bool is_continue){
        is_continue ? logging_widget->info(info) : logging_widget->warning(info);
    });

    connect(sw_type->getSearcher(), &ModelSearch::onResponsing, this, [=](const QString & info, bool is_continue){
        is_continue ? logging_widget->info(info) : logging_widget->warning(info);
    });

    this->resize(800, 600);

}

HomeWindow::~HomeWindow()
{
//    qDebug() << "HomeWindow Closed";
}

void HomeWindow::keyPressEvent(QKeyEvent *ev)
{
    // TODO: 方便调试
    if(ev->key() == Qt::Key::Key_Escape) {
        close();
    }
}

void HomeWindow::closeEvent(QCloseEvent *event)
{
    saveLayout();
    QWidget::closeEvent(event);
}

void HomeWindow::showEvent(QShowEvent *event)
{
    if (_is_first_paint) {
        _is_first_paint = false;
        restoreLayout();
    }
    update();
}

void HomeWindow::makeMenu()
{
    m_top_menubar = new QMenuBar(this);

    // ----------------文件----------------
    {
        auto menu_file = new QMenu(tr("文件(&F)"), this);
        m_top_menubar->addMenu(menu_file);

        // ----------------导入----------------
        menu_file->addAction(tr("资源导入(&I)"), this, [this](){
            auto winName = "资源导入";
            auto fm = FunctionWnidowManager::getInstance();
            if (!fm->show(winName, this->isMaximized(), true)) {
                fm->create(winName, new ImportWindow(this));
            }
        }, QKeySequence("Ctrl+I"));

        // ----------------选项----------------
        menu_file->addAction(tr("选项(&O)"), this, [this](){
                auto winName = "选项";
                auto fm = FunctionWnidowManager::getInstance();
                if (!fm->show(winName, this->isMaximized(), true)) {
                    fm->create(winName, new OptionWindow(this));
                }
        }, QKeySequence("Ctrl+O"));

        menu_file->addSeparator();

        // ----------------退出----------------
        menu_file->addAction(tr("退出(&X)"), this, [](){
            QApplication::exit(0);
        }, QKeySequence("Ctrl+w"));

    }

    // ----------------编辑----------------
    {
        auto menu_version = new QMenu(tr("编辑(&V)"), this);

        // ----------------类型----------------
        menu_version->addAction(tr("类型管理"), this, [=](){
            auto winName = "类型管理";
            auto fm = FunctionWnidowManager::getInstance();
            if (!fm->show(winName, this->isMaximized(), true)){
                fm->create(winName, new TypeManagerWindow(this));
            }
        });

        // ----------------标签----------------
        menu_version->addAction(tr("标签管理"), this, [=](){
            auto winName = "标签管理";
            auto fm = FunctionWnidowManager::getInstance();
            if (!fm->show(winName, this->isMaximized(), true)){
                fm->create(winName, new TagsManagerWindow(this));
            }
        });

        // ----------------索引----------------
        menu_version->addAction(tr("索引管理"), this, [=](){
            auto winName = "索引管理";
            auto fm = FunctionWnidowManager::getInstance();
            if (!fm->show(winName, this->isMaximized(), true)){
                fm->create(winName, new IndexWindow(this));
            }
        });

        // ----------------属性编辑----------------
        menu_version->addAction(tr("属性编辑"));

        // ----------------版本管理----------------
        menu_version->addAction(tr("版本管理"));
        m_top_menubar->addMenu(menu_version);
    }

    // ----------------视图----------------
    {
        auto menu_view = new QMenu(tr("视图(&V)"), this);

        // ----------------保存布局----------------
        menu_view->addAction(tr("保存布局"), this, [=]() {
            saveLayout();
        });
        // ----------------加载布局----------------
        menu_view->addAction(tr("加载布局"), this, [=]() {
            restoreLayout();
        });

        m_top_menubar->addMenu(menu_view);
    }

    this->setMenuBar(m_top_menubar);

}

void HomeWindow::saveLayout()
{
    LayoutManager::getInstance()->save(this, "HomeWindow");
}

void HomeWindow::restoreLayout()
{
    LayoutManager::getInstance()->restore(this, "HomeWindow");
}
