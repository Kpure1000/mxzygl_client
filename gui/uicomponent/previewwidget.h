#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QWidget>
#include "resource/assetinfo.h"

class PreviewPane;
class InfoTableWidget;
class QTableWidgetItem;

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

#endif // PREVIEWWIDGET_H
