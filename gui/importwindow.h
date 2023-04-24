#ifndef MX_UPLOADWINDOW_H
#define MX_UPLOADWINDOW_H

#include "ifunctionwindow.h"
#include <QStringList>

class ImportWindow : public IFunctionWindow
{
    Q_OBJECT

public:
    enum ImportType : int {
        IM_MODEL = 0x1 << 0,
        IM_BVH    = 0x1 << 1,
        IM_EFFECT = 0x1 << 2,
    };

public:
    explicit ImportWindow(const QString &title, ImportType type, QWidget *parent = nullptr);

signals:

private:
    ImportType m_im_type;
    QStringList m_selected_files;

};

#endif // MX_UPLOADWINDOW_H
