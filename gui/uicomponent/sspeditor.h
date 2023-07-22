#ifndef MX_SSPEDITOR_H
#define MX_SSPEDITOR_H

#include <QWidget>

namespace Ui {
class SSPEditor;
}

class PreviewWidget;

class SSPEditor : public QWidget
{
    Q_OBJECT

public:
    explicit SSPEditor(PreviewWidget *prewviewer, QWidget *parent = nullptr);
    ~SSPEditor();

    void fresh();

private:
    Ui::SSPEditor *ui;
    PreviewWidget *m_prewviewer;
};

#endif // SSPEDITOR_H
