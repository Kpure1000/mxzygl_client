#ifndef MX_PREVIEWWIDGET_H
#define MX_PREVIEWWIDGET_H

#include <QWidget>
#include <QTableWidgetItem>

#include "resource/assetinfo.h"

class PreviewPane;
class InfoTableWidget;

class PreviewWidget : public QWidget
{
    Q_OBJECT
public:
    using PreviewType = res::AssetInfo::AssetType;

    explicit PreviewWidget(int row,
                           int column,
                           PreviewType type,
                           QWidget *parent = nullptr);

    void setPreviewInfo(const std::vector<res::AssetInfo> &assets);

signals:

private slots:
    void doPreview(const std::vector<QTableWidgetItem *> &items, const QStringList &assetNames);

private:
    InfoTableWidget *m_infoTable;
    std::vector<PreviewPane *> m_previewPanes;
    int m_previewNum;
    PreviewType m_type;

};

#endif // MX_PREVIEWWIDGET_H
