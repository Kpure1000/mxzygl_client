#include "previewpane.h"

#include <QFileInfo>
#include <QVBoxLayout>

#include <QMouseEvent>

#include "function/assetloader/modelloader.h"
#include "function/assetloader/bvhloader.h"

PreviewPane::PreviewPane(QWidget *parent, bool linkTableItem)
    : QWidget(parent), m_linkTableItem(linkTableItem)
{
    m_renderWidget = new RenderWidget(this);
    m_renderWidget->setStyleSheet("border: 1px solid #8f8f8f;");

    m_default = new QLabel(this);
    m_default->setAlignment(Qt::AlignmentFlag::AlignCenter);
    m_default->setStyleSheet("border: 1px solid #8f8f8f; font-size: 26px;");

    m_info = new QLabel(this);
    m_info->setStyleSheet("border: 1px solid #8f8f8f;");
    m_info->setAlignment(Qt::AlignmentFlag::AlignCenter);

    auto ly_v = new QVBoxLayout(this);
    ly_v->setSpacing(2);
    ly_v->addWidget(m_renderWidget,1);
    ly_v->addWidget(m_default, 1);
    ly_v->addWidget(m_info, 0);
    this->setLayout(ly_v);

    doClear();

    connect(this, &PreviewPane::onModelLoaded, m_renderWidget, &RenderWidget::doModelRendering);
    connect(this, &PreviewPane::onBVHLoaded, m_renderWidget, &RenderWidget::doBVHRendering);

    connect(this, &PreviewPane::onPreviewPrepared, this, &PreviewPane::doShowRenderer);

}

void PreviewPane::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (m_linkTableItem) {
        if (event->button() == Qt::MouseButton::LeftButton) {
            emit onSelectedPane();
        }
    }
}

void PreviewPane::doShowRenderer(const QString& assetName)
{
    m_info->setText(assetName);

    m_default->hide();
    m_renderWidget->show();
}

void PreviewPane::doShowPreparing(const QString& assetName)
{
    m_default->setText(tr("资源正在载入..."));
    m_info->setText(assetName);

    m_renderWidget->hide();
    m_default->show();
}

void PreviewPane::doShowDefault()
{
    m_default->setText(tr("无资源"));
    m_info->setText(tr("无名称"));

    m_renderWidget->hide();
    m_default->show();
}

void PreviewPane::doPreviewModel(const QString &filePath, const QString& assetName)
{
    doShowPreparing(assetName);
    ModelLoader::getInstance()->asyncLoad(filePath, assetName, [filePath, assetName, this]() {
        emit onModelLoaded(filePath);
        emit onPreviewPrepared(assetName);
    });
}

void PreviewPane::doPreviewBVH(const QString &filePath, const QString &assetName)
{
    doShowPreparing(assetName);
    // TODO load BVH file
}

void PreviewPane::doClear()
{
    doShowDefault();
    m_renderWidget->stopRendering();
}
