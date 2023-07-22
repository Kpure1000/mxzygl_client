#ifndef MX_VERSIONPAINTER_H
#define MX_VERSIONPAINTER_H

#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QJsonObject>
#include <QJsonArray>
#include <QGraphicsView>
#include <QGraphicsScene>

#include <unordered_map>

class QGraphicsSceneHoverEvent;

class VersionPainter : public QWidget
{
    Q_OBJECT
public:
    explicit VersionPainter(QWidget *parent = nullptr);

//    void setRepoInfo(QJsonObject *info);
//    void setVersionInfo(QJsonObject *info);

    void refreshVersion(const QJsonArray &version_data);
    void clearVersion();

    void setCurrentVersion(int index);

signals:
    void onVersionSelected(int index);
    void onVersionSelectionCleared();

private:
    QString versionText(const QString &name,
                        const QString &hash,
                        const QString &time,
                        const QString &repo,
                        bool is_head = false);

    QGraphicsView *m_gview;
    QGraphicsScene *m_gscene;
    std::unordered_map<QGraphicsItem*, int> m_itemsMap;
//    QJsonObject *m_repo = nullptr, *m_version = nullptr;
};

class VersionRect : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit VersionRect(const QRectF &rect, qreal radius, QGraphicsItem *parent = nullptr, QObject *objparent = nullptr);
    explicit VersionRect(qreal x, qreal y, qreal w, qreal h, qreal radius, QGraphicsItem *parent = nullptr, QObject *objparent = nullptr);
    ~VersionRect();

    void setBrushColor(const QColor& color);
    void setSize(qreal w, qreal h);
    QRectF rect() const { return m_rect; }

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

//    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

    void setVersionSelected(bool selected);
    bool isVersionSelected() const { return m_is_selected; }

signals:
    void onHover();

private:
    qreal m_radius;
    QRectF m_rect;
    QColor m_brushColor;
    bool m_is_selected, m_is_zoomin = false;

    // QGraphicsItem interface
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

};

#endif // VERSIONPAINTER_H
