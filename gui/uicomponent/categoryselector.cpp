#include "categoryselector.h"
#include "ui_categoryselector.h"

#include <QButtonGroup>
#include <QDebug>

#include "function/metacategory.h"

CategorySelector::CategorySelector(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::CategorySelector)
{
    ui->setupUi(this);
    auto bt_group = new QButtonGroup(this);
    bt_group->addButton(ui->rbt_1, static_cast<int>(MetaCategory::Category::MC_OUTFIT));
    bt_group->addButton(ui->rbt_2, static_cast<int>(MetaCategory::Category::MC_STAFF));
    bt_group->addButton(ui->rbt_3, static_cast<int>(MetaCategory::Category::MC_GROUND));
    bt_group->addButton(ui->rbt_4, static_cast<int>(MetaCategory::Category::MC_BUILDING));

    bt_group->button(MetaCategory::getInstance()->getCategoryInt())->setChecked(true);
    auto checked_bt = bt_group->checkedButton();
    auto checked_font = checked_bt->font();
    checked_font.setBold(true);
    checked_bt->setFont(checked_font);

    connect(bt_group, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this, [=](QAbstractButton * bt){
        MetaCategory::getInstance()->setCategory(static_cast<MetaCategory::Category>(bt_group->id(bt)));
    });

    connect(MetaCategory::getInstance(), &MetaCategory::onCategoryModyfied, this, [=](MetaCategory::Category pre, MetaCategory::Category cur){
        auto pre_bt = bt_group->button(static_cast<int>(pre));
        auto pre_font = pre_bt->font();
        pre_font.setBold(false);
        pre_bt->setFont(pre_font);

        auto cur_bt = bt_group->button(static_cast<int>(cur));
        cur_bt->setChecked(true);
        auto cur_font = cur_bt->font();
        cur_font.setBold(true);
        cur_bt->setFont(cur_font);
    });
}

CategorySelector::~CategorySelector()
{
    delete ui;
}
