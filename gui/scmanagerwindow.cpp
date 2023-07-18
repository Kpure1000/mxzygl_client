#include "scmanagerwindow.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QDockWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>
#include <QSpacerItem>
#include <QKeyEvent>
#include <QShowEvent>
#include <QCloseEvent>
#include <QDebug>

#include "gui/uicomponent/categoryselector.h"
#include "gui/uicomponent/infotablewidget.h"
#include "gui/uicomponent/loggingwidget.h"
#include "function/layoutmanager.h"
#include "function/scmanager.h"

SCManagerWindow::SCManagerWindow(QWidget *parent)
    : IFunctionWindow{tr("三维模型标准分类管理"), {800, 600}, true, false, true, parent}
{

    // ----------------视图----------------
    {
        auto top_menubar = new QMenuBar(this);
        auto menu_view = new QMenu(tr("视图(&V)"), this);

        // ----------------保存布局----------------
        menu_view->addAction(tr("保存布局"), this, [=]() {
            LayoutManager::getInstance()->save(this, "SCManagerWindow");
        });
        // ----------------加载布局----------------
        menu_view->addAction(tr("加载布局"), this, [=]() {
            LayoutManager::getInstance()->restore(this, "SCManagerWindow");
        });

        top_menubar->addMenu(menu_view);
        this->setMenuBar(top_menubar);
    }

    auto center_widget = this->centralWidget();
    auto gridLayout = new QGridLayout(center_widget);


    m_scManager = new SCManager(this);

    auto manager_w = initManagerWidget(m_scManager);

    gridLayout->addWidget(manager_w);

    initCategoryWidget();
    initLoggingWidget();

    connect(m_scManager, &SCManager::onResponsing, this, [=](const QString & info, bool is_continue){
        is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
    });

}

void SCManagerWindow::closeEvent(QCloseEvent *event)
{
    LayoutManager::getInstance()->save(this, "SCManagerWindow");
    QWidget::closeEvent(event);
}

void SCManagerWindow::showEvent(QShowEvent *event)
{
    if (_is_first_paint) {
        _is_first_paint = false;
        LayoutManager::getInstance()->restore(this, "SCManagerWindow");
    }
    update();
}

QWidget *SCManagerWindow::initManagerWidget(SCManager *manager)
{
    auto total_widget = new QWidget(centralWidget());

    auto ly_total = new QHBoxLayout(total_widget);

    // info UI
    auto ly_info = new QVBoxLayout();
    auto ly_pull = new QHBoxLayout();

    auto bt_pull = new QPushButton(tr("↓↓↓获取标准分类↓↓↓"), total_widget);
    ly_pull->addWidget(bt_pull);

    ly_info->addLayout(ly_pull);

    auto infotable = new InfoTableWidget(manager->getSCInfo(), 1, false, true, total_widget);

    ly_info->addWidget(infotable);
    ly_total->addLayout(ly_info);

    // editor UI
    auto ly_editor = new QVBoxLayout();

    // editor-add
    auto gb_add = new QGroupBox(tr("添加标准分类"), total_widget);
    gb_add->setEnabled(false);
    auto ly_gb_add = new QHBoxLayout(gb_add);

    auto le_add_input = new QLineEdit(gb_add);
    le_add_input->setPlaceholderText("请输入标准分类名称, 不能为空");

    auto bt_add = new QPushButton(tr("添加"), gb_add);

    ly_gb_add->addWidget(bt_add);
    ly_gb_add->addWidget(le_add_input);

    ly_editor->addWidget(gb_add);

    // editor-remove
    auto gb_remove = new QGroupBox(tr("删除标准分类"), total_widget);
    gb_remove->setEnabled(false);
    auto ly_gb_remove = new QHBoxLayout(gb_remove);

    auto bt_remove = new QPushButton(tr("删除"), gb_remove);

    auto lb_remove_hint = new QLabel(tr("要删除的标准分类: "), gb_remove);
    auto lb_toRemove = new QLabel("\"\"", gb_remove);

    ly_gb_remove->addWidget(bt_remove, 0);
    ly_gb_remove->addWidget(lb_remove_hint, 0);
    ly_gb_remove->addWidget(lb_toRemove, 0);
    ly_gb_remove->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding));

    ly_editor->addWidget(gb_remove);

    // editor-modify
    auto gb_modify = new QGroupBox(tr("更改标准分类"), total_widget);
    gb_modify->setEnabled(false);
    auto ly_gb_modify = new QVBoxLayout(gb_modify);

    auto bt_modify = new QPushButton(tr("更改"), gb_modify);

    auto lb_modify_hint = new QLabel(tr("→更改为→"), gb_modify);
    auto lb_toModify = new QLabel("\"\"", gb_modify);

    auto le_modify_input = new QLineEdit(gb_modify);
    le_modify_input->setPlaceholderText("请输入标准分类名称, 不能为空");

    auto ly_modify_hint = new QHBoxLayout();

    ly_modify_hint->addWidget(lb_toModify, 0);
    ly_modify_hint->addWidget(lb_modify_hint, 0);
    ly_modify_hint->addWidget(le_modify_input, 0);
    ly_modify_hint->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding));

    ly_gb_modify->addLayout(ly_modify_hint, 0);

    ly_gb_modify->addWidget(bt_modify, 0);

    ly_editor->addWidget(gb_modify);

    ly_total->addLayout(ly_editor);

    // 信息表选中功能
    connect(infotable, &InfoTableWidget::itemDoubleClicked, this, [=](QTableWidgetItem *item) {
        auto row = item->row();
        if (row < 0) {
            qDebug() << "SCManagerWindow>>InfoTableWidget::onGroupSelected>> item < 0";
            return;
        }
        m_selected_row = row;
        lb_toRemove->setText(QString().asprintf("\"%s\"", manager->sc(row).toStdString().c_str()));
        lb_toModify->setText(QString().asprintf("\"%s\"", manager->sc(row).toStdString().c_str()));
    });

    // 拉取功能
    connect(bt_pull, &QPushButton::clicked, this, [=](){
        // 注意, 由于pull会覆盖掉现有表格, 原本选择的是无意义的, 故此清除选择状态
        lb_toRemove->setText("\"\"");
        lb_toModify->setText("\"\"");
        m_selected_row  = -1;
        // 拉取
        manager->pull();
    });
    connect(manager, &SCManager::onPullSuccessful, this, [=]() {
        infotable->refresh();
        gb_add->setEnabled(true);
        gb_remove->setEnabled(true);
        gb_modify->setEnabled(true);
    });

    // 添加功能
    connect(bt_add, &QPushButton::clicked, this, [=]() {
        auto scName = le_add_input->text();
        if (scName == "") {
            auto info = tr("标准分类名<b>不能为空</b>, 添加失败!");
            m_logging_widget->error(info);
            QMessageBox::warning(this, tr("标准分类名为空, 添加失败"), info);
            return;
        }
        manager->add(scName);
    });
    connect(manager, &SCManager::onAddRepeat, this, [=](const QString &scName) {
        auto info = tr("标准分类名\" ") + scName + tr(" \"<b>重复</b>, 添加失败!");
        m_logging_widget->error(info);
        QMessageBox::warning(this, tr("标准分类名重复, 添加失败"), info);
    });
    connect(manager, &SCManager::onAddSuccessed, this, [=](){
        infotable->refresh();
        le_add_input->clear();
        m_logging_widget->info(tr("标准分类添加成功，上传数据中..."));
        QMessageBox::warning(this, tr("标准分类添加"), tr("标准分类添加成功"));
        manager->push();
    });

    // 删除功能
    connect(bt_remove, &QPushButton::clicked, this, [=]() {
        int selected_row = -1;
        selected_row = m_selected_row;
        if (selected_row == -1) {
            auto info = tr("未选择要删除的标准分类, 删除失败! 请<b>双击</b>标准分类进行选择!");
            m_logging_widget->error(info);
            QMessageBox::warning(this, tr("未选择要更改的标准分类, 删除失败"), info);
            return;
        }
        manager->remove(selected_row);
    });
    connect(manager, &SCManager::onRemoveSuccessed, this, [=]() {
        infotable->refresh();
        lb_toRemove->setText("\"\"");
        lb_toModify->setText("\"\"");
        m_selected_row = -1;
        m_logging_widget->info(tr("标准分类删除成功，上传数据中..."));
        QMessageBox::warning(this, tr("标准分类删除"), tr("标准分类删除成功"));
        manager->push();
    });

    // 更改功能
    connect(bt_modify, &QPushButton::clicked, this, [=]() {
        int selected_row = -1;
        selected_row = m_selected_row;
        if (selected_row == -1) {
            auto info = tr("未选择要更改的标准分类, 更改失败! 请<b>双击</b>标准分类进行选择!");
            m_logging_widget->error(info);
            QMessageBox::warning(this, tr("未选择要更改的标准分类, 更改失败"), info);
            return;
        }
        auto scName = le_modify_input->text();
        if (scName == "") {
            auto info = tr("标准分类名<b>不能为空</b>, 更改失败!");
            m_logging_widget->error(info);
            QMessageBox::warning(this, tr("标准分类名为空, 更改失败"), info);
            return;
        }
        manager->modify(selected_row, scName);
    });
    connect(manager, &SCManager::onModifyRepeat, this, [=](const QString &scName) {
        auto info = tr("无法更改为<b>同名标准分类</b>\" ") + scName + tr(" \", 更改失败!");
        m_logging_widget->error(info);
        QMessageBox::warning(this, tr("无法更改同名标准分类, 更改失败"), info);
    });
    connect(manager, &SCManager::onModifySuccessed, this, [=]() {
        infotable->refresh();
        le_modify_input->clear();
        lb_toRemove->setText("\"\"");
        lb_toModify->setText("\"\"");
        m_selected_row = -1;
        m_logging_widget->info(tr("标准分类更改成功，上传数据中..."));
        QMessageBox::warning(this, tr("标准分类更改"), tr("标准分类更改成功"));
        manager->push();
    });

    // 推送功能
    connect(manager, &SCManager::onPushSuccessful, this, [=]() {
        m_logging_widget->info(tr("数据上传成功"));
    });

    bt_pull->click();

    return total_widget;
}

void SCManagerWindow::initCategoryWidget()
{
    auto center_widget = this->centralWidget();
    auto cate_sel = new CategorySelector(center_widget);
    auto docker_cate = new QDockWidget(tr("标准分类管理 - 大类选择器"), center_widget);
    docker_cate->setObjectName("docker_cate");
    docker_cate->setWidget(cate_sel);
    docker_cate->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    docker_cate->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::TopDockWidgetArea, docker_cate);
}

void SCManagerWindow::initLoggingWidget()
{
    auto center_widget = this->centralWidget();
    m_logging_widget = new LoggingWidget(center_widget);
    auto docker_logging = new QDockWidget(tr("标准分类管理 - 信息输出"), center_widget);
    docker_logging->setObjectName("docker_logging");
    docker_logging->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    docker_logging->setWidget(m_logging_widget);
    docker_logging->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::RightDockWidgetArea, docker_logging);
}
