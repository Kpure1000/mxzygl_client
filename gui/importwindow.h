#ifndef MX_UPLOADWINDOW_H
#define MX_UPLOADWINDOW_H

#include "ifunctionwindow.h"
#include <QStringList>
#include <QWidget>
#include "function/assetimporter.h"

class PreviewWidget;

/**
 * @brief 资源导入
 */
class ImportWindow : public IFunctionWindow
{
    Q_OBJECT
public:
    explicit ImportWindow(QWidget *parent = nullptr);

private:
    static QWidget *setupImportWidget(QWidget *parent, AssetImporter *importer);
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;

signals:

private:
    bool _is_first_paint = true;

};

#endif // MX_UPLOADWINDOW_H
