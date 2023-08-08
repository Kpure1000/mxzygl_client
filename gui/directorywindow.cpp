#include "directorywindow.h"

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
#include "gui/uicomponent/infotreewidget.h"
#include "gui/uicomponent/previewpane.h"
#include "gui/uicomponent/renderwidget.h"
#include "gui/uicomponent/previewwidget.h"
#include "gui/uicomponent/loggingwidget.h"
#include "function/layoutmanager.h"
#include "function/indexeditor.h"
#include "function/renderer/renderer.h"

////////////////////////
//// DirectoryWindow
////////////////////////

DirectoryWindow::DirectoryWindow(const QString &title, QWidget *parent)
    : IFunctionWindow{title, {800, 600}, true, false, true, parent}
{

}

////////////////////////
//// SCDirWindow
////////////////////////

SCDirWindow::SCDirWindow(QWidget *parent)
    : DirectoryWindow{tr("三维模型标准分类目录管理"), parent}
{

    // ----------------视图----------------
    {
        auto top_menubar = new QMenuBar(this);
        auto menu_view = new QMenu(tr("视图(&V)"), this);

        // ----------------保存布局----------------
        menu_view->addAction(tr("保存布局"), this, [=]() {
            LayoutManager::getInstance()->save(this, "SCDirWindow");
        });
        // ----------------加载布局----------------
        menu_view->addAction(tr("加载布局"), this, [=]() {
            LayoutManager::getInstance()->restore(this, "SCDirWindow");
        });

        top_menubar->addMenu(menu_view);
        this->setMenuBar(top_menubar);
    }

    auto editor = new IndexEditor(IndexEditor::IndexType::CONTENT, this);

    auto center_widget = centralWidget();

    auto ly_total = new QGridLayout(center_widget);

    auto dirWidget = new QWidget(center_widget);

    auto ly_dir = new QHBoxLayout(dirWidget);

    m_dir_tree = new InfoTreeWidget(editor->getIndexInfo(),
                                    editor->getOriginInfo(),
                                    true,
                                    dirWidget);

    ly_dir->addWidget(m_dir_tree, 1);

    auto modelWidget = initModelWidget(editor);
    ly_total->addWidget(modelWidget);

    auto bt_sync = new QPushButton(tr("→\n→\n→\n同\n步\n修\n改\n到\n原\n始\n数\n据\n→\n→\n→\n"),center_widget);
    {
        bt_sync->setMaximumWidth(bt_sync->fontMetrics().averageCharWidth() * 4);
        auto plc_bt_sync = bt_sync->sizePolicy();
        plc_bt_sync.setVerticalPolicy(QSizePolicy::Expanding);
        bt_sync->setSizePolicy(plc_bt_sync);
    }

    ly_dir->addWidget(bt_sync,  0);

    connect(bt_sync, &QPushButton::clicked, this, [=]() {
        editor->syncToOrg();
    });

    {
        auto docker_index = new QDockWidget(tr("标签目录编辑"), center_widget);
        docker_index->setWidget(dirWidget);
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

void SCDirWindow::closeEvent(QCloseEvent *event)
{
    LayoutManager::getInstance()->save(this, "SCDirWindow");
    QWidget::closeEvent(event);
}

void SCDirWindow::showEvent(QShowEvent *event)
{
    if (_is_first_paint) {
        _is_first_paint = false;
        LayoutManager::getInstance()->restore(this, "SCDirWindow");
    }
    update();
}

QWidget *SCDirWindow::initModelWidget(IndexEditor *editor)
{
    auto modelWidget = new QWidget(centralWidget());

    auto ly_total = new QGridLayout(modelWidget);

    auto bt_pull = new QPushButton(tr("↓↓↓拉取数据↓↓↓"), modelWidget);

//    auto bt_push = new QPushButton(tr("↑↑↑推送数据↑↑↑"), modelWidget);

    auto ly_pp = new QHBoxLayout();
    ly_pp->addWidget(bt_pull);
//    ly_pp->addWidget(bt_push);

    auto bt_compress = new QPushButton(tr("←\n←\n←\n创\n建\n标\n准\n分\n类\n目\n录\n←\n←\n←\n"), modelWidget);
    {
        bt_compress->setMaximumWidth(bt_compress->fontMetrics().averageCharWidth() * 4);
        auto plc_bt_sync = bt_compress->sizePolicy();
        plc_bt_sync.setVerticalPolicy(QSizePolicy::Expanding);
        bt_compress->setSizePolicy(plc_bt_sync);
    }

    auto preview_w = new PreviewWidget(editor->getOriginInfo(), 1, 3, PreviewWidget::PreviewType::MODEL, Qt::Vertical, true, true, modelWidget);

    // editor-modify
    auto gb_modify = new QGroupBox(tr("标准分类目录编辑"), modelWidget);
    gb_modify->setEnabled(false);

    auto lb_modify_hint = new QLabel(tr("→更改为→"), gb_modify);
    auto lb_toModify = new QLabel("\"\"", gb_modify);

    auto le_modify_input = new QLineEdit(gb_modify);
    le_modify_input->setPlaceholderText("请输入标准分类名称, 不能为空");

    auto ly_modify_hint = new QHBoxLayout();

    ly_modify_hint->addWidget(lb_toModify, 0);
    ly_modify_hint->addWidget(lb_modify_hint, 0);
    ly_modify_hint->addWidget(le_modify_input, 0);
    ly_modify_hint->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding));

    auto bt_modify = new QPushButton(tr("更改标准分类名"), gb_modify);

    auto ly_gb_modify = new QVBoxLayout(gb_modify);
    ly_gb_modify->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Expanding));
    ly_gb_modify->addLayout(ly_modify_hint, 0);
    ly_gb_modify->addWidget(bt_modify, 0);
    ly_gb_modify->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Expanding));
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
//    connect(bt_push, &QPushButton::clicked, this, [=]() {
//        editor->push_org();
//    });
    connect(bt_compress, &QPushButton::clicked, this, [=]() {
        editor->compressToIndex();
    });
    connect(preview_w, &PreviewWidget::onPreview, this, [=](const std::vector<int> &index) {
        preview_w->previewFiles(editor->getFilePaths(index), editor->getPreviewInfo(index), true);
        auto panes = preview_w->getPreviewPane();
        for (size_t i = 0; i < panes.size(); i++) {
            if (i < index.size()) {
                auto renderer = panes[i]->getRenderWidget()->getRenderer();
                auto trans_pair = editor->getTransform(index[i]);
                renderer->setModelTransform(trans_pair.first);
                renderer->setCameraTransform(trans_pair.second);
            }
        }
    });

    connect(editor, &IndexEditor::onResponsing, this, [=](const QString & info, bool is_continue){
        is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
    });
    connect(editor, &IndexEditor::onPullSuccessful, this, [=]() {
        preview_w->refreshInfo();
//        QMessageBox::information(this, tr("数据拉取"), tr("数据拉取成功!"));
        // 自动压缩
        bt_compress->click();
    });
    connect(editor, &IndexEditor::onPushSuccessful, this, [=]() {
        preview_w->refreshInfo();
        QMessageBox::information(this, tr("数据推送"), tr("数据推送成功!"));
        // 自动更新
        bt_pull->click();
    });
    connect(editor, &IndexEditor::onCompressed, this, [=]() {
        m_logging_widget->info(tr("创建标准分类目录成功"));
        m_dir_tree->clearTree();
        m_dir_tree->refreshTree();
        QMessageBox::information(this, tr("创建标准分类目录"), tr("创建标准分类目录成功!"));
    });
    connect(editor, &IndexEditor::onSyncToOrg, this, [=]() {
        m_logging_widget->info(tr("标准分类目录同步成功"));
        QMessageBox::information(this, tr("标准分类目录同步"), tr("标准分类目录同步成功!"));
    });

    // 更改标签功能
    connect(m_dir_tree, &InfoTreeWidget::onRootSelected, this, [=](int row) {
        m_selected_row = row;
        if (m_selected_row == -1) {
            lb_toModify->setText("\"\"");
        } else {
            gb_modify->setEnabled(true);
            lb_toModify->setText(QString().asprintf("\"%s\"", editor->index(row).toStdString().c_str()));
        }
    });

    // 更改功能
    connect(bt_modify, &QPushButton::clicked, this, [=]() {
        if (m_selected_row == -1) {
            auto info = tr("未选择要更改的标准分类, 更改失败! 请<b>点击目录根节点</b>进行选择!");
            m_logging_widget->error(info);
            QMessageBox::warning(this, tr("未选择要更改的标准分类, 更改失败"), info);
            return;
        }
        auto tagName = le_modify_input->text();
        if (tagName == "") {
            auto info = tr("标准分类目录名<b>不能为空</b>, 更改失败!");
            m_logging_widget->error(info);
            QMessageBox::warning(this, tr("标准分类目录名为空, 更改失败"), info);
            return;
        }
        editor->modify(m_selected_row, tagName);
    });
    connect(editor, &IndexEditor::onModifyRepeat, this, [=](const QString &scName) {
        auto info = tr("无法更改为<b>同名标准分类</b>\" ") + scName + tr(" \", 更改失败!");
        m_logging_widget->error(info);
        QMessageBox::warning(this, tr("无法更改同名标准分类, 更改失败"), info);
    });
    connect(editor, &IndexEditor::onModifySuccessed, this, [=]() {
        m_dir_tree->clearTree();
        m_dir_tree->refreshTree();
        le_modify_input->clear();
        lb_toModify->setText("\"\"");
        m_selected_row = -1;
        m_logging_widget->info(tr("标准分类更改成功，上传数据中..."));
        editor->push_org();
        QMessageBox::information(this, tr("标准分类更改"), tr("标准分类更改成功"));
    });

    return modelWidget;
}

////////////////////////
//// TagDirWindow
////////////////////////

TagDirWindow::TagDirWindow(QWidget *parent)
    : DirectoryWindow{tr("三维模型标签目录管理"), parent}
{

    // ----------------视图----------------
    {
        auto top_menubar = new QMenuBar(this);
        auto menu_view = new QMenu(tr("视图(&V)"), this);

        // ----------------保存布局----------------
        menu_view->addAction(tr("保存布局"), this, [=]() {
            LayoutManager::getInstance()->save(this, "TagDirWindow");
        });
        // ----------------加载布局----------------
        menu_view->addAction(tr("加载布局"), this, [=]() {
            LayoutManager::getInstance()->restore(this, "TagDirWindow");
        });

        top_menubar->addMenu(menu_view);
        this->setMenuBar(top_menubar);
    }

    auto editor = new IndexEditor(IndexEditor::IndexType::TAGS, this);

    auto center_widget = centralWidget();

    auto ly_total = new QGridLayout(center_widget);

    auto dirWidget = new QWidget(center_widget);

    auto ly_dir = new QHBoxLayout(dirWidget);

    m_dir_tree = new InfoTreeWidget(editor->getIndexInfo(),
                                    editor->getOriginInfo(),
                                    true,
                                    dirWidget);

    ly_dir->addWidget(m_dir_tree, 1);

    auto modelWidget = initModelWidget(editor);
    ly_total->addWidget(modelWidget);

    auto bt_sync = new QPushButton(tr("→\n→\n→\n同\n步\n修\n改\n到\n原\n始\n数\n据\n→\n→\n→\n"),center_widget);
    {
        bt_sync->setMaximumWidth(bt_sync->fontMetrics().averageCharWidth() * 4);
        auto plc_bt_sync = bt_sync->sizePolicy();
        plc_bt_sync.setVerticalPolicy(QSizePolicy::Expanding);
        bt_sync->setSizePolicy(plc_bt_sync);
    }

    ly_dir->addWidget(bt_sync,  0);

    connect(bt_sync, &QPushButton::clicked, this, [=]() {
        editor->syncToOrg();
    });

    {
        auto docker_index = new QDockWidget(tr("标签目录编辑"), center_widget);
        docker_index->setWidget(dirWidget);
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

void TagDirWindow::closeEvent(QCloseEvent *event)
{
    LayoutManager::getInstance()->save(this, "TagDirWindow");
    QWidget::closeEvent(event);
}

void TagDirWindow::showEvent(QShowEvent *event)
{
    if (_is_first_paint) {
        _is_first_paint = false;
        LayoutManager::getInstance()->restore(this, "TagDirWindow");
    }
    update();
}

QWidget *TagDirWindow::initModelWidget(IndexEditor *editor)
{
    auto modelWidget = new QWidget(centralWidget());

    auto ly_total = new QGridLayout(modelWidget);

    auto bt_pull = new QPushButton(tr("↓↓↓拉取数据↓↓↓"), modelWidget);

//    auto bt_push = new QPushButton(tr("↑↑↑推送数据↑↑↑"), modelWidget);

    auto ly_pp = new QHBoxLayout();
    ly_pp->addWidget(bt_pull);
//    ly_pp->addWidget(bt_push);

    auto bt_compress = new QPushButton(tr("←\n←\n←\n创\n建\n标\n签\n目\n录\n←\n←\n←\n"), modelWidget);
    {
        bt_compress->setMaximumWidth(bt_compress->fontMetrics().averageCharWidth() * 4);
        auto plc_bt_sync = bt_compress->sizePolicy();
        plc_bt_sync.setVerticalPolicy(QSizePolicy::Expanding);
        bt_compress->setSizePolicy(plc_bt_sync);
    }

    auto preview_w = new PreviewWidget(editor->getOriginInfo(), 1, 3, PreviewWidget::PreviewType::MODEL, Qt::Vertical, true, true, modelWidget);

    // editor-modify
    auto gb_modify = new QGroupBox(tr("标签目录编辑"), modelWidget);
    gb_modify->setEnabled(false);

    auto lb_modify_hint = new QLabel(tr("→更改为→"), gb_modify);
    auto lb_toModify = new QLabel("\"\"", gb_modify);

    auto le_modify_input = new QLineEdit(gb_modify);
    le_modify_input->setPlaceholderText("请输入标签名称, 不能为空");

    auto ly_modify_hint = new QHBoxLayout();

    ly_modify_hint->addWidget(lb_toModify, 0);
    ly_modify_hint->addWidget(lb_modify_hint, 0);
    ly_modify_hint->addWidget(le_modify_input, 0);
    ly_modify_hint->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding));

    auto bt_modify = new QPushButton(tr("更改标签名"), gb_modify);

    auto ly_gb_modify = new QVBoxLayout(gb_modify);
    ly_gb_modify->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Expanding));
    ly_gb_modify->addLayout(ly_modify_hint, 0);
    ly_gb_modify->addWidget(bt_modify, 0);
    ly_gb_modify->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Expanding));
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
//    connect(bt_push, &QPushButton::clicked, this, [=]() {
//        editor->push_org();
//    });
    connect(bt_compress, &QPushButton::clicked, this, [=]() {
        editor->compressToIndex();
    });
    connect(preview_w, &PreviewWidget::onPreview, this, [=](const std::vector<int> &index) {
        preview_w->previewFiles(editor->getFilePaths(index), editor->getPreviewInfo(index), true);
        auto panes = preview_w->getPreviewPane();
        for (size_t i = 0; i < panes.size(); i++) {
            if (i < index.size()) {
                auto renderer = panes[i]->getRenderWidget()->getRenderer();
                auto trans_pair = editor->getTransform(index[i]);
                renderer->setModelTransform(trans_pair.first);
                renderer->setCameraTransform(trans_pair.second);
            }
        }
    });

    connect(editor, &IndexEditor::onResponsing, this, [=](const QString & info, bool is_continue){
        is_continue ? m_logging_widget->info(info) : m_logging_widget->warning(info);
    });
    connect(editor, &IndexEditor::onPullSuccessful, this, [=]() {
        preview_w->refreshInfo();
//        QMessageBox::information(this, tr("数据拉取"), tr("数据拉取成功!"));
        // 自动压缩
        bt_compress->click();
    });
    connect(editor, &IndexEditor::onPushSuccessful, this, [=]() {
        preview_w->refreshInfo();
        QMessageBox::information(this, tr("数据推送"), tr("数据推送成功!"));
        // 自动更新
        bt_pull->click();
    });
    connect(editor, &IndexEditor::onCompressed, this, [=]() {
        m_logging_widget->info(tr("创建标签目录成功"));
        m_dir_tree->clearTree();
        m_dir_tree->refreshTree();
        QMessageBox::information(this, tr("创建标签目录"), tr("创建标签目录成功!"));
    });
    connect(editor, &IndexEditor::onSyncToOrg, this, [=]() {
        m_logging_widget->info(tr("标签目录同步成功"));
        QMessageBox::information(this, tr("标签目录同步"), tr("标签目录同步成功!"));
    });

    // 更改标签功能
    connect(m_dir_tree, &InfoTreeWidget::onRootSelected, this, [=](int row) {
        m_selected_row = row;
        gb_modify->setEnabled(true);
        lb_toModify->setText(QString().asprintf("\"%s\"", editor->index(row).toStdString().c_str()));
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
        m_dir_tree->clearTree();
        m_dir_tree->refreshTree();
        le_modify_input->clear();
        lb_toModify->setText("\"\"");
        m_selected_row = -1;
        m_logging_widget->info(tr("标签更改成功，上传数据中..."));
        QMessageBox::information(this, tr("标签更改"), tr("标签更改成功"));
        editor->push_org();
    });

    return modelWidget;
}
