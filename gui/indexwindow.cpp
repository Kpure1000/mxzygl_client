#include "indexwindow.h"

#include <QDockWidget>
#include <QTabWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QShowEvent>
#include <QCloseEvent>
#include <QMenuBar>
#include <QMenu>

#include "gui/uicomponent/categoryselector.h"
#include "gui/uicomponent/infotablewidget.h"
#include "gui/uicomponent/previewwidget.h"
#include "gui/uicomponent/loggingwidget.h"
#include "function/layoutmanager.h"
#include "function/indexeditor.h"

IndexWindow::IndexWindow(QWidget *parent)
    : IFunctionWindow(tr("索引编辑"), {800, 600}, true, false, true, parent)
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

    auto index_editor_cont = new IndexEditor(IndexEditor::IndexType::CONTENT, this);
    auto index_editor_tags = new IndexEditor(IndexEditor::IndexType::TAGS,    this);
    auto index_editor_type = new IndexEditor(IndexEditor::IndexType::TYPE,    this);

    auto center_widget = centralWidget();

    auto ly_total = new QGridLayout(center_widget);

    auto tab_w = new QTabWidget(center_widget);
    auto model_cont = initModelWidget(index_editor_cont);
    auto model_tags = initModelWidget(index_editor_tags);
    auto model_type = initModelWidget(index_editor_type);
    tab_w->addTab(model_cont, tr("模型内容数据"));
    tab_w->addTab(model_tags, tr("模型标签数据"));
    tab_w->addTab(model_type, tr("模型类型数据"));

    ly_total->addWidget(tab_w);

    auto indexWidget = new QWidget(center_widget);

    auto ly_index = new QHBoxLayout(indexWidget);

    auto infotable = new InfoTableWidget(index_editor_cont->getIndexInfo(), 1, true, true, center_widget);

    auto bt_sync = new QPushButton(tr("→\n→\n→\n同\n步\n修\n改\n到\n原\n始\n数\n据\n→\n→\n→\n"),center_widget);
    {
        bt_sync->setMaximumWidth(bt_sync->fontMetrics().averageCharWidth() * 4);
        auto plc_bt_sync = bt_sync->sizePolicy();
        plc_bt_sync.setVerticalPolicy(QSizePolicy::Expanding);
        bt_sync->setSizePolicy(plc_bt_sync);
    }
    ly_index->addWidget(infotable, 1);
    ly_index->addWidget(bt_sync,   0);

    connect(bt_sync, &QPushButton::clicked, this, [=]() {
        if (model_cont == tab_w->currentWidget()) {
            index_editor_cont->syncToOrg();
        } else if (model_tags == tab_w->currentWidget()) {
            index_editor_tags->syncToOrg();
        } else if (model_type == tab_w->currentWidget()) {
            index_editor_type->syncToOrg();
        }
    });

    connect(tab_w, &QTabWidget::currentChanged, this, [=](int index) {
        if (model_cont == tab_w->widget(index)) {
            infotable->resetInfo(index_editor_cont->getIndexInfo());
        } else if (model_tags == tab_w->widget(index)) {
            infotable->resetInfo(index_editor_tags->getIndexInfo());
        } else if (model_type == tab_w->widget(index)) {
            infotable->resetInfo(index_editor_type->getIndexInfo());
        }
    });

    {
        auto docker_index = new QDockWidget(tr("索引编辑"), center_widget);
        docker_index->setWidget(indexWidget);
        docker_index->setObjectName("docker_index");
        docker_index->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
        docker_index->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
        addDockWidget(Qt::LeftDockWidgetArea, docker_index);
    }

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
        auto logging_widget = new LoggingWidget(center_widget);
        auto docker_logging = new QDockWidget(tr("索引编辑 - 信息输出"), center_widget);
        docker_logging->setObjectName("docker_logging");
        docker_logging->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
        docker_logging->setWidget(logging_widget);
        docker_logging->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
        addDockWidget(Qt::RightDockWidgetArea, docker_logging);
    }

}

QWidget *IndexWindow::initModelWidget(IndexEditor *editor)
{
    auto modelWidget = new QWidget(centralWidget());

    auto ly_total = new QGridLayout(modelWidget);

    auto bt_pull = new QPushButton(tr("↓↓↓拉取数据↓↓↓"), modelWidget);

    auto bt_push = new QPushButton(tr("↑↑↑推送数据↑↑↑"), modelWidget);

    auto ly_pp = new QHBoxLayout();
    ly_pp->addWidget(bt_pull);
    ly_pp->addWidget(bt_push);

    auto bt_compress = new QPushButton(tr("←\n←\n←\n压\n缩\n为\n索\n引\n←\n←\n←\n"), modelWidget);
    {
        bt_compress->setMaximumWidth(bt_compress->fontMetrics().averageCharWidth() * 4);
        auto plc_bt_sync = bt_compress->sizePolicy();
        plc_bt_sync.setVerticalPolicy(QSizePolicy::Expanding);
        bt_compress->setSizePolicy(plc_bt_sync);
    }

    auto preview_w = new PreviewWidget(editor->getOriginInfo(), 1, 3, PreviewWidget::PreviewType::MODEL, Qt::Vertical, true, true, modelWidget);

    ly_total->addLayout(ly_pp,       0, 1);
    ly_total->addWidget(bt_compress, 1, 0);
    ly_total->addWidget(preview_w,   1, 1);

    connect(bt_pull, &QPushButton::clicked, this, [=]() {
        editor->pull_org();
    });
    connect(bt_push, &QPushButton::clicked, this, [=]() {
        editor->push_org();
    });
    connect(bt_compress, &QPushButton::clicked, this, [=]() {
        editor->compressToIndex();
    });
    connect(preview_w, &PreviewWidget::onPreview, this, [=](const std::vector<int> &index) {
        //
    });

    return modelWidget;
}

void IndexWindow::closeEvent(QCloseEvent *event)
{
    LayoutManager::getInstance()->save(this, "IndexWindow");
    QWidget::closeEvent(event);
}

void IndexWindow::showEvent(QShowEvent *event)
{
    if (_is_first_paint) {
        _is_first_paint = false;
        LayoutManager::getInstance()->restore(this, "IndexWindow");
    }
    update();
}
