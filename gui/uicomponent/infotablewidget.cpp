#include "infotablewidget.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QHeaderView>
#include <QDebug>

InfoTableWidget::InfoTableWidget(QWidget *parent, int spanNum)
    : QTableWidget(parent), m_spanNum(spanNum)
{
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    connect(this, &QTableWidget::itemDoubleClicked, this, [this](QTableWidgetItem *item) {
        std::vector<QTableWidgetItem*> previewItems;
        if (item->column() == 0) {
            int groupEnd = item->row() + m_spanNum;
            for (int r = item->row(); r < this->rowCount() && r < groupEnd; r++) {
                auto realPreviewItem = this->item(r, 1);
                previewItems.push_back(realPreviewItem);
            }
        }
        if (previewItems.size() > 0)
            emit this->onSelectGroupToPreview(previewItems);
    });
}

void InfoTableWidget::setInfos(const QJsonObject &info)
{
    auto headers = info["headers"].toArray();
    if (headers.size() == 0) {
        qDebug() << "InfoTableWidget::addInfo>> headers.size()==0";
        return;
    }
    setColumnCount(headers.size() + 1);

    auto rows = info["data"].toArray();
    if (rows.size() == 0) {
        qDebug() << "InfoTableWidget::addInfo>> data.size()==0";
        return;
    }
    setRowCount(rows.size());

    QStringList headers_str({tr("预览组")});
    for (const auto &header : headers)
        headers_str.append(header.toString());
    setHorizontalHeaderLabels(headers_str);

    for (int row = 0; row < rows.size(); row++) {
        auto key2value = rows[row].toObject();
        if (key2value.size() == 0) {
            qDebug() << "InfoTableWidget::addInfo>> element.size()==0";
            return;
        }
        auto previewItem = new QTableWidgetItem(std::to_string(row / m_spanNum + 1).c_str());
        previewItem->setToolTip(tr("双击以预览组"));
        previewItem->setFlags(previewItem->flags() ^= Qt::ItemIsEditable);
        setItem(row, 0, previewItem);
        int col = 1;
        for (const auto &header_name : headers) {
            setItem(row, col, new QTableWidgetItem(key2value[header_name.toString()].toString()));
            col++;
        }
    }

    // 每m_spanNum合并一次首列单元格
    for (int i = 0; i < rows.size(); i += m_spanNum) {
        auto spanOffset = std::min(m_spanNum, static_cast<int>(rows.size()) - i);
        if (spanOffset > 1) {
            setSpan(i, 0, spanOffset, 1);
        }
    }

    // 默认双击第一组预览并选中
    auto firstGroupItem = item(0, 0);
    emit itemDoubleClicked(firstGroupItem);
    setCurrentItem(firstGroupItem);
}

void InfoTableWidget::jumpTo(QTableWidgetItem *item)
{
    selectRow(item->row());
    this->item(item->row(), 0)->setSelected(false);
    scrollToItem(item);
}

void InfoTableWidget::clearInfos()
{
    this->clearContents();
    std::vector<QTableWidgetItem*>emptyItem;
    emit onSelectGroupToPreview(emptyItem);
}

void InfoTableWidget::doPreviewPaneSelected()
{

}
