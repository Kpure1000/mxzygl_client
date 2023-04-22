#include "importwindow.h"

ImportWindow::ImportWindow(const QString &title, ImportType type, QWidget *parent)
    : IFunctionWindow(title, parent->size(), parent), m_im_type(type)
{
}
