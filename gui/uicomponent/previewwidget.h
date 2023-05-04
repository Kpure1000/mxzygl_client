#ifndef MX_PREVIEWWIDGET_H
#define MX_PREVIEWWIDGET_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QJsonObject>

#include "resource/assetinfo.h"

class PreviewPane;
class InfoTableWidget;

class PreviewWidget : public QWidget
{
    Q_OBJECT
public:
    using PreviewType = res::AssetInfo::AssetType;

    explicit PreviewWidget(QJsonObject *info,
                           int row,
                           int column,
                           PreviewType type,
                           Qt::Orientation split_orientation,
                           bool info_editable,
                           QWidget *parent = nullptr);

    void refreshInfos();

    void clearInfos();

    void PreviewFiles(const QStringList &filePaths, const QStringList &assetNames);

signals:
    void onPreview(const std::vector<int> &index);

private slots:
    void doPreviewPrepare(const std::vector<QTableWidgetItem *> &items);

private:
    InfoTableWidget *m_infoTable;
    std::vector<PreviewPane *> m_previewPanes;
    int m_previewNum;
    PreviewType m_type;
//    bool m_info_editable;

};

#endif // MX_PREVIEWWIDGET_H
