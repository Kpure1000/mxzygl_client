#ifndef INTERACTIVEWIDGET_H
#define INTERACTIVEWIDGET_H

#include <QWidget>

namespace Ui {
class InteractiveWidget;
}

class PreviewPane;

class InteractiveWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InteractiveWidget(QWidget *parent = nullptr);
    ~InteractiveWidget();

    PreviewPane *getPreviewPane() const;

signals:
    void onSaveTransform();

private:
    Ui::InteractiveWidget *ui;
};

#endif // INTERACTIVEWIDGET_H
