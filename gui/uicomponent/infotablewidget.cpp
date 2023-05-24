#include "infotablewidget.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QHeaderView>
#include <QDebug>
#include <QBrush>
#include <QComboBox>

const QColor InfoTableWidget::m_editable_col = QColor{235, 255, 235, 255};
const QColor InfoTableWidget::m_uneditable_col = QColor{255, 245, 235, 255};
const QColor InfoTableWidget::m_group_col = QColor{235, 245, 255, 255};

InfoTableWidget::InfoTableWidget(QJsonObject *info, int spanNum, bool info_editable, QWidget *parent)
    : QTableWidget(parent), m_spanNum(spanNum), m_info(info), m_info_editable(info_editable)
{
    this->setStyleSheet("border:1px solid #8f8f8f;");
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Interactive);
    horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    connect(this, &QTableWidget::itemDoubleClicked, this, [this](QTableWidgetItem *item) {
        std::vector<int> rows;
        if (item->column() == 0) {
            int groupEnd = item->row() + m_spanNum;
            for (int r = item->row(); r < this->rowCount() && r < groupEnd; r++) {
                rows.push_back(r);
            }
        }
        if (rows.size() > 0)
            emit this->onGroupSelected(rows);
    });
    if (m_info_editable) {
        connect(this, &QTableWidget::itemChanged, this, [this](QTableWidgetItem *cur) {
            auto newText = cur->text();
            auto R = cur->row();
            auto C = cur->column();
            if (C <= 0 )
                return;
            auto headerName = this->horizontalHeaderItem(C)->text();
            auto headers = (*this->m_info)["headers"].toArray();
            auto vals = (*this->m_info)["data"].toArray();
            if (R >= vals.size()) {
                return;
            }
            auto row = vals[R].toObject();
            if (!row.contains(headerName)) {
                qDebug() << "InfoTableWidget>>QTableWidget::itemChanged>> m_info doesn't contain header " << headerName;
                return;
            }
            QString preText;
            bool is_array = false;
            for (auto header : headers) {
                if (header.toObject()["name"].toString() == headerName) {
                    is_array = header.toObject()["is_array"].toBool();
                }
            }
            if (is_array) {
                auto row_obj = row[headerName].toObject();
                preText = row_obj["value"].toString();
                row_obj["value"] = newText;
                row[headerName] = row_obj;
            } else {
                preText = row[headerName].toString();
                row[headerName] = newText;
            }
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

    auto rows = (*this->m_info)["data"].toArray();
    setRowCount(rows.size());
    if (rows.size() == 0) {
        qDebug() << "InfoTableWidget::refresh>> data.size()==0";
        return;
    }

    QStringList headers_str({tr("预览组")});
    for (const auto &header : headers) {
        QJsonObject header_ele = header.toObject();
        if (header_ele["visible"].toBool())
            headers_str.append(header_ele["name"].toString());
    }
    setColumnCount(headers_str.size());
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
        QBrush brush = previewItem->background();
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        brush.setColor(m_group_col);
        previewItem->setBackground(brush);
        setItem(row, 0, previewItem);
        int col = 1;
        for (const auto &header : headers) {
            QJsonObject header_ele = header.toObject();
            if (header_ele["visible"].toBool()) {
                QTableWidgetItem *dataItem;
                if (header_ele["is_array"].toBool()) {
                    dataItem = new QTableWidgetItem("");
                    auto data_array = key2value[header_ele["name"].toString()].toObject()["array"].toArray();
                    auto combo = new QComboBox(this);
                    for (const auto &d : data_array) {
                        combo->addItem(d.toString());
                    }
                    connect(combo, &QComboBox::currentTextChanged, this, [=](const QString &t) {
                        dataItem->setText(t);
                    });
                    if (data_array.size() > 0) {
                        combo->setCurrentIndex(0);
                        dataItem->setText(combo->itemText(0));
                    }
                    combo->setStyleSheet(QString().asprintf("background-color: #%04X; color:black;", m_editable_col.rgb()));
                    this->setCellWidget(row, col, combo);
                } else {
                    dataItem = new QTableWidgetItem(key2value[header_ele["name"].toString()].toString());
                }
                if (!m_info_editable || !header_ele["editable"].toBool()) {
                    dataItem->setFlags(dataItem->flags() ^= Qt::ItemIsEditable);
                    QBrush brush = dataItem->background();
                    brush.setStyle(Qt::BrushStyle::SolidPattern);
                    brush.setColor(m_uneditable_col);
                    dataItem->setBackground(brush);
                    dataItem->setToolTip(tr("不可编辑"));
                } else {
                    QBrush brush = dataItem->background();
                    brush.setStyle(Qt::BrushStyle::SolidPattern);
                    brush.setColor(m_editable_col);
                    dataItem->setBackground(brush);
                    dataItem->setToolTip(tr("可编辑"));
                }
                setItem(row, col, dataItem);
                col++;
            }
        }
    }

    // 每m_spanNum合并一次首列单元格
    for (int i = 0; i < rows.size(); i += m_spanNum) {
        auto spanOffset = std::min(m_spanNum, static_cast<int>(rows.size()) - i);
        if (spanOffset > 1) {
            setSpan(i, 0, spanOffset, 1);
        }
    }

    horizontalHeader()->resizeSections(QHeaderView::ResizeMode::ResizeToContents);
}

void InfoTableWidget::jumpTo(int row)
{
    selectRow(row);
    this->item(row, 0)->setSelected(false);
    scrollToItem(item(row, 0));
}

void InfoTableWidget::selectGroup(int group)
{
    if (this->rowCount() > 0)
        emit itemDoubleClicked(item(std::min(group * m_spanNum, rowCount() - 1), 0));
}

void InfoTableWidget::clearInfos()
{
    this->clearContents();
    std::vector<int> empty_row;
    emit onGroupSelected(empty_row);
}
