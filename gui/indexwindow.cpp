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
#include <QMessageBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>

#include "gui/uicomponent/categoryselector.h"
#include "gui/uicomponent/infotablewidget.h"
#include "gui/uicomponent/previewwidget.h"
#include "gui/uicomponent/loggingwidget.h"
#include "function/layoutmanager.h"

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

    m_index_infotable = new InfoTableWidget(index_editor_cont->getIndexInfo(), 1, false, true, center_widget);

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

    auto bt_sync = new QPushButton(tr("→\n→\n→\n同\n步\n修\n改\n到\n原\n始\n数\n据\n→\n→\n→\n"),center_widget);
    {
        bt_sync->setMaximumWidth(bt_sync->fontMetrics().averageCharWidth() * 4);
        auto plc_bt_sync = bt_sync->sizePolicy();
        plc_bt_sync.setVerticalPolicy(QSizePolicy::Expanding);
        bt_sync->setSizePolicy(plc_bt_sync);
    }
    ly_index->addWidget(m_index_infotable, 1);
    ly_index->addWidget(bt_sync,   0);

    // 选中索引功能
    connect(m_index_infotable, &InfoTableWidget::itemDoubleClicked, this, [=](QTableWidgetItem *item) {
        auto row = item->row();
        if (row < 0) {
            qDebug() << "IndexWindow>>InfoTableWidget::itemDoubleClicked>> row < 0";
            return;
        }
        m_selected_row = row;
        if (model_cont == tab_w->currentWidget()) {
            emit onIndexSelected(IndexEditor::IndexType::CONTENT);
        } else if (model_tags == tab_w->currentWidget()) {
            emit onIndexSelected(IndexEditor::IndexType::TAGS);
        } else if (model_type == tab_w->currentWidget()) {
            emit onIndexSelected(IndexEditor::IndexType::TYPE);
        }
    });

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
            m_index_infotable->resetInfo(index_editor_cont->getIndexInfo());
        } else if (model_tags == tab_w->widget(index)) {
            m_index_infotable->resetInfo(index_editor_tags->getIndexInfo());
        } else if (model_type == tab_w->widget(index)) {
            m_index_infotable->resetInfo(index_editor_type->getIndexInfo());
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
        m_logging_widget = new LoggingWidget(center_widget);
        auto docker_logging = new QDockWidget(tr("索引编辑 - 信息输出"), center_widget);
        docker_logging->setObjectName("docker_logging");
        docker_logging->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
        docker_logging->setWidget(m_logging_widget);
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

    // 不可编辑
    auto preview_w = new PreviewWidget(editor->getOriginInfo(), 1, 3, PreviewWidget::PreviewType::MODEL, Qt::Vertical, false, true, modelWidget);

    // editor-modify
    auto gb_modify = new QGroupBox(tr("索引编辑"), modelWidget);
    gb_modify->setEnabled(false);

    auto lb_modify_hint = new QLabel(tr("→更改为→"), gb_modify);
    auto lb_toModify = new QLabel("\"\"", gb_modify);

    auto le_modify_input = new QLineEdit(gb_modify);
    le_modify_input->setPlaceholderText("请输入索引名称, 不能为空");

    auto ly_modify_hint = new QHBoxLayout();

    ly_modify_hint->addWidget(lb_toModify, 0);
    ly_modify_hint->addWidget(lb_modify_hint, 0);
    ly_modify_hint->addWidget(le_modify_input, 0);
    ly_modify_hint->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding));

    auto bt_modify = new QPushButton(tr("更改索引名"), gb_modify);

    auto ly_gb_modify = new QVBoxLayout(gb_modify);
    ly_gb_modify->addSpacerItem(
        new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Expanding));
    ly_gb_modify->addLayout(ly_modify_hint, 0);
    ly_gb_modify->addWidget(bt_modify, 0);
    ly_gb_modify->addSpacerItem(
        new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Expanding));
    ly_gb_modify->setStretch(0, 1);
    ly_gb_modify->setStretch(3, 2);

    ly_total->addWidget(gb_modify,   1, 0);
    ly_total->addLayout(ly_pp,       0, 2);
    ly_total->addWidget(bt_compress, 1, 1);
    ly_total->addWidget(preview_w,   1, 2);
    ly_total->setColumnStretch(0, 0);
    ly_total->setColumnStretch(1, 0);
    ly_total->setColumnStretch(2, 1);

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
        preview_w->previewFiles(editor->getFilePaths(index), editor->getPreviewInfo(index), false);
    });

    connect(editor, &IndexEditor::onResponsing, this, [=](const QString & info, bool is_continue){
        is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
    });
    connect(editor, &IndexEditor::onPullSuccessful, this, [=]() {
        preview_w->refreshInfo();
        QMessageBox::information(this, tr("数据拉取"), tr("数据拉取成功!"));
        // 自动压缩
        bt_compress->click();
    });
    connect(editor, &IndexEditor::onPushSuccessful, this, [=]() {
        preview_w->refreshInfo();
        QMessageBox::information(this, tr("数据推送"), tr("数据推送成功!"));
        // 自动拉取
        bt_pull->click();
    });
    connect(editor, &IndexEditor::onCompressed, this, [=]() {
        m_logging_widget->info(tr("压缩为索引成功"));
        m_index_infotable->resetInfo(editor->getIndexInfo());
        QMessageBox::information(this, tr("压缩为索引"), tr("压缩为索引成功!"));
    });
    connect(editor, &IndexEditor::onSyncToOrg, this, [=]() {
        m_logging_widget->info(tr("索引同步成功"));
        QMessageBox::information(this, tr("索引同步"), tr("索引同步成功!"));
    });

    // 更改索引功能
    connect(this, &IndexWindow::onIndexSelected, this, [=](IndexEditor::IndexType type) {
        if (type == editor->getType()) {
            gb_modify->setEnabled(true);
            lb_toModify->setText(QString().asprintf("\"%s\"", editor->index(m_selected_row).toStdString().c_str()));
        }
    });

    connect(bt_modify, &QPushButton::clicked, this, [=]() {
        if (m_selected_row == -1) {
            auto info = tr("未选择要更改的标签, 更改失败! 请<b>点击目录根节点</b>进行选择!");
            m_logging_widget->error(info);
            QMessageBox::warning(this, tr("未选择要更改的标签, 更改失败"), info);
            return;
        }
        auto tagName = le_modify_input->text();
        if (tagName == "") {
            auto info = tr("标签目录名<b>不能为空</b>, 更改失败!");
            m_logging_widget->error(info);
            QMessageBox::warning(this, tr("标签目录名为空, 更改失败"), info);
            return;
        }
        editor->modify(m_selected_row, tagName);
    });
    connect(editor, &IndexEditor::onModifyRepeat, this, [=](const QString &scName) {
        auto info = tr("无法更改为<b>同名标签</b>\" ") + scName + tr(" \", 更改失败!");
                    m_logging_widget->error(info);
        QMessageBox::warning(this, tr("无法更改同名标签, 更改失败"), info);
    });
    connect(editor, &IndexEditor::onModifySuccessed, this, [=]() {
        preview_w->clearInfo();
        preview_w->refreshInfo();
        le_modify_input->clear();
        lb_toModify->setText("\"\"");
        m_selected_row = -1;
        m_logging_widget->info(tr("标签更改成功，上传数据中..."));
        QMessageBox::information(this, tr("标签更改"), tr("标签更改成功"));
        editor->push_org();
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
