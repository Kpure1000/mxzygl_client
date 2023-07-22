#ifndef MX_VERSIONWIDGET_H
#define MX_VERSIONWIDGET_H

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

    void refresh();

    void refreshRepoInfo();
    void clearRepoInfo();

    void refreshVersionInfo();
    void clearVersionInfo();

private:
    Ui::VersionWidget *ui;
    VersionController * m_vctrl;
    int m_selected_repo_index{-1}, m_selected_version_index{-1};
    QString last_selected_repo{};

    // QWidget interface
protected:
    void showEvent(QShowEvent *event) override;
};

#endif // MX_VERSIONWIDGET_H
