#ifndef MX_PREVIEWPANE_H
#define MX_PREVIEWPANE_H

#include <QWidget>
#include <QLabel>
#include <memory>

#include "renderwidget.h"

class PreviewPane : public QWidget
{
    Q_OBJECT
public:
    explicit PreviewPane(QWidget *parent = nullptr, bool linkTableItem = true);

private:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

signals:
    void onPreviewPrepared(const QString& assetName);
    void onPreviewFailed(const QString& assetName);
    void onSelectedPane();
    void onModelLoaded(const QString& modelName);
    void onBVHLoaded(const QString& bvhName);

public slots:
    void doPreviewModel(const QString &filePath, const QString& previewInfo);
    void doPreviewBVH(const QString &filePath, const QString& previewInfo);
    void doClear();

private:
    void doShowRenderer(const QString& previewInfo);
    void doShowPreparing(const QString& previewInfo);
    void doShowError(const QString& previewInfo);
    void doShowDefault();

private:
    RenderWidget *m_renderWidget;
    QLabel *m_default;

    QLabel *m_info;

    bool m_linkTableItem;

};

#endif // MX_PREVIEWPANE_H
