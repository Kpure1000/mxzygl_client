#ifndef MX_INFOTABLEWIDGET_H
#define MX_INFOTABLEWIDGET_H

#include <QTableWidget>
#include <vector>
#include <QJsonObject>

class InfoTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit InfoTableWidget(QJsonObject *info, int spanNum, bool info_editable, QWidget *parent = nullptr);
    
    void refresh();

    void jumpTo(int row);

    void selectGroup(int group);
    
    void clearInfos();

signals:
    void onSelectGroupToPreview(const std::vector<int> &rows);

public slots:

private:
    int m_spanNum;
    QJsonObject *m_info;
    bool m_info_editable;
};

#endif // MX_INFOTABLEWIDGET_H
