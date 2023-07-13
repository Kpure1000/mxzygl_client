#include "versionpainter.h"

#include <QGraphicsView>
#include <QHBoxLayout>
#include <QGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QDebug>
#include <QVector3D>

VersionPainter::VersionPainter(QWidget *parent)
    : QWidget{parent}
{
    auto ly_total = new QHBoxLayout(this);
    auto gscene = new QGraphicsScene(this);
    auto gview = new QGraphicsView(gscene, this);
    ly_total->addWidget(gview);

    int version_count = 2;
    auto start_color = QVector3D{50, 60, 90};
    auto end_color = QVector3D{170, 180, 210};
    auto sub_color = (end_color - start_color) / version_count;

    for (int i = 0; i < version_count; ++i) {
        float x = 50 + i * 250, y = 150;
        QRectF rect{x, y, 150, 100};
        auto ritem = new QGraphicsRectItem(rect);
        ritem->setFlags(ritem->flags() | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable);
        auto cur_color = start_color + sub_color * i;
        ritem->setBrush(QBrush{QColor{(int) cur_color.x(), (int) cur_color.y(), (int) cur_color.z()}});
        gscene->addItem(ritem);
        m_itemsMap.insert({ritem, i});

        auto titem = new QGraphicsTextItem();
        titem->setPos(x, y);
        titem->setHtml(QString().asprintf(R"(
            <p style="font-size:16px; color:#fff;">Repo: <b><i>test3</i></b></p>
            <p style="font-size:14px; color:#fff;">Ver: <i>0.%d.%d</i></p>)", (i + 1) / 4, (i + 1) % 4));
        gscene->addItem(titem);
    }

    connect(gscene, &QGraphicsScene::focusItemChanged, this, [=](QGraphicsItem *newFocus, QGraphicsItem *oldFocus, Qt::FocusReason reason){
        if (newFocus)
        {
            qDebug("focus on item[ %d ]", m_itemsMap[newFocus]);
        }
    });
}

//VersionRect::VersionRect(const QRectF &rect, QGraphicsItem *parent)
//    : QGraphicsRectItem{rect, parent}
//{}

//VersionRect::VersionRect(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
//    : QGraphicsRectItem{x, y, w, h, parent}
//{}

//VersionRect::~VersionRect() {}

//void VersionRect::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
//{
//    qDebug("hover");
//}


