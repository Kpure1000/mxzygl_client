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
    explicit ImportWindow(const QString &title, QWidget *parent = nullptr);

private:
    static QWidget *setupImportWidget(QWidget *parent, AssetImporter *importer);

signals:

private:

};

#endif // MX_UPLOADWINDOW_H
