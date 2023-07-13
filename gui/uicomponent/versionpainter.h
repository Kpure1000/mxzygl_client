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

//class VersionRect : public QGraphicsRectItem
//{
//    Q_OBJECT
//public:
//    explicit VersionRect(const QRectF &rect, QGraphicsItem *parent = nullptr);
//    explicit VersionRect(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = nullptr);
//    ~VersionRect();

//    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
//};

#endif // VERSIONPAINTER_H
