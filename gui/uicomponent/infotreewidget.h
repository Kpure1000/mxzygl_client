#ifndef MX_INFOTREEWIDGET_H
#define MX_INFOTREEWIDGET_H

#include <QTreeWidget>
#include <QJsonObject>

class InfoTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit InfoTreeWidget(QJsonObject *root,
                            QJsonObject *detail,
                            bool resizeAsContent,
                            QWidget *parent = nullptr);

    void refreshTree();

    void clearTree();

    void resetTree(QJsonObject *root, QJsonObject *detail);

signals:
    void onRootSelected(int row);

private:
    QJsonObject *m_root, *m_detail;
    bool m_resizeAsContent;
};

#endif // MX_INFOTREEWIDGET_H
