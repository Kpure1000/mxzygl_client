#ifndef MX_UPLOADWINDOW_H
#define MX_UPLOADWINDOW_H

#include "ifunctionwindow.h"
#include <QStringList>
#include <QWidget>
#include <QPushButton>
#include "function/assetimporter.h"

class PreviewWidget;
class LoggingWidget;

/**
 * @brief 资源导入
 */
class ImportWindow : public IFunctionWindow
{
    Q_OBJECT
public:
    explicit ImportWindow(QWidget *parent = nullptr);

private:
    QWidget *setupImportWidget(AssetImporter *importer);
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;

signals:

private:
    bool _is_first_paint = true;
    LoggingWidget *m_logging_widget;

};

#endif // MX_UPLOADWINDOW_H
