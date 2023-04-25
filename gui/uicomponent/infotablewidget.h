#ifndef INFOTABLEWIDGET_H
#define INFOTABLEWIDGET_H

#include <QTableWidget>
#include <vector>
#include <QJsonObject>

class InfoTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    InfoTableWidget(QWidget *parent = nullptr, int spanNum = 6);
    
    void setInfo(const QJsonObject& info);

    void jumpTo(QTableWidgetItem* item);

signals:
    void onSelectGroupToPreview(const std::vector<QTableWidgetItem *> &items,
                                const QStringList &assetNames);

public slots:
    void doPreviewPaneSelected();

private:
    int m_spanNum;
};

#endif // INFOTABLEWIDGET_H
