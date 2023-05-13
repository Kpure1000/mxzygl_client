#include "indexwindow.h"

#include <QDockWidget>
#include <QTabWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpacerItem>

#include "gui/uicomponent/categoryselector.h"
#include "gui/uicomponent/infotablewidget.h"
#include "gui/uicomponent/previewwidget.h"
#include "gui/uicomponent/loggingwidget.h"
#include "function/layoutmanager.h"

IndexWindow::IndexWindow(QWidget *parent)
    : IFunctionWindow(tr("索引编辑"), {800, 600}, true, false, true, parent)
{
    auto center_widget = centralWidget();
    {
        auto cate_sel = new CategorySelector(center_widget);
        auto docker_cate = new QDockWidget(tr("索引编辑 - 大类选择器"), center_widget);
        docker_cate->setObjectName("docker_cate");
        docker_cate->setWidget(cate_sel);
        docker_cate->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
        docker_cate->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
        addDockWidget(Qt::TopDockWidgetArea, docker_cate);
    }

    {
        auto docker_index = new QDockWidget(tr("索引编辑"), center_widget);
        docker_index->setWidget(initIndexWidget());
        docker_index->setObjectName("docker_index");
        docker_index->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
        docker_index->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
        addDockWidget(Qt::LeftDockWidgetArea, docker_index);
    }

    auto logging_widget = new LoggingWidget(center_widget);
    auto docker_logging = new QDockWidget(tr("索引编辑 - 信息输出"), center_widget);
    docker_logging->setObjectName("docker_logging");
    docker_logging->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    docker_logging->setWidget(logging_widget);
    docker_logging->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::RightDockWidgetArea, docker_logging);

    initModelWidget();

}

QWidget *IndexWindow::initIndexWidget()
{

    auto center_widget = centralWidget();

    auto totalWidget = new QWidget(center_widget);

    auto ly_total = new QGridLayout(totalWidget);
    totalWidget->setLayout(ly_total);

    auto bt_pull = new QPushButton(tr("↓↓↓拉取索引↓↓↓"), totalWidget);

    auto bt_push = new QPushButton(tr("↑↑↑推送索引↑↑↑"), center_widget);

    auto ly_bt_push_pull = new QHBoxLayout(totalWidget);
    ly_bt_push_pull->addWidget(bt_pull);
    ly_bt_push_pull->addWidget(bt_push);

    auto bt_sync = new QPushButton(tr("→\n→\n→\n加\n载\n索\n引\n内\n容\n→\n→\n→\n"), totalWidget);
    {
        bt_sync->setMaximumWidth(bt_sync->fontMetrics().averageCharWidth() * 4);
        auto plc_bt_sync = bt_sync->sizePolicy();
        plc_bt_sync.setVerticalPolicy(QSizePolicy::Expanding);
        bt_sync->setSizePolicy(plc_bt_sync);
    }

    auto index_content = new InfoTableWidget(nullptr, 1, true, totalWidget);
    auto index_type = new InfoTableWidget(nullptr, 1, true, totalWidget);
    auto index_tags = new InfoTableWidget(nullptr, 1, true, totalWidget);

    auto tab_w = new QTabWidget(totalWidget);
    tab_w->addTab(index_content, tr("模型内容索引"));
    tab_w->addTab(index_type, tr("模型类型索引"));
    tab_w->addTab(index_tags, tr("模型标签索引"));

    ly_total->addLayout(ly_bt_push_pull, 0, 0);
    ly_total->addWidget(bt_sync,         1, 1);
    ly_total->addWidget(tab_w,           1, 0);

    return totalWidget;
}

void IndexWindow::initModelWidget()
{
    auto center_widget = centralWidget();

    auto ly_total = new QVBoxLayout(center_widget);

    auto pre_w = new PreviewWidget(nullptr,
                                   3,
                                   2,
                                   PreviewWidget::PreviewType::MODEL,
                                   Qt::Horizontal,
                                   true,
                                   center_widget);
    ly_total->addWidget(pre_w);

}

void IndexWindow::closeEvent(QCloseEvent *event)
{
    LayoutManager::getInstance()->save(this, "IndexWindow");
    QWidget::closeEvent(event);
}

void IndexWindow::paintEvent(QPaintEvent *event)
{
    if (_is_first_paint)
        LayoutManager::getInstance()->restore(this, "IndexWindow");
    _is_first_paint = false;
}
