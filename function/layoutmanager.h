#ifndef LAYOUTMANAGER_H
#define LAYOUTMANAGER_H

#include <QMainWindow>

class LayoutManager
{
public:
    void save   (QMainWindow *win,  const QString &title = "");
    void restore(QMainWindow *win,  const QString &title = "");

public:
    static LayoutManager *getInstance()
    {
        static LayoutManager instance;
        return &instance;
    }
};

#endif // LAYOUTMANAGER_H
