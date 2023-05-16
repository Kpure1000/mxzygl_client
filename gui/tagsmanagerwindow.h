#ifndef MX_TAGSMANAGERWINDOW_H
#define MX_TAGSMANAGERWINDOW_H


#include "gui/ifunctionwindow.h"

class TagsManager;
class LoggingWidget;

class TagsManagerWindow : public IFunctionWindow
{
    Q_OBJECT

public:
    explicit TagsManagerWindow(QWidget *parent = nullptr);
    ~TagsManagerWindow();

private:
    QWidget *initManagerWidget(TagsManager * manager);
    void initCategoryWidget();
    void initLoggingWidget();

private:
    TagsManager *m_modelManager, *m_bvhManager, *m_effectManager;
    int m_selected_row_model = -1, m_selected_row_bvh = -1, m_selected_row_effect = -1;
    LoggingWidget *m_logging_widget;
};

#endif // MX_TAGSMANAGERWINDOW_H
