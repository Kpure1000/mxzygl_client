#ifndef MX_INDEXWINDOW_H
#define MX_INDEXWINDOW_H

#include "gui/ifunctionwindow.h"
#include "function/indexeditor.h"

class LoggingWidget;
class InfoTableWidget;

class IndexWindow : public IFunctionWindow
{
    Q_OBJECT
public:
    explicit IndexWindow(QWidget *parent = nullptr);

signals:
    void onIndexSelected(IndexEditor::IndexType type);

private:
    QWidget *initModelWidget(IndexEditor *editor);
    QWidget *initIndexWidget(QTabWidget *tab_w, IndexEditor *index_editor_cont, IndexEditor *index_editor_tags, IndexEditor *index_editor_type);
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;
    bool _is_first_paint = true;
    LoggingWidget *m_logging_widget;
    InfoTableWidget *m_index_infotable;
    int m_selected_row = -1;
};

#endif // MX_INDEXWINDOW_H
