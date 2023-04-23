#include "importwindow.h"

#include "mxzygl.h"
#include <QGridLayout>
#include <QPushButton>
#include <QFileDialog>

ImportWindow::ImportWindow(const QString &title, ImportType type, QWidget *parent)
    : IFunctionWindow(title, parent->size(), false, parent), m_im_type(type)
{
    auto ly = new QGridLayout(this);
    this->setLayout(ly);

    auto bt_fileBrowse = new QPushButton(tr("浏览"), this);
    connect(bt_fileBrowse, &QPushButton::clicked, this, [&type,this]() {
        auto fileDialog = new QFileDialog(this);
        ConfigManager::getInstance();
        switch (type) {
        case ImportWindow::ImportType::IM_MODEL:
            fileDialog->setWindowTitle(tr("浏览模型文件"));
            fileDialog->setNameFilter("*.obj;*.fbx");
            break;
        case ImportWindow::ImportType::IM_BVH:
            fileDialog->setWindowTitle(tr("浏览骨骼动画文件"));
            fileDialog->setNameFilter("*.bvh");
            break;
        case ImportWindow::ImportType::IM_EFFECT:
            fileDialog->setWindowTitle(tr("浏览特效文件"));
            fileDialog->setNameFilter("*.obj;*.fbx;*.bvh");
            break;
        }
        fileDialog->setNameFilter("*.obj;*.fbx");
    });
}
