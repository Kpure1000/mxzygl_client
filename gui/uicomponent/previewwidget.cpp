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

#include "mxzygl.h"

PreviewWidget::PreviewWidget(int row, int column, PreviewType type, QWidget *parent)
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
    splitter->setOrientation(Qt::Orientation::Vertical);
    splitter->addWidget(panesWidget);
    splitter->setStretchFactor(0, 2);
    splitter->addWidget(m_infoTable);
    splitter->setStretchFactor(1, 1);

    ly_total->addWidget(splitter);

    connect(m_infoTable, &InfoTableWidget::onSelectGroupToPreview, this, &PreviewWidget::doPreview);
}

void PreviewWidget::setPreviewInfo(const std::vector<res::AssetInfo> &assets)
{
    QJsonObject info;
    QJsonArray headers;
    auto header_names = assets[0].getInfoNameList();
    for (auto& header : header_names) {
        headers.append(header);
    }
    QJsonArray data;
    for (auto& asset : assets) {
        data.append(*asset.getJsonObject());
    }
    info.insert("headers", headers);
    info.insert("data", data);
    m_infoTable->setInfo(info);
}

void PreviewWidget::doPreview(const std::vector<QTableWidgetItem *> &items, const QStringList &assetNames)
{
    for (size_t i = 0; i < static_cast<size_t>(m_previewNum); i++) {
        disconnect(m_previewPanes[i], &PreviewPane::onSelectedPane, this, 0);
        m_previewPanes[i]->doClear();
        if (i < m_previewPanes.size() && i < items.size()) {
            if (m_type == PreviewType::MODEL) {
                m_previewPanes[i]->doPreviewModel(assetNames[i]);
            } else if (m_type == PreviewType::BVH) {
                m_previewPanes[i]->doPreviewBVH(assetNames[i]);
            } else if (m_type == PreviewType::EFFECT) {
                // TODO: preview effect
            }
            connect(m_previewPanes[i], &PreviewPane::onSelectedPane, this, [items, i, this]() {
                m_infoTable->jumpTo(items[i]);
            });
        }
    }
}
