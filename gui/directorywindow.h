#ifndef MX_DIRECTORYWINDOW_H
#define MX_DIRECTORYWINDOW_H

#include <QTreeWidget>

#include "gui/ifunctionwindow.h"

class LoggingWidget;
class InfoTableWidget;
class InfoTreeWidget;
class IndexEditor;

class DirectoryWindow : public IFunctionWindow
{
    Q_OBJECT
public:
    explicit DirectoryWindow(const QString &title, QWidget *parent = nullptr);

signals:

protected:
    bool _is_first_paint = true;
};

/**
 * The Standard Classification Directory Window
 */
class SCDirWindow : public DirectoryWindow
{
    Q_OBJECT
public:
    explicit SCDirWindow(QWidget *parent = nullptr);

signals:

private:
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;
    QWidget *initModelWidget(IndexEditor *);

private:
    InfoTreeWidget *m_dir_tree;
    LoggingWidget *m_logging_widget;
    int m_selected_row = -1;
};

class TagDirWindow : public DirectoryWindow
{
    Q_OBJECT
public:
    explicit TagDirWindow(QWidget *parent = nullptr);

signals:

private:
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;
    QWidget *initModelWidget(IndexEditor *);

private:
    InfoTreeWidget *m_dir_tree;
    LoggingWidget *m_logging_widget;
    int m_selected_row = -1;
};

#endif // MX_DIRECTORYWINDOW_H
