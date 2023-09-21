#ifndef MX_OPTIONWINDOW_H
#define MX_OPTIONWINDOW_H

#include "ifunctionwindow.h"

class OptionWindow : public IFunctionWindow
{
    Q_OBJECT
public:
    explicit OptionWindow(QWidget *parent = nullptr, bool is_logined = true);

signals:

};

#endif // MX_OPTIONWINDOW_H
