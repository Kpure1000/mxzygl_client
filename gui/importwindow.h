#ifndef MX_UPLOADWINDOW_H
#define MX_UPLOADWINDOW_H

#include "ifunctionwindow.h"
#include <QStringList>
#include <QWidget>
#include <QPushButton>
#include "function/assetimporter.h"

class PreviewWidget;
class LoggingWidget;
class WizardWidget;

/**
 * @brief 资源导入
 */
class ImportWindow : public IFunctionWindow
{
    Q_OBJECT
public:
    explicit ImportWindow(QWidget *parent = nullptr);

private:
    WizardWidget *setupModelWizard(AssetImporter *importer);
    WizardWidget *setupBVHWizard(AssetImporter *importer);
    WizardWidget *setupEffectWizard(AssetImporter *importer);

    QWidget *setupBrowseWidget(AssetImporter *importer, WizardWidget* wizard);
    // 模型部分界面
    QWidget *setupModel_UniformFormat(AssetImporter *importer, WizardWidget* wizard);
    QWidget *setupModel_TransformCamera(AssetImporter *importer, WizardWidget* wizard);
    QWidget *setupModel_ThumbUpload(AssetImporter *importer, WizardWidget* wizard);
    //
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;

signals:

private:
    bool _is_first_paint = true;
    LoggingWidget *m_logging_widget;
    int m_transform_row_selected = -1;

};

#endif // MX_UPLOADWINDOW_H
