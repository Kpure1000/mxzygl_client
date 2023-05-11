#include "metacategory.h"

#include <QDebug>
#include <QTextStream>

MetaCategory::MetaCategory(QObject *parent) : QObject(parent), m_category(Category::MC_DEFAULT)
{

}

void MetaCategory::setCategory(Category catgr)
{
    m_category = catgr;
    emit onCategoryModyfied();
}
