#include "thumbwidget.h"
#include "ui_thumbwidget.h"

#include "previewwidget.h"

#include <QJsonObject>

ThumbWidget::ThumbWidget(QJsonObject *info, ThumbType type, int r, int c, QWidget *parent)
    : QWidget{parent}, ui{new Ui::ThumbWidget}, m_Type(type), m_Row{r}, m_Column{c}
{
    ui->setupUi(this);
//    for (int row = 0; row < m_Row; ++row) {
//        for (int col = 0; col < m_Column; ++col) {
//            m_PreviewPanes.emplace_back(new PreviewPane(this));
//            ui->ly_total->addWidget(m_PreviewPanes.back(), row, col);
//        }
//    }
    // TODO: 暂时用PreviewWidget替代
    m_previewWidget = new PreviewWidget(info, m_Row, m_Column, m_Type, Qt::Orientation::Horizontal, false, true, this);
    ui->ly_total->addWidget(m_previewWidget);

}

ThumbWidget::~ThumbWidget()
{
    delete ui;
}

PreviewWidget *ThumbWidget::getPreviewWidget() const
{
    return m_previewWidget;
}

//PreviewPane *ThumbWidget::getPreviewPane(int row, int col) const
//{
//    return m_PreviewPanes[row * m_Column + col];
//}

//int ThumbWidget::Column() const
//{
//    return m_Column;
//}

//int ThumbWidget::Row() const
//{
//    return m_Row;
//}
