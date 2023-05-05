#include "infotablewidget.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QHeaderView>
#include <QDebug>

InfoTableWidget::InfoTableWidget(QJsonObject *info, int spanNum, bool info_editable, QWidget *parent)
    : QTableWidget(parent), m_spanNum(spanNum), m_info(info), m_info_editable(info_editable)
{
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    connect(this, &QTableWidget::itemDoubleClicked, this, [this](QTableWidgetItem *item) {
        std::vector<int> rows;
        if (item->column() == 0) {
            int groupEnd = item->row() + m_spanNum;
            for (int r = item->row(); r < this->rowCount() && r < groupEnd; r++) {
                rows.push_back(r);
            }
        }
        if (rows.size() > 0)
            emit this->onSelectGroupToPreview(rows);
    });
    if (m_info_editable) {
        connect(this, &QTableWidget::itemChanged, this, [this](QTableWidgetItem *cur) {
            auto newText = cur->text();
            auto R = cur->row();
            auto C = cur->column();
            if (C <= 0 )
                return;
            auto headerName = this->horizontalHeaderItem(C)->text();
            auto vals = (*this->m_info)["data"].toArray();
            if (R >= vals.size()) {
                return;
            }
            auto row = vals[R].toObject();
            if (!row.contains(headerName)) {
                qDebug() << "InfoTableWidget>>QTableWidget::itemChanged>> m_info doesn't contain header " << headerName;
                return;
            }
            auto preText = row[headerName].toString();// 去除预览组一列
            row[headerName] = newText;
            vals[R] = row;
            (*this->m_info)["data"]=vals;
//            qDebug() << "InfoTableWidget>>QTableWidget::itemChanged>>, "
//                     << "from" << preText
//                     << "to" << newText
//                     << ", in table[" << R <<", " << C << "]";
        });
    }
}

void InfoTableWidget::refresh()
{
    auto headers = (*this->m_info)["headers"].toArray();
    if (headers.size() == 0) {
        qDebug() << "InfoTableWidget::refresh>> headers.size()==0";
        return;
    }
    setColumnCount(headers.size() + 1);

    auto rows = (*this->m_info)["data"].toArray();
    if (rows.size() == 0) {
        qDebug() << "InfoTableWidget::refresh>> data.size()==0";
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
            qDebug() << "InfoTableWidget::refresh>> element.size()==0";
            return;
        }
        auto previewItem = new QTableWidgetItem(std::to_string(row / m_spanNum + 1).c_str());
        previewItem->setToolTip(tr("双击以预览组"));
        previewItem->setFlags(previewItem->flags() ^= Qt::ItemIsEditable);
        setItem(row, 0, previewItem);
        int col = 1;
        for (const auto &header_name : headers) {
            auto dataItem = new QTableWidgetItem(key2value[header_name.toString()].toString());
            if (!m_info_editable)
                dataItem->setFlags(dataItem->flags() ^= Qt::ItemIsEditable);
            setItem(row, col, dataItem);
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
//    auto firstGroupItem = item(0, 0);
//    emit itemDoubleClicked(firstGroupItem);
//    setCurrentItem(firstGroupItem);
}

void InfoTableWidget::jumpTo(int row)
{
    selectRow(row);
    this->item(row, 0)->setSelected(false);
    scrollToItem(item(row, 0));
}

void InfoTableWidget::selectGroup(int group)
{
    emit itemDoubleClicked(item(std::min(group * m_spanNum, rowCount()), 0));
}

void InfoTableWidget::clearInfos()
{
    this->clearContents();
    std::vector<int> empty_row;
    emit onSelectGroupToPreview(empty_row);
}
