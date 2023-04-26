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
    void onSelectedPane();
    void onModelLoaded(const QString& modelName);
    void onBVHLoaded(const QString& bvhName);

public slots:
    void doPreviewModel(const QString &assetName);
    void doPreviewBVH(const QString &assetName);
    void doClear();

private slots:
    void doShowRenderer(const QString& assetName);
    void doShowPreparing(const QString& assetName);
    void doShowDefault();

private:
    RenderWidget *m_renderWidget;
    QLabel *m_default;

    QLabel *m_info;

    bool m_linkTableItem;

};

#endif // MX_PREVIEWPANE_H
