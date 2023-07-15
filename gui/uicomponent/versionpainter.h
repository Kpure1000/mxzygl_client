#ifndef VERSIONPAINTER_H
#define VERSIONPAINTER_H

#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsSceneHoverEvent>

#include <unordered_map>

class QGraphicsSceneHoverEvent;

class VersionPainter : public QWidget
{
    Q_OBJECT
public:
    explicit VersionPainter(QWidget *parent = nullptr);

signals:

private:
    std::unordered_map<QGraphicsItem*, int> m_itemsMap;

};

class VersionRect : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit VersionRect(const QRectF &rect, qreal radius, QGraphicsItem *parent = nullptr, QObject *objparent = nullptr);
    explicit VersionRect(qreal x, qreal y, qreal w, qreal h, qreal radius, QGraphicsItem *parent = nullptr, QObject *objparent = nullptr);
    ~VersionRect();

    void setBrushColor(const QColor& color);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

//    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

    void setVersionSelected(bool selected);

signals:
    void onHover();

private:
    qreal m_radius;
    QRectF m_rect;
    QColor m_brushColor;
    bool m_is_selected;

    // QGraphicsItem interface
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

};

#endif // VERSIONPAINTER_H
