#ifndef VERSIONWIDGET_H
#define VERSIONWIDGET_H

#include <QWidget>

namespace Ui {
class VersionWidget;
}

class VersionController;

class VersionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VersionWidget(VersionController * vctrl, QWidget *parent = nullptr);
    ~VersionWidget();

private:
    Ui::VersionWidget *ui;
    VersionController * m_vctrl;
};

#endif // VERSIONWIDGET_H
