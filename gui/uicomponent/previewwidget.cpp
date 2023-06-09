#include "previewwidget.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QSplitter>
#include <QHeaderView>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "gui/uicomponent/previewpane.h"
#include "gui/uicomponent/infotablewidget.h"

PreviewWidget::PreviewWidget(QJsonObject *info, int row, int column, PreviewType type, Qt::Orientation split_orientation, bool info_editable, bool info_resizeAsContent, QWidget *parent)
    : QWidget{parent}, m_previewNum(row * column), m_type(type)
{
    auto ly_total = new QVBoxLayout(this);
    this->setLayout(ly_total);

    auto ly_pane = new QGridLayout();

    auto panesWidget = new QWidget(this);
    panesWidget->setStyleSheet("border:1px solid #8f8f8f;");
    panesWidget->setLayout(ly_pane);

    m_infoTable = new InfoTableWidget(info, m_previewNum, info_editable, info_resizeAsContent, this);

    for (int r = 0; r < row; r++) {
        for (int c = 0; c < column; c++) {
            auto previewPane = new PreviewPane(panesWidget);
            ly_pane->addWidget(previewPane, r, c);
            m_previewPanes.push_back(previewPane);
        }
    }

    auto splitter = new QSplitter(this);
    splitter->setOrientation(split_orientation);
    if (split_orientation == Qt::Orientation::Horizontal) {
        splitter->addWidget(m_infoTable);
        splitter->setStretchFactor(0, 1);
        splitter->addWidget(panesWidget);
        splitter->setStretchFactor(1, 2);
    } else if (split_orientation == Qt::Orientation::Vertical) {
        splitter->addWidget(panesWidget);
        splitter->setStretchFactor(0, 1);
        splitter->addWidget(m_infoTable);
        splitter->setStretchFactor(1, 1);
    }

    ly_total->addWidget(splitter);

}

void PreviewWidget::refreshInfo()
{
    connect(m_infoTable, &InfoTableWidget::onGroupSelected, this, &PreviewWidget::doPreviewPrepare);
    m_infoTable->refresh();
}

void PreviewWidget::clearInfo()
{
    m_infoTable->clearInfos();
    disconnect(m_infoTable, &InfoTableWidget::onGroupSelected, this, 0);
}

void PreviewWidget::selectGroup(int group)
{
    m_infoTable->selectGroup(group);
}

void PreviewWidget::previewFiles(const QStringList &filePaths, const QStringList &assetNames, bool cache)
{
    for (int i = 0; i < m_previewNum; i++) {
        if (i < filePaths.size()) {
            if (m_type == PreviewType::MODEL) {
                m_previewPanes[i]->doPreviewModel(filePaths[i], assetNames[i], cache);
            } else if (m_type == PreviewType::BVH) {
                m_previewPanes[i]->doPreviewBVH(filePaths[i], assetNames[i], cache);
            } else if (m_type == PreviewType::EFFECT) {
                // TODO: preview effect
            }
        }
    }
}

std::vector<PreviewPane *> PreviewWidget::getPreviewPane() const
{
    return m_previewPanes;
}

void PreviewWidget::doPreviewPrepare(const std::vector<int> &index)
{
    for (size_t i = 0; i < static_cast<size_t>(m_previewNum); i++) {
        disconnect(m_previewPanes[i], &PreviewPane::onSelectedPane, this, 0);
        m_previewPanes[i]->doClear();
        if (i < index.size()) {
            auto row = index[i];
            connect(m_previewPanes[i], &PreviewPane::onSelectedPane, this, [row, this]() {
                m_infoTable->jumpTo(row);
            });
        }
    }
    emit onPreview(index);
}
