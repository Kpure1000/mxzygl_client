#include "versionpainter.h"

#include <QGraphicsView>
#include <QHBoxLayout>
#include <QGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QDebug>
#include <QVector3D>
#include <QMenu>
#include <QGraphicsDropShadowEffect>

VersionPainter::VersionPainter(QWidget *parent)
    : QWidget{parent}
{
    auto ly_total = new QHBoxLayout(this);
    auto gscene = new QGraphicsScene(this);
    auto gview = new QGraphicsView(gscene, this);
    gview->setStyleSheet("background-color: #6e6e6e; border: 2px dashed #e2e2e2;");
    ly_total->addWidget(gview);

    int version_count = 5;
    auto start_color = QVector3D{50, 60, 90};
    auto end_color = QVector3D{170, 180, 210};
    auto sub_color = (end_color - start_color) / version_count;

    for (int i = 0; i < version_count; ++i) {
        float x = 50 + i * 250, y = 150;
        QRectF rect{x, y, 150, 100};
        auto ritem = new VersionRect(rect, 5.0);
        ritem->setAcceptHoverEvents(true);
        ritem->setFlags(ritem->flags() | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable);
        auto cur_color = start_color + sub_color * i;
//        ritem->setBrush(QBrush{QColor{(int) cur_color.x(), (int) cur_color.y(), (int) cur_color.z()}});
        ritem->setBrushColor(QColor{(int) cur_color.x(), (int) cur_color.y(), (int) cur_color.z()});
        gscene->addItem(ritem);
        m_itemsMap.insert({ritem, i});
        connect(ritem, &VersionRect::onHover, gview, [=](){
            gview->repaint();
        });

        auto titem = new QGraphicsTextItem();
        titem->setPos(x, y);
        titem->setAcceptHoverEvents(false);
        titem->setHtml(QString().asprintf(R"(
            <p style="font-size:16px; color:#fff;">Repo: <b><i>test3</i></b></p>
            <p style="font-size:14px; color:#fff;">Ver: <i>0.%d.%d</i></p>)", (i + 1) / 4, (i + 1) % 4));
        gscene->addItem(titem);
    }

    connect(gscene, &QGraphicsScene::focusItemChanged, this, [=](QGraphicsItem *newFocus, QGraphicsItem *oldFocus, Qt::FocusReason reason){
        if (newFocus && m_itemsMap.find(newFocus) != m_itemsMap.end())
        {
            auto versionFocus = dynamic_cast<VersionRect*>(newFocus);
            versionFocus->setVersionSelected(true);
//            qDebug("focus on item[ %d ]", m_itemsMap[versionFocus]);
            // TODO: change selected version
        }
        if (oldFocus && m_itemsMap.find(oldFocus) != m_itemsMap.end())
        {
            auto versionOldFocus = dynamic_cast<VersionRect*>(oldFocus);
            versionOldFocus->setVersionSelected(false);
        }
    });
}

VersionRect::VersionRect(const QRectF &rect, qreal radius, QGraphicsItem *parent, QObject* objparent)
    : QObject(objparent), QGraphicsItem{parent}, m_radius{radius}, m_rect{rect} , m_brushColor{1, 1, 1}, m_is_selected{false}
{
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setOffset(5.0);
//    shadow->setColor({100,100,100,30});
    this->setGraphicsEffect(shadow);
}

VersionRect::VersionRect(qreal x, qreal y, qreal w, qreal h, qreal radius, QGraphicsItem *parent, QObject* objparent)
    : QObject(objparent), QGraphicsItem{parent}, m_radius{radius}, m_rect{x, y, w, h} , m_brushColor{1, 1, 1}, m_is_selected{false}
{
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setOffset(5.0);
//    shadow->setColor({100,100,100,30});
    this->setGraphicsEffect(shadow);
}

VersionRect::~VersionRect() {}

void VersionRect::setBrushColor(const QColor &color)
{
    m_brushColor = color;
}

void VersionRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(m_rect, m_radius, m_radius);

    painter->fillPath(path, QBrush{m_brushColor});
}

//void VersionRect::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
//{
//    QMenu menu;
//    QAction *moveAction = menu.addAction("ASD");
//    QAction *selectedAction = menu.exec(event->screenPos());

//    if(selectedAction == moveAction)
//    {
//        setPos(0, 0);
//    }
//}

void VersionRect::setVersionSelected(bool selected)
{
    m_is_selected = selected;
    if (!m_is_selected)
    {
        qreal zoomer = 5.0;
        auto temp_rect = this->graphicsEffect()->boundingRect();
        m_rect = {m_rect.x() + zoomer, m_rect.y() + zoomer, m_rect.width() - zoomer * 2, m_rect.height() - zoomer * 2};
        this->update(temp_rect);
        emit onHover();
    }
}

QRectF VersionRect::boundingRect() const
{
    qreal adjust = 1.0;
    return QRectF{m_rect.x() - adjust,
                  m_rect.y() - adjust,
                  m_rect.width() + adjust * 2,
                  m_rect.height() + adjust * 2};
}


void VersionRect::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (!m_is_selected)
    {
        qreal zoomer = 5.0;
        m_rect = {m_rect.x() - zoomer, m_rect.y() - zoomer, m_rect.width() + zoomer * 2, m_rect.height() + zoomer * 2};
        this->update();
        emit onHover();
    }
}

void VersionRect::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (!m_is_selected)
    {
        qreal zoomer = 5.0;
        auto temp_rect = this->graphicsEffect()->boundingRect();
        m_rect = {m_rect.x() + zoomer, m_rect.y() + zoomer, m_rect.width() - zoomer * 2, m_rect.height() - zoomer * 2};
        this->update(temp_rect);
        emit onHover();
    }
}
