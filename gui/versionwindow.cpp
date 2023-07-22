#include "versionwindow.h"

#include <QGridLayout>
#include <QDockWidget>
#include <QMenuBar>
#include <QMenu>
#include <QShowEvent>
#include <QCloseEvent>
#include <QJsonObject>

#include "uicomponent/versionwidget.h"
#include "gui/uicomponent/loggingwidget.h"
#include "function/layoutmanager.h"
#include "function/versioncontroller.h"

VersionWindow::VersionWindow(QWidget *parent)
    : IFunctionWindow{tr("版本管理"), {800, 600}, true, false, true, parent}
{

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

    auto center_widget = centralWidget();

    auto ly_total = new QGridLayout(center_widget);

    auto vctrl = new VersionController(this);

    auto vwidget = new VersionWidget(vctrl, this);

    ly_total->addWidget(vwidget);

    {
        m_logging_widget = new LoggingWidget(center_widget);
        auto docker_logging = new QDockWidget(tr("版本管理 - 信息输出"), center_widget);
        docker_logging->setObjectName("docker_logging");
        docker_logging->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
        docker_logging->setWidget(m_logging_widget);
        docker_logging->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
        addDockWidget(Qt::RightDockWidgetArea, docker_logging);
    }

    connect(vctrl, &VersionController::onResponsing, this, [=](const QString & info, bool is_continue){
        is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
    });

    connect(vctrl, &VersionController::onPullReopSuccessful, this, [=]() {
        m_logging_widget->trace("版本库拉取成功");
    });
    connect(vctrl, &VersionController::onCreateReopSuccessful, this, [=](const QJsonObject &info) {
        m_logging_widget->trace(QString("版本库'%1'创建成功").arg(info["name"].toString()));
    });
    connect(vctrl, &VersionController::onAddVersionSuccessful, this, [=]() {
        m_logging_widget->trace("版本增加成功");
    });
    connect(vctrl, &VersionController::onQueryVersionSuccessful, this, [=](const QJsonObject &info) {
        m_logging_widget->trace("版本查询成功");
    });
    connect(vctrl, &VersionController::onSyncVersionSuccessful, this, [=]() {
        m_logging_widget->trace("版本同步成功");
    });
    connect(vctrl, &VersionController::onDeleteVersionSuccessful, this, [=]() {
        m_logging_widget->trace("版本删除成功");
    });
    connect(vctrl, &VersionController::onRollbackVersionSuccessful, this, [=]() {
        m_logging_widget->trace("版本回退成功");
    });

}

void VersionWindow::closeEvent(QCloseEvent *event)
{
    LayoutManager::getInstance()->save(this, "IndexWindow");
    QWidget::closeEvent(event);
}

void VersionWindow::showEvent(QShowEvent *event)
{
    if (_is_first_paint) {
        _is_first_paint = false;
        LayoutManager::getInstance()->restore(this, "IndexWindow");
    }
    update();
}
