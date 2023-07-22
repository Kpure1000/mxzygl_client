#include "versionpainter.h"

#include <QGraphicsView>
#include <QHBoxLayout>
#include <QGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QDebug>
#include <QVector3D>
#include <QMenu>
#include <QGraphicsDropShadowEffect>
#include <QJsonArray>

VersionPainter::VersionPainter(QWidget *parent)
    : QWidget{parent}
{
    auto ly_total = new QHBoxLayout(this);
    m_gscene = new QGraphicsScene(this);
    m_gview = new QGraphicsView(m_gscene, this);
    m_gview->setStyleSheet("background-color: #5e5e5e; border: 2px dashed #e2e2e2;");
    ly_total->addWidget(m_gview);

    connect(m_gscene, &QGraphicsScene::selectionChanged, this, [=]() {
        bool has_selected = false;
        int selected_index = -1;
        for (auto &item : m_itemsMap) {
            auto versionFocus = dynamic_cast<VersionRect*>(item.first);
            if (versionFocus->isVersionSelected())
                versionFocus->setVersionSelected(false);
        }
        const auto &selected = m_gscene->selectedItems();
        for (int i = selected.size() - 1; i >= 0; i--) {
            if (m_itemsMap.find(selected[i]) != m_itemsMap.end())
            {
                auto versionFocus = dynamic_cast<VersionRect*>(selected[i]);
                if (!versionFocus->isVersionSelected())
                    versionFocus->setVersionSelected(true);
                selected_index = m_itemsMap[versionFocus];
                has_selected = true;
                emit onVersionSelected(selected_index);
                break;
            }
        }
        if (!has_selected)
            emit onVersionSelectionCleared();
    });

}

void VersionPainter::refreshVersion(const QJsonArray &version_data)
{
    m_itemsMap.clear();
    const auto &data = version_data;

    if (data.isEmpty()) {
        qDebug() << "VersionPainter::refreshVersion>> version data is empty";
        return;
    }

    // initial option
    auto start_color = QVector3D{50, 60, 80};
//    auto end_color = QVector3D{170, 180, 200};
    auto end_color = start_color;
    auto sub_color = (end_color - start_color) / data.size();

    qreal start_x = 150, start_y = 100;
    qreal interval_ratio = 0.8;
    qreal init_w = 150, init_h = 100;

    int count = 0;
    for (const auto &d : data) {
        const auto &dObj = d.toObject();
        QString ver_name = dObj["name"].toString();
        QString ver_hash = dObj["hash"].toString();
        QString ver_time = dObj["time"].toString();
        QString ver_repo = dObj["repo"].toString();

        QRectF rect(start_x, start_y, init_w, init_h);

        auto ritem = new VersionRect(rect, 5.0);
        ritem->setAcceptHoverEvents(true);
        ritem->setFlags(ritem->flags() | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable);

        auto cur_color = start_color + sub_color * count;
        ritem->setBrushColor(QColor{(int) cur_color.x(), (int) cur_color.y(), (int) cur_color.z()});
        m_gscene->addItem(ritem);

        m_itemsMap.insert({ritem, count});

        connect(ritem, &VersionRect::onHover, m_gview, [=]() { m_gview->repaint(); });

        auto titem = new QGraphicsTextItem();
        titem->setPos(start_x, start_y);
        titem->setAcceptHoverEvents(false);
        titem->setHtml(versionText(ver_name, ver_hash, ver_time, ver_repo, count == 0));
        m_gscene->addItem(titem);

        ritem->setSize(std::max(init_w, titem->boundingRect().width() + 20.0),
                       std::max(init_h, titem->boundingRect().height() + 10.0));

        start_y += ritem->rect().height() * (1.0 + interval_ratio);
        count++;
    }
}

void VersionPainter::clearVersion()
{
    m_itemsMap.clear(); // 注意!!!一定要先清空map, 因为gscene的clear会导致selectionChanged信号发送,而此时map里面有被释放掉的item
    m_gscene->clear();
    // 既然清空了所有, 那选择项也不存在了
    emit onVersionSelectionCleared();
}

void VersionPainter::setCurrentVersion(int index)
{
    for (auto &rectItem : m_itemsMap) {
        auto versionFocus = dynamic_cast<VersionRect *>(rectItem.first);
        if (index == rectItem.second) {
            rectItem.first->setSelected(true);
        } else {
            rectItem.first->setSelected(false);
        }
    }
}

QString VersionPainter::versionText(const QString &name, const QString &hash, const QString &time, const QString &repo, bool is_head)
{
    QString head = is_head ? "&lt;-HEAD" : " ";
    return QString(R"(
        <p style="font-size:18px; color:#fff;">名称: <b>%1</b></p>
        <p style="font-size:14px; color:#eaeaea;">Hash:
            <b>
                <i> %2</i>
                <span style="font-size:16px; color:#66a334;"> %3</span>
            </b>
        </p>
        <p style="font-size:14px; color:#eaeaea;">创建时间: <i>%4</i></p>
        <p style="font-size:14px; color:#eaeaea;">所属仓库: <i>%5</i></p>
    )").arg(name, hash, head, time, repo);
}

/////////////////////////////
//// VersionRect
/////////////////////////////

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

void VersionRect::setSize(qreal w, qreal h)
{
    m_rect.setWidth(w);
    m_rect.setHeight(h);
    this->update();
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
    if (!m_is_selected && m_is_zoomin)
    {
        qreal zoomer = 5.0;
        auto temp_rect = this->graphicsEffect()->boundingRect();
        m_rect = {m_rect.x() + zoomer, m_rect.y() + zoomer, m_rect.width() - zoomer * 2, m_rect.height() - zoomer * 2};
        this->update(temp_rect);
        emit onHover();
        m_is_zoomin = false;
    } else if (m_is_selected && !m_is_zoomin) {
        qreal zoomer = 5.0;
        m_rect = {m_rect.x() - zoomer, m_rect.y() - zoomer, m_rect.width() + zoomer * 2, m_rect.height() + zoomer * 2};
        this->update();
        emit onHover();
        m_is_zoomin = true;
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
    if (!m_is_selected && !m_is_zoomin)
    {
        qreal zoomer = 5.0;
        m_rect = {m_rect.x() - zoomer, m_rect.y() - zoomer, m_rect.width() + zoomer * 2, m_rect.height() + zoomer * 2};
        this->update();
        emit onHover();
        m_is_zoomin = true;
    }
}

void VersionRect::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (!m_is_selected && m_is_zoomin)
    {
        qreal zoomer = 5.0;
        auto temp_rect = this->graphicsEffect()->boundingRect();
        m_rect = {m_rect.x() + zoomer, m_rect.y() + zoomer, m_rect.width() - zoomer * 2, m_rect.height() - zoomer * 2};
        this->update(temp_rect);
        emit onHover();
        m_is_zoomin = false;
    }
}
