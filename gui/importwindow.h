#ifndef MX_UPLOADWINDOW_H
#define MX_UPLOADWINDOW_H

#include "ifunctionwindow.h"
#include <QStringList>

#include "function/assetimporter.h"

class PreviewWidget;

/**
 * @brief 资源导入
 */
class ImportWindow : public IFunctionWindow
{
    Q_OBJECT
public:
    explicit ImportWindow(const QString &title, AssetImporter::ImportType type, QWidget *parent = nullptr);

signals:

private:
    AssetImporter *m_assetImporter;

    PreviewWidget *m_previewWidget;

};

#endif // MX_UPLOADWINDOW_H
