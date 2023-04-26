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

PreviewWidget::PreviewWidget(int row, int column, PreviewType type, Qt::Orientation split_orientation, QWidget *parent)
    : QWidget{parent}, m_previewNum(row * column), m_type(type)
{
    auto ly_total = new QVBoxLayout(this);
    this->setLayout(ly_total);

    auto ly_pane = new QGridLayout();

    auto panesWidget = new QWidget(this);
    panesWidget->setStyleSheet("border:1px solid #8f8f8f;");
    panesWidget->setLayout(ly_pane);

    m_infoTable = new InfoTableWidget(this, m_previewNum);

    for (int r = 0; r < row; r++) {
        for (int c = 0; c < column; c++) {
            auto previewPane = new PreviewPane(this);
            ly_pane->addWidget(previewPane, r, c);
            m_previewPanes.push_back(previewPane);
        }
    }

    auto splitter = new QSplitter(this);
    splitter->setOrientation(split_orientation);
    splitter->addWidget(m_infoTable);
    splitter->setStretchFactor(0, 1);
    splitter->addWidget(panesWidget);
    splitter->setStretchFactor(1, 2);

    ly_total->addWidget(splitter);

    connect(m_infoTable, &InfoTableWidget::onSelectGroupToPreview, this, &PreviewWidget::doPreviewPrepare);
}

void PreviewWidget::setInfos(const QJsonObject &info)
{
    m_infoTable->setInfos(info);
}

void PreviewWidget::clearInfos()
{
    m_infoTable->clearInfos();
}

void PreviewWidget::PreviewFiles(const QStringList &filePaths, const QStringList &assetNames)
{
    for (int i = 0; i < m_previewNum; i++) {
        if (i < filePaths.size()) {
            if (m_type == PreviewType::MODEL) {
                m_previewPanes[i]->doPreviewModel(filePaths[i], assetNames[i]);
            } else if (m_type == PreviewType::BVH) {
                m_previewPanes[i]->doPreviewBVH(filePaths[i], assetNames[i]);
            } else if (m_type == PreviewType::EFFECT) {
                // TODO: preview effect
            }
        }
    }
}

void PreviewWidget::doPreviewPrepare(const std::vector<QTableWidgetItem *> &items)
{
    std::vector<int> index;
    for (size_t i = 0; i < static_cast<size_t>(m_previewNum); i++) {
        disconnect(m_previewPanes[i], &PreviewPane::onSelectedPane, this, 0);
        m_previewPanes[i]->doClear();
        if (i < items.size()) {
            connect(m_previewPanes[i], &PreviewPane::onSelectedPane, this, [items, i, this]() {
                m_infoTable->jumpTo(items[i]);
            });
            index.push_back(items[i]->row());
        }
    }
    emit onPreview(index);
}
