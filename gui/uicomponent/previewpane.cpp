#include "previewpane.h"

#include <QFileInfo>
#include <QVBoxLayout>
#include <QDir>

#include <QMouseEvent>
#include <QDebug>

#include "function/assetloader/modelloader.h"
#include "function/assetloader/bvhloader.h"
#include "gui/uicomponent/renderwidget.h"

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
    m_info->setContentsMargins(0, 5, 0, 5);

    auto ly_v = new QVBoxLayout(this);
    ly_v->setSpacing(2);
    ly_v->addWidget(m_renderWidget,1);
    ly_v->addWidget(m_default, 1);
    ly_v->addWidget(m_info, 0);
    this->setLayout(ly_v);

    doShowDefault();

    connect(this, &PreviewPane::onModelCached, m_renderWidget, QOverload<const QString&>::of(&RenderWidget::doModelRendering), Qt::QueuedConnection);
    connect(this, &PreviewPane::onBVHCached, m_renderWidget, QOverload<const QString&>::of(&RenderWidget::doBVHRendering), Qt::QueuedConnection);

    qRegisterMetaType<std::shared_ptr<res::Model>>("std::shared_ptr<res::Model>");
    qRegisterMetaType<std::shared_ptr<res::BVH>>("std::shared_ptr<res::BVH>");
    connect(this, &PreviewPane::onModelLoaded, m_renderWidget, QOverload<std::shared_ptr<res::Model>>::of(&RenderWidget::doModelRendering), Qt::QueuedConnection);
    connect(this, &PreviewPane::onBVHLoaded, m_renderWidget, QOverload<std::shared_ptr<res::BVH>>::of(&RenderWidget::doBVHRendering), Qt::QueuedConnection);

    connect(this, &PreviewPane::onPreviewPrepared, this, &PreviewPane::doShowRenderer, Qt::QueuedConnection);
    connect(this, &PreviewPane::onEffectPrepared, this, &PreviewPane::doShowEffect, Qt::QueuedConnection);
    connect(this, &PreviewPane::onPreviewFailed, this, &PreviewPane::doShowError, Qt::QueuedConnection);

}

RenderWidget *PreviewPane::getRenderWidget() const { return m_renderWidget; }

void PreviewPane::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (m_linkTableItem) {
        if (event->button() == Qt::MouseButton::LeftButton) {
            emit onSelectedPane();
        }
    }
}

void PreviewPane::doShowRenderer(const QString& previewInfo)
{
    m_info->setText(previewInfo);

    m_default->hide();
    m_renderWidget->show();
}

void PreviewPane::doShowEffect(const QString &previewInfo)
{
    m_default->setText(tr("双击此处\n打开特效文件"));
    m_info->setText(previewInfo);

    m_renderWidget->hide();
    m_default->show();
}

void PreviewPane::doShowPreparing(const QString& previewInfo)
{
    m_default->setText(tr("资源正在载入..."));
    m_info->setText(previewInfo);

    m_renderWidget->hide();
    m_default->show();
}

void PreviewPane::doShowError(const QString &previewInfo)
{
    m_default->setText(tr("资源载入失败"));
    m_info->setText(previewInfo);

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

void PreviewPane::doPreviewModel(const QString &filePath, const QString& previewInfo, bool cache)
{
    doShowPreparing(previewInfo);
    if (cache) {
        ModelLoader::getInstance()->cachedAsyncLoad(filePath, [filePath, previewInfo, this](bool is_loaded) {
            if (is_loaded) {
                emit onPreviewPrepared(previewInfo);
                emit onModelCached(filePath);
            } else {
                emit onPreviewFailed(previewInfo);
            }
        });
    } else {
        ModelLoader::getInstance()->tempAsyncLoad(filePath, [filePath, previewInfo, this](std::shared_ptr<res::Model> model) {
            if (nullptr != model) {
                emit onPreviewPrepared(previewInfo);
                emit onModelLoaded(model);
            } else {
                emit onPreviewFailed(previewInfo);
            }
        });
    }
}

void PreviewPane::doPreviewBVH(const QString &filePath, const QString &previewInfo, bool cache)
{
    doShowPreparing(previewInfo);
    if (cache) {
        BVHLoader::getInstance()->cachedAsyncLoad(filePath, [filePath, previewInfo, this](bool is_loaded) {
            if (is_loaded) {
                emit onPreviewPrepared(previewInfo);
                emit onBVHCached(filePath);
            } else {
                emit onPreviewFailed(previewInfo);
            }
        });
    } else {
        BVHLoader::getInstance()->tempAsyncLoad(filePath, [filePath, previewInfo, this](std::shared_ptr<res::BVH> bvh) {
            if (nullptr != bvh) {
                emit onPreviewPrepared(previewInfo);
                emit onBVHLoaded(bvh);
            } else {
                emit onPreviewFailed(previewInfo);
            }
        });
    }
}

void PreviewPane::doPreviewEffect(const QString &filePath, const QString &previewInfo)
{
    doShowPreparing(previewInfo);
    QDir dir(filePath);
//    qDebug() << dir;
    if (dir.exists(filePath))
    {
        emit onEffectPrepared(previewInfo);
    }
    else
    {
        emit onPreviewFailed(previewInfo);
    }
}

void PreviewPane::doClear()
{
    doShowDefault();
    m_renderWidget->stopRendering();
}
