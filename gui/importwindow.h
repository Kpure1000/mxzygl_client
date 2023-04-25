#ifndef MX_UPLOADWINDOW_H
#define MX_UPLOADWINDOW_H

#include "ifunctionwindow.h"
#include <QStringList>

#include "resource/assetinfo.h"

class PreviewWidget;

/**
 * @brief 资源导入
 */
class ImportWindow : public IFunctionWindow
{
    Q_OBJECT

public:
//    typedef res::AssetInfo::AssetType ImportType;
    using ImportType = res::AssetInfo::AssetType;

public:
    explicit ImportWindow(const QString &title, ImportType type, QWidget *parent = nullptr);

signals:

private:
    ImportType m_im_type;
    QStringList m_selected_files;

    PreviewWidget *m_previewWidget;

};

#endif // MX_UPLOADWINDOW_H
