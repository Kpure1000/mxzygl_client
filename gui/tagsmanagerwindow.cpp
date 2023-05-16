#include "tagsmanagerwindow.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QDockWidget>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QSpacerItem>

#include "gui/uicomponent/infotablewidget.h"
#include "gui/uicomponent/loggingwidget.h"
#include "gui/uicomponent/categoryselector.h"
#include "function/tagsmanager.h"

TagsManagerWindow::TagsManagerWindow(QWidget *parent)
    : IFunctionWindow("", parent ? parent->size() / 2 : QSize{1000, 900}, true, false, false, parent)
{
    this->setWindowTitle("标签管理");
    this->setMinimumSize({1000, 900});

    auto center_widget = this->centralWidget();
    auto gridLayout = new QGridLayout(center_widget);
    auto tabWidget = new QTabWidget(center_widget);
    gridLayout->addWidget(tabWidget);

    m_modelManager  = new TagsManager(TagsManager::AssetType::MODEL,  this);
    m_bvhManager    = new TagsManager(TagsManager::AssetType::BVH,    this);
    m_effectManager = new TagsManager(TagsManager::AssetType::EFFECT, this);

    tabWidget->addTab(initManagerWidget(m_modelManager), tr("模型标签管理"));
    tabWidget->addTab(initManagerWidget(m_bvhManager), tr("骨骼动画标签管理"));
    tabWidget->addTab(initManagerWidget(m_effectManager), tr("特效资源标签管理"));

    initCategoryWidget();
    initLoggingWidget();

    connect(m_modelManager, &TagsManager::onResponsing, this, [=](const QString & info, bool is_continue){
        is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
    });
    connect(m_bvhManager, &TagsManager::onResponsing, this, [=](const QString & info, bool is_continue){
        is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
    });
    connect(m_effectManager, &TagsManager::onResponsing, this, [=](const QString & info, bool is_continue){
        is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
    });

}

TagsManagerWindow::~TagsManagerWindow()
{
}

QWidget *TagsManagerWindow::initManagerWidget(TagsManager *manager)
{
    auto tab = new QWidget();

    auto ly_tab_info = new QHBoxLayout(tab);

    // info
    auto ly_info = new QVBoxLayout();
    auto ly_pullpush = new QHBoxLayout();

    auto bt_pull = new QPushButton(tr("↓↓↓拉取标签↓↓↓"), tab);
    ly_pullpush->addWidget(bt_pull);

    auto bt_push = new QPushButton(tr("↑↑↑推送标签↑↑↑"), tab);
    ly_pullpush->addWidget(bt_push);
    bt_push->setEnabled(false);

    ly_info->addLayout(ly_pullpush);

    auto infotable = new InfoTableWidget(manager->getTags(), 1, false, tab);

    ly_info->addWidget(infotable);
    ly_tab_info->addLayout(ly_info);

    // editor
    auto ly_editor = new QVBoxLayout();
    auto gb_add = new QGroupBox(tr("添加标签"), tab);
    auto ly_gb_add = new QHBoxLayout(gb_add);

    auto le_input = new QLineEdit(gb_add);
    le_input->setPlaceholderText("请输入标签名称, 不能为空");
    le_input->setEnabled(false);

    auto bt_add = new QPushButton(tr("添加"), gb_add);
    bt_add->setEnabled(false);

    ly_gb_add->addWidget(bt_add);
    ly_gb_add->addWidget(le_input);

    ly_editor->addWidget(gb_add);

    auto gb_remove = new QGroupBox(tr("删除标签"), tab);
    auto ly_gb_remove = new QHBoxLayout(gb_remove);

    auto bt_remove = new QPushButton(tr("删除"), gb_remove);
    bt_remove->setEnabled(false);

    auto lb_remove_hint = new QLabel(tr("要删除的标签: "), gb_remove);
    auto lb_toRemove = new QLabel("", gb_remove);

    ly_gb_remove->addWidget(bt_remove, 0);
    ly_gb_remove->addWidget(lb_remove_hint, 0);
    ly_gb_remove->addWidget(lb_toRemove, 0);
    ly_gb_remove->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding));

    ly_editor->addWidget(gb_remove);

    ly_tab_info->addLayout(ly_editor);

    // 标签信息表功能
    connect(infotable, &InfoTableWidget::itemDoubleClicked, this, [=](QTableWidgetItem *item) {
        auto row = item->row();
        if (row < 0) {
            qDebug() << "TypeManagerWindow>>InfoTableWidget::onGroupSelected>> item < 0";
            return;
        }
        switch (manager->getType()) {
        case TagsManager::AssetType::MODEL:   m_selected_row_model  = row; break;
        case TagsManager::AssetType::BVH:     m_selected_row_bvh    = row; break;
        case TagsManager::AssetType::EFFECT:  m_selected_row_effect = row; break;
        }
        lb_toRemove->setText(manager->tags(row));
    });

    // 拉取功能
    connect(bt_pull, &QPushButton::clicked, this, [=](){
        // 注意, 由于pull会覆盖掉现有表格, 原本选择的标签是无意义的, 故此清除选择状态
        lb_toRemove->setText("");
        switch (manager->getType()) {
        case TagsManager::AssetType::MODEL:   m_selected_row_model = -1;  break;
        case TagsManager::AssetType::BVH:     m_selected_row_bvh = -1;    break;
        case TagsManager::AssetType::EFFECT:  m_selected_row_effect = -1; break;
        }
        // 拉取
        manager->pull();
    });
    connect(manager, &TagsManager::onPullSuccessful, this, [=]() {
        infotable->refresh();
        bt_push->setEnabled(true);
        bt_add->setEnabled(true);
        bt_remove->setEnabled(true);
        le_input->setEnabled(true);
    });

    // 推送功能
    connect(bt_push, &QPushButton::clicked, this, [=]() {
        manager->push();
    });
    connect(manager, &TagsManager::onPushSuccessful, this, [=]() {
        bt_push->setEnabled(false);
    });

    // 添加标签功能
    connect(bt_add, &QPushButton::clicked, this, [=]() {
        auto typeName = le_input->text();
        if (typeName == "") {
            auto info = tr("标签名不能为空, 添加失败!");
            m_logging_widget->error(info);
            QMessageBox::warning(this, tr("标签添加失败"), info);
            return;
        }
        manager->add(typeName);
    });
    connect(manager, &TagsManager::onAddRepeat, this, [=](const QString &typeName) {
        auto info = tr("标签名\" ") + typeName + tr(" \"重复, 添加失败!");
        m_logging_widget->error(info);
        QMessageBox::warning(this, tr("标签添加失败"), info);
    });
    connect(manager, &TagsManager::onAddSuccessed, this, [=](){
        infotable->refresh();
        le_input->clear();
        m_logging_widget->info(tr("标签添加成功!"));
    });

    // 删除标签功能
    connect(bt_remove, &QPushButton::clicked, this, [=]() {
        int selected_row = -1;
        switch (manager->getType()) {
        case TagsManager::AssetType::MODEL:   selected_row = m_selected_row_model;  break;
        case TagsManager::AssetType::BVH:     selected_row = m_selected_row_bvh;    break;
        case TagsManager::AssetType::EFFECT:  selected_row = m_selected_row_effect; break;
        }
        if (selected_row == -1) {
            auto info = tr("未选择要删除的标签, 删除失败! 请双击标签进行选择!");
            m_logging_widget->error(info);
            QMessageBox::warning(this, tr("标签删除失败"), info);
            return;
        }
        manager->remove(selected_row);
    });
    connect(manager, &TagsManager::onRemoveSuccessed, this, [=]() {
        infotable->refresh();
        lb_toRemove->setText("");
        switch (manager->getType()) {
        case TagsManager::AssetType::MODEL:   m_selected_row_model = -1;  break;
        case TagsManager::AssetType::BVH:     m_selected_row_bvh = -1;    break;
        case TagsManager::AssetType::EFFECT:  m_selected_row_effect = -1; break;
        }
        m_logging_widget->info(tr("标签删除成功!"));
    });

    return tab;
}

void TagsManagerWindow::initCategoryWidget()
{
    auto center_widget = this->centralWidget();
    auto cate_sel = new CategorySelector(center_widget);
    auto docker_cate = new QDockWidget(tr("标签管理 - 大类选择器"), center_widget);
    docker_cate->setObjectName("docker_cate");
    docker_cate->setWidget(cate_sel);
    docker_cate->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    docker_cate->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::TopDockWidgetArea, docker_cate);

}

void TagsManagerWindow::initLoggingWidget()
{
    auto center_widget = this->centralWidget();
    m_logging_widget = new LoggingWidget(center_widget);
    auto docker_logging = new QDockWidget(tr("标签管理 - 信息输出"), center_widget);
    docker_logging->setObjectName("docker_logging");
    docker_logging->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    docker_logging->setWidget(m_logging_widget);
    docker_logging->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::RightDockWidgetArea, docker_logging);
}
