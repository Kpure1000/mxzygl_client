#ifndef MX_QSSLOADER_H
#define MX_QSSLOADER_H

#include <QString>

class QssLoader
{
public:
    QssLoader();

    QString loadStyleSheet(const QString &fileName);

    static QssLoader* getInstance()
    {
        static QssLoader instance;
        return &instance;
    }
};

#endif // MX_QSSLOADER_H
