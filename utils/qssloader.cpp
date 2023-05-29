#include "qssloader.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

QssLoader::QssLoader()
{

}

QString QssLoader::loadStyleSheet(const QString &fileName)
{
    auto fileIn = QFile(fileName);
    if (fileIn.open(QFile::ReadOnly | QFile::Truncate))
    {
        return QString(fileIn.readAll());
    }
    qDebug() << "QssLoader::loadStyleSheet>> QSS File'" << fileName << "'Open Failed";
    return "";
}
