#ifndef MX_THUMBWIDGET_H
#define MX_THUMBWIDGET_H

#include <QWidget>

#include "previewwidget.h"

namespace Ui {
class ThumbWidget;
}

class ThumbWidget : public QWidget
{
    Q_OBJECT

public:
    using ThumbType = PreviewWidget::PreviewType;

    explicit ThumbWidget(QJsonObject *info, ThumbType type, int r, int c, QWidget *parent = nullptr);
    ~ThumbWidget();

//    int Column() const;
//    int Row() const;

//    PreviewPane *getPreviewPane(int row, int col) const;

    PreviewWidget *getPreviewWidget() const;

signals:
    void onThumbCreated();

private:
    Ui::ThumbWidget *ui;
    ThumbType m_Type;
    int m_Row, m_Column;
    PreviewWidget *m_previewWidget;
    //    std::vector<PreviewPane*> m_PreviewPanes;
};

#endif // MX_THUMBWIDGET_H
