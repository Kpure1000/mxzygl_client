#ifndef MX_INFOTABLEWIDGET_H
#define MX_INFOTABLEWIDGET_H

#include <QTableWidget>
#include <vector>
#include <QJsonObject>

class InfoTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    InfoTableWidget(QJsonObject *info, int spanNum = 6, QWidget *parent = nullptr);
    
    void refresh();

    void jumpTo(int row);
    
    void clearInfos();

signals:
    void onSelectGroupToPreview(const std::vector<QTableWidgetItem *> &items);

public slots:
    void doPreviewPaneSelected();

private:
    int m_spanNum;
    QJsonObject *m_info;
};

#endif // MX_INFOTABLEWIDGET_H
