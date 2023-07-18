#include "infotreewidget.h"
#include <QHeaderView>
#include <QJsonArray>
#include <QDebug>

InfoTreeWidget::InfoTreeWidget(QJsonObject *root, QJsonObject *detail, bool resizeAsContent, QWidget *parent)
    : QTreeWidget{parent}, m_root{root}, m_detail{detail}, m_resizeAsContent{resizeAsContent}
{
//    this->header()->setSectionResizeMode(QHeaderView::ResizeMode::Interactive);
    this->header()->setDefaultAlignment(Qt::AlignLeft);
    connect(this, &QTreeWidget::itemClicked, this, [=](QTreeWidgetItem *item, int column) {
        if (nullptr == item->parent())
        {
            qDebug() << "QTreeWidget::itemClicked>> 点击了: " << item->text(column)
                     << ", index: " << indexOfTopLevelItem(item);
            emit onRootSelected(indexOfTopLevelItem(item));
        }
    });
}

void InfoTreeWidget::refreshTree()
{
    auto root_headers = (*this->m_root)["headers"].toArray();
    if (root_headers.size() != 1) {
        qDebug() << "InfoTreeWidget::refreshTree>> m_root headers size is not 1";
        return;
    }

    auto detail_headers = (*this->m_detail)["headers"].toArray();
    if (detail_headers.size() == 0) {
        qDebug() << "InfoTreeWidget::refreshTree>> m_detail headers size is 0";
        return;
    }

    QStringList headerLabels;
    for (const auto &h : root_headers) {
        const auto& hobj = h.toObject();
        if (hobj["visible"].toBool())
            headerLabels << hobj["name"].toString();
    }
    for (const auto & h : detail_headers) {
        const auto& hobj = h.toObject();
        if (hobj["visible"].toBool())
            headerLabels << h.toObject()["name"].toString();
    }

    setHeaderLabels(headerLabels);

    auto root_rows = (*this->m_root)["data"].toArray();
    if (root_rows.size() == 0) {
        qDebug() << "InfoTreeWidget::refreshTree>> root_rows size is 0";
        return;
    }

    auto detail_rows = (*this->m_detail)["data"].toArray();

    for (const auto &rroot : root_rows) {
        const auto &rObj = rroot.toObject();
        auto keyName = headerLabels[0];
        auto valName = rObj[headerLabels[0]].toString();
        auto topItem = new QTreeWidgetItem({valName});
        addTopLevelItem(topItem);
        for (const auto &rdetail : detail_rows) {
            const auto &dObj = rdetail.toObject();
            if (dObj[keyName] == valName) {
                QStringList detailInfo;
                detailInfo << dObj["name"].toString();
                for (int i = 1; i < headerLabels.size(); ++i) {
                    detailInfo << dObj[headerLabels[i]].toString();
                }
                auto detailItem = new QTreeWidgetItem(detailInfo);
                topItem->addChild(detailItem);
            }
        }
        topItem->setExpanded(true);
    }

    if (m_resizeAsContent)
    {
        header()->resizeSections(QHeaderView::ResizeMode::ResizeToContents);
    }
    else
    {
        header()->resizeSections(QHeaderView::ResizeMode::Interactive);
    }
}

void InfoTreeWidget::clearTree()
{
    this->clear();
    emit onRootSelected(-1);
}

void InfoTreeWidget::resetTree(QJsonObject *root, QJsonObject *detail)
{
    m_root = root;
    m_detail = detail;
    refreshTree();
}
