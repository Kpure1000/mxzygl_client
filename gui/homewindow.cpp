#include "homewindow.h"

#include <QGridLayout>
#include <QApplication>
#include <QDebug>
#include <QDialog>
#include <QKeyEvent>
#include <QHBoxLayout>

#include "optionwindow.h"
#include "importwindow.h"
#include "gui/uicomponent/modelsearchwidget.h"

HomeWindow::HomeWindow(QWidget* parent, QApplication* current_app) : QWidget(parent)
{
    setWindowIcon(QIcon(":/assets/assets/icon/icon.jpg"));

    makeMenu();

    m_tabw = new QTabWidget(this);
    auto ly_total = new QGridLayout(this);
    ly_total->addWidget(m_tabw);

    m_tabw->addTab(new ModelSearchWidget(ModelSearch::SearchType::CONTENT,this), "模型内容检索");
    m_tabw->addTab(new ModelSearchWidget(ModelSearch::SearchType::TAG,this), "模型标签检索");
    m_tabw->addTab(new ModelSearchWidget(ModelSearch::SearchType::TYPE,this), "模型类型检索");

    this->resize(800, 600);
    this->setLayout(ly_total);

}

HomeWindow::~HomeWindow()
{
    qDebug() << "HomeWindow Closed";
}

void HomeWindow::keyPressEvent(QKeyEvent *ev)
{
    // TODO: 方便调试
    if(ev->key() == Qt::Key::Key_Escape) {
        QApplication::exit(0);
    }
}

void HomeWindow::makeMenu()
{
    m_top_menubar = new QMenuBar(this);

    // ----------------文件----------------
    {
        auto menu_file = new QMenu(tr("文件(&F)"), this);
        m_top_menubar->addMenu(menu_file);

        // ----------------导入----------------
        auto submenu_import = menu_file->addMenu(tr("导入...(&I)"));
        // 模型导入
        submenu_import->addAction(tr("模型导入(.obj;.fbx)"), this, [this]() {
            auto winName = "模型导入";
            auto fm = FunctionWnidowManager::getInstance();
            if (!fm->show(winName)) {
                fm->create(winName, new ImportWindow(winName, AssetImporter::ImportType::MODEL, this));
            }
        });

        // 骨骼动画导入
        submenu_import->addAction(tr("骨骼动画导入(.bvh)"), this, [this]() {
            auto winName = "骨骼动画导入";
            auto fm = FunctionWnidowManager::getInstance();
            if (!fm->show(winName)) {
                fm->create(winName, new ImportWindow(winName, AssetImporter::ImportType::BVH, this));
            }
        });

        // 特效导入
        submenu_import->addAction(tr("特效导入(.bvh)"), this, [this]() {
            auto winName = "特效导入";
            auto fm = FunctionWnidowManager::getInstance();
            if (!fm->show(winName)) {
                fm->create(winName, new ImportWindow(winName, AssetImporter::ImportType::EFFECT, this));
            }
        });

        // ----------------选项----------------
        menu_file->addAction(tr("选项(&O)"), this, [this](){
                auto winName = "选项";
                auto fm = FunctionWnidowManager::getInstance();
                if (!fm->show(winName)) {
                    fm->create(winName, new OptionWindow(this));
                }
        }, QKeySequence("Ctrl+O"));

        menu_file->addSeparator();

        // ----------------退出----------------
        menu_file->addAction(tr("退出(&X)"), this, [](){
            QApplication::exit(0);
        }, QKeySequence("Ctrl+w"));

    }

    // ----------------工具----------------
    {
        auto menu_version = new QMenu(tr("工具(&V)"), this);

        // ----------------属性编辑----------------
        menu_version->addAction(tr("属性编辑"));

        // ----------------版本管理----------------
        menu_version->addAction(tr("版本管理"));
        m_top_menubar->addMenu(menu_version);
    }

}
