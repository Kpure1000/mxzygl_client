#ifndef INDEXEDITOR_H
#define INDEXEDITOR_H

#include <QObject>

#include "resource/indexinfo.h"

class IndexEditor : public QObject
{
    Q_OBJECT
public:

    using IndexType = res::IndexInfo::IndexType;

    explicit IndexEditor(IndexType type, QObject *parent = nullptr);

    IndexType getType() { return m_type; }

signals:

private:
    IndexType m_type;

};

#endif // INDEXEDITOR_H
