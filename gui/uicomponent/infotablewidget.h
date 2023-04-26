#ifndef MX_INFOTABLEWIDGET_H
#define MX_INFOTABLEWIDGET_H

#include <QTableWidget>
#include <vector>
#include <QJsonObject>

class InfoTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    InfoTableWidget(QWidget *parent = nullptr, int spanNum = 6);
    
    void setInfos(const QJsonObject& info);

    void jumpTo(QTableWidgetItem* item);
    
    void clearInfos();

signals:
    void onSelectGroupToPreview(const std::vector<QTableWidgetItem *> &items);

public slots:
    void doPreviewPaneSelected();

private:
    int m_spanNum;
};

#endif // MX_INFOTABLEWIDGET_H
