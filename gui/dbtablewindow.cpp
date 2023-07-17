#include "dbtablewindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QMessageBox>
#include <QCloseEvent>
#include <QShowEvent>
#include <QPushButton>
#include <QTabWidget>

#include "gui/uicomponent/loggingwidget.h"
#include "gui/uicomponent/infotablewidget.h"
#include "function/layoutmanager.h"
#include "function/dbtableviewer.h"

DBTableWindow::DBTableWindow(QWidget *parent)
    : IFunctionWindow("", {800, 600}, true, false, true, parent)
{
    auto center_widget = centralWidget();

    m_logging_widget = new LoggingWidget(center_widget);
    auto docker_logging = new QDockWidget(tr("模型数据表预览 - 信息输出"), center_widget);
    docker_logging->setObjectName("docker_logging");
    docker_logging->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    docker_logging->setWidget(m_logging_widget);
    docker_logging->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::RightDockWidgetArea, docker_logging);

    auto ly_total = new QVBoxLayout(center_widget);

    auto tabw = new QTabWidget(center_widget);
    ly_total->addWidget(tabw);

    auto tb_model_viewer = new DBTableViewer(DBTableViewer::TableType::MODEL, this);
    auto tb_bvh_viewer = new DBTableViewer(DBTableViewer::TableType::BVH, this);
    auto tb_effect_viewer = new DBTableViewer(DBTableViewer::TableType::EFFECT, this);

    auto modelTableW = setupTableWidget(tb_model_viewer);
    auto bvhTableW = setupTableWidget(tb_bvh_viewer);
    auto effectTableW = setupTableWidget(tb_effect_viewer);

    tabw->addTab(modelTableW, tr("模型资源数据表"));
    tabw->addTab(bvhTableW, tr("骨骼动画资源数据表"));
    tabw->addTab(effectTableW, tr("特效资源数据表"));
}

void DBTableWindow::closeEvent(QCloseEvent *event)
{
    LayoutManager::getInstance()->save(this, "DBTableWindow");
    QWidget::closeEvent(event);
}

void DBTableWindow::showEvent(QShowEvent *event)
{
    if (_is_first_paint) {
        _is_first_paint = false;
        LayoutManager::getInstance()->restore(this, "DBTableWindow");
    }
    update();
}

QWidget *DBTableWindow::setupTableWidget(DBTableViewer * viewer)
{
    auto center_widget = centralWidget();

    auto totalW = new QWidget(center_widget);
    auto ly_total = new QVBoxLayout(totalW);

    auto bt_pullTB = new QPushButton(tr("拉取数据表信息"), this);
    ly_total->addWidget(bt_pullTB, 0);

    auto infotable = new InfoTableWidget(viewer->getTableInfo(), 1, false, false, this);
    ly_total->addWidget(infotable, 1);

    connect(bt_pullTB, &QPushButton::clicked, this, [=](){
        infotable->clearInfos();
        bt_pullTB->setEnabled(false);
        viewer->pullTBInfo();
    });

    connect(viewer, &DBTableViewer::onTBPullSuccessful, this, [=](){
        infotable->refresh();
        infotable->selectGroup(0);
        bt_pullTB->setEnabled(true);
        m_logging_widget->trace("拉取数据表信息成功");
    });

    connect(viewer, &DBTableViewer::onResponsing, this, [=](const QString & info, bool is_continue){
        is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
    });

    return totalW;
}
