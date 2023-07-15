#ifndef MX_PREVIEWPANE_H
#define MX_PREVIEWPANE_H

#include <QWidget>
#include <QLabel>
#include <memory>

namespace res {
struct Model;
struct BVH;
}
class RenderWidget;
class PreviewPane : public QWidget
{
    Q_OBJECT
public:
    explicit PreviewPane(QWidget *parent = nullptr, bool linkTableItem = true);

    RenderWidget *getRenderWidget() const;

private:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

signals:
    void onPreviewPrepared(const QString& assetName);
    void onEffectPrepared(const QString& assetName);
    void onPreviewFailed(const QString& assetName);
    void onSelectedPane();
    void onModelCached(const QString& modelName);
    void onBVHCached(const QString& bvhName);
    void onModelLoaded(std::shared_ptr<res::Model> model);
    void onBVHLoaded(std::shared_ptr<res::BVH> model);


public slots:
    void doPreviewModel(const QString &filePath, const QString& previewInfo, bool cache);
    void doPreviewBVH(const QString &filePath, const QString& previewInfo, bool cache);
    void doPreviewEffect(const QString &filePath, const QString& previewInfo);
    void doClear();

private:
    void doShowRenderer(const QString& previewInfo);
    void doShowEffect(const QString& previewInfo);
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
