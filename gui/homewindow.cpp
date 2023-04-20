#include "homewindow.h"

#include <QGridLayout>
#include <QApplication>
#include <QDebug>
#include <QDialog>
#include <QKeyEvent>
#include <QHBoxLayout>

#include "optionwindow.h"

#include "mxzygl.h"

OptionWindow *option_win;
ImportWindow *import_win;

HomeWindow::HomeWindow(QWidget* parent, QApplication* current_app) : QWidget(parent)
{
    setWindowIcon(QIcon(":/assets/assets/icon/icon.jpg"));

    m_top_menubar = new QMenuBar(this);

    // ----------------文件----------------
    {
        auto menu_file = new QMenu(tr("文件(&F)"), this);

        // ----------------导入----------------
        menu_file->addAction(tr("模型导入(&I)"), this, [this](){
            if (nullptr == import_win || import_win->isHidden()) {
                delete import_win;
                import_win = new ImportWindow(this);
            }
            import_win->showOnTop();
            import_win->show();
        }, QKeySequence("Ctrl+I"));

        // ----------------选项----------------
        menu_file->addAction(tr("选项(&O)"), this, [this](){
            if (nullptr == option_win || option_win->isHidden()) {
                delete option_win;
                option_win = new OptionWindow(this);
            }
            option_win->showOnTop();
            option_win->show();
        }, QKeySequence("Ctrl+O"));

        menu_file->addSeparator();

        // ----------------退出----------------
        menu_file->addAction(tr("退出(&X)"), this, [](){
            QApplication::exit(0);
        }, QKeySequence("Ctrl+w"));

        m_top_menubar->addMenu(menu_file);
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

    m_tabw = new QTabWidget(this);

    this->resize(1000, 800);

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
