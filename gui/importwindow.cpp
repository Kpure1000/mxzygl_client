#include "importwindow.h"

#include "mxzygl.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QFileInfo>

ImportWindow::ImportWindow(const QString &title, ImportType type, QWidget *parent)
    : IFunctionWindow(title, parent->size(), false, parent), m_im_type(type)
{
    auto ly_total = new QGridLayout(this);
    this->setLayout(ly_total);

    auto bt_fileBrowse = new QPushButton(tr("浏览"), this);
    connect(bt_fileBrowse, &QPushButton::clicked, this, [type, this]() {
        switch (type) {
        case ImportWindow::ImportType::IM_MODEL: {
            auto open_dir
                = ConfigManager::getInstance()->getConfig("FileBrowser/IM_MODEL").toString();
            m_selected_files = QFileDialog::getOpenFileNames(this,
                                                                     tr("浏览模型文件"),
                                                                     open_dir,
                                                                     "*.obj;*.fbx");
            if (m_selected_files.size() == 0)
                break;
            ConfigManager::getInstance()->setConfig("FileBrowser/IM_MODEL", QFileInfo(m_selected_files[0]).absolutePath());

            for (auto& file : m_selected_files) {
                auto base_name = QFileInfo(file).baseName();
                auto extend_name = QFileInfo(file).fileName().split('.').back();
                auto mfm = ModelFileManager::getInstance();
                if (mfm->has(base_name.toStdString())) {
                    qDebug() << "Model" << base_name << "Exists in"
                             << mfm->get(base_name.toStdString());
                } else {
                    mfm->add(base_name.toStdString(), file);
                }

//                auto mfm = ModelFileManager::getInstance();
//                auto mm = ModelManager::getInstance();
//                if (mm->has(base_name.toStdString())) {
//                    qDebug() << "Model" << base_name << "exists";
//                } else {
//                    std::shared_ptr<res::Model> model;
//                    if (extend_name == "fbx") {
//                        model = ModelLoader::getInstance()->loadFBX(file, base_name);
//                    } else if (extend_name == "obj") {
//                        model = ModelLoader::getInstance()->loadOBJ(file, base_name);
//                    }
//                    if (model != nullptr) {
//                        qDebug() << "Loaded Model" << model->name;
//                        mm->add(model->name.toStdString(), model);
//                    } else {
//                        qDebug() << "Model" << base_name << "is null";
//                    }
//                }

            }

            break;
        }
        case ImportWindow::ImportType::IM_BVH: {
            auto open_dir
                = ConfigManager::getInstance()->getConfig("FileBrowser/IM_BVH").toString();
            m_selected_files = QFileDialog::getOpenFileNames(this,
                                                                     tr("浏览骨骼动画文件"),
                                                                     open_dir,
                                                                     "*.bvh");
            if (m_selected_files.size() == 0)
                break;
            ConfigManager::getInstance()->setConfig("FileBrowser/IM_BVH", QFileInfo(m_selected_files[0]).absolutePath());
            break;
        }
        case ImportWindow::ImportType::IM_EFFECT: {
            auto open_dir
                = ConfigManager::getInstance()->getConfig("FileBrowser/IM_EFFECT").toString();
            m_selected_files = QFileDialog::getOpenFileNames(this,
                                                                     tr("浏览特效文件"),
                                                                     open_dir,
                                                                     "*.obj;*.fbx;*.bvh");
            if (m_selected_files.size() == 0)
                break;
            ConfigManager::getInstance()->setConfig("FileBrowser/IM_EFFECT", QFileInfo(m_selected_files[0]).absolutePath());
            break;
        }
        }
        // TODO 直接开始预览
        if (m_selected_files.size() == 0) {
            qDebug() << "m_selected_files.size()==0";
            return;
        }

    });

    auto ly_bottom = new QHBoxLayout(this);

    auto bt_clear = new QPushButton(tr("清空"), this);
    connect(bt_clear, &QPushButton::clicked, this, [type, this](){
        // TODO 清空预览与表格
        m_selected_files.clear();
    });

    auto bt_upload = new QPushButton(tr("上传"), this);
    connect(bt_upload, &QPushButton::clicked, this, [type, this]() {
        // TODO 连接服务器 上传 m_selected_files
    });

    ly_total->addLayout(ly_bottom, ly_total->rowCount(), 0);
    ly_bottom->addWidget(bt_fileBrowse);
}
