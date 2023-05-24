#ifndef MX_TYPEMANAGERWINDOW_H
#define MX_TYPEMANAGERWINDOW_H

#include "gui/ifunctionwindow.h"

class TypeManager;
class LoggingWidget;

class TypeManagerWindow : public IFunctionWindow
{
    Q_OBJECT

public:
    explicit TypeManagerWindow(QWidget *parent = nullptr);
    ~TypeManagerWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    QWidget *initManagerWidget(TypeManager * manager);
    void initCategoryWidget();
    void initLoggingWidget();

private:
    TypeManager *m_modelManager, *m_bvhManager, *m_effectManager;
    int m_selected_row_model = -1, m_selected_row_bvh = -1, m_selected_row_effect = -1;
    LoggingWidget *m_logging_widget;
    bool _is_first_paint = true;
};

#endif // MX_TYPEMANAGERWINDOW_H
