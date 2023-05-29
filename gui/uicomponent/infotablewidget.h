#ifndef MX_INFOTABLEWIDGET_H
#define MX_INFOTABLEWIDGET_H

#include <QTableWidget>
#include <vector>
#include <QJsonObject>
#include <QColor>

class InfoTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit InfoTableWidget(QJsonObject *info, int spanNum, bool info_editable, bool resizeAsContent, QWidget *parent = nullptr);
    
    void refresh();

    void jumpTo(int row);

    void selectGroup(int group);
    
    void clearInfos();

    void resetInfo(QJsonObject *info);

signals:
    void onGroupSelected(const std::vector<int> &rows);

public slots:

private:
    int m_spanNum;
    QJsonObject *m_info;
    bool m_info_editable, m_resizeAsContent;
    static const QColor m_editable_col, m_uneditable_col, m_group_col;
};

#endif // MX_INFOTABLEWIDGET_H
