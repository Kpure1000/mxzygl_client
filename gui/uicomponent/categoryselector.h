#ifndef MX_CATEGORYSELECTOR_H
#define MX_CATEGORYSELECTOR_H

#include <QWidget>

namespace Ui {
class CategorySelector;
}

class CategorySelector : public QWidget
{
    Q_OBJECT

public:
    explicit CategorySelector(QWidget *parent = nullptr);
    ~CategorySelector();

private:
    Ui::CategorySelector *ui;
};

#endif // MX_CATEGORYSELECTOR_H
