#ifndef MX_CATEGORYMANAGER_H
#define MX_CATEGORYMANAGER_H

#include <QObject>

class MetaCategory : public QObject
{
    Q_OBJECT
public:

    enum class Category : int {

        MC_OUTFIT      = 0x0100, // 主战装备
        MC_STAFF       = 0x0200, // 人员
        MC_GROUND      = 0x0300, // 地物
        MC_BUILDING    = 0x0400, // 建筑

        MC_DEFAULT     = MC_OUTFIT
    };

    explicit    MetaCategory(QObject *parent = nullptr);

    void        setCategory(Category catgr);

    Category    getCategory() const { return m_category; }

    int         getCategoryInt() const { return static_cast<int>(m_category); }

signals:
    void        onCategoryModyfied(Category pre, Category cur);

public:
    static MetaCategory *getInstance(QObject *parent = nullptr)
    {
        static MetaCategory instance(parent);
        return &instance;
    }

private:

    Q_ENUM(Category)

    Category m_category;

};

#endif // MX_CATEGORYMANAGER_H
