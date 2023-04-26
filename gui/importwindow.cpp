#include "importwindow.h"

#include "mxzygl.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QFileInfo>
#include <QSpacerItem>

#include "uicomponent/previewwidget.h"

ImportWindow::ImportWindow(const QString &title, ImportType type, QWidget *parent)
    : IFunctionWindow(title, parent ? parent->size() : QSize{800, 600}, false, parent)
    , m_im_type(type)
{
    auto ly_top = new QHBoxLayout();
    ly_top->addSpacerItem(new QSpacerItem(40, 20));
    ly_top->setStretch(0, 4);
    ly_top->setSpacing(2);

    auto bt_fileBrowse = new QPushButton(tr("浏览"), this);
    connect(bt_fileBrowse, &QPushButton::clicked, this, [type, this]() {
        switch (type) {
        case ImportWindow::ImportType::MODEL: {
            auto open_dir
                = ConfigManager::getInstance()->getConfig("FileBrowser/ImportType::MODEL").toString();
            m_selected_files = QFileDialog::getOpenFileNames(this,
                                                                     tr("浏览模型文件"),
                                                                     open_dir,
                                                                     "*.obj;*.fbx");
            if (m_selected_files.size() == 0)
                break;
            ConfigManager::getInstance()->setConfig("FileBrowser/ImportType::MODEL", QFileInfo(m_selected_files[0]).absolutePath());

            for (auto &file : m_selected_files) {
                auto base_name = QFileInfo(file).baseName();
                if (ModelFileManager::getInstance()->has(base_name.toStdString())) {
                    qDebug() << "Model" << base_name << "Exists in"
                             << ModelFileManager::getInstance()->get(base_name.toStdString());
                } else {
                    ModelFileManager::getInstance()->add(base_name.toStdString(), file);
                }
            }
            break;
        }
        case ImportWindow::ImportType::BVH: {
            auto open_dir
                = ConfigManager::getInstance()->getConfig("FileBrowser/ImportType::BVH").toString();
            m_selected_files = QFileDialog::getOpenFileNames(this,
                                                                     tr("浏览骨骼动画文件"),
                                                                     open_dir,
                                                                     "*.bvh");
            if (m_selected_files.size() == 0)
                break;
            ConfigManager::getInstance()->setConfig("FileBrowser/ImportType::BVH", QFileInfo(m_selected_files[0]).absolutePath());
            break;
        }
        case ImportWindow::ImportType::EFFECT: {
            auto open_dir
                = ConfigManager::getInstance()->getConfig("FileBrowser/ImportType::EFFECT").toString();
            m_selected_files = QFileDialog::getOpenFileNames(this,
                                                                     tr("浏览特效文件"),
                                                                     open_dir,
                                                                     "*.obj;*.fbx;*.bvh");
            if (m_selected_files.size() == 0)
                break;
            ConfigManager::getInstance()->setConfig("FileBrowser/ImportType::EFFECT", QFileInfo(m_selected_files[0]).absolutePath());
            break;
        }
        }

        if (m_selected_files.size() == 0) {
            qDebug() << "m_selected_files.size()==0";
            return;
        }

        // 设置要预览的资源文件
        std::vector<res::AssetInfo> assetInfos;
        assetInfos.reserve(m_selected_files.size());
        for (const auto &file : m_selected_files) {
            assetInfos.emplace_back(m_im_type, file);
        }
        m_previewWidget->setPreviewInfo(assetInfos);
    });

    ly_top->addWidget(bt_fileBrowse, 1);

    auto bt_clear = new QPushButton(tr("清空"), this);
    connect(bt_clear, &QPushButton::clicked, this, [type, this](){
        // TODO 清空预览与表格
        m_selected_files.clear();
    });
    ly_top->addWidget(bt_clear, 1);

    auto bt_upload = new QPushButton(tr("上传"), this);
    connect(bt_upload, &QPushButton::clicked, this, [type, this]() {
        // TODO 连接服务器 上传 m_selected_files
    });
    ly_top->addWidget(bt_upload, 1);

    auto ly_total = new QVBoxLayout(this);

    ly_total->addLayout(ly_top, 0);

    m_previewWidget = new PreviewWidget(2, 3, static_cast<PreviewWidget::PreviewType>(type), this);

    ly_total->addWidget(m_previewWidget, 1);

    this->setLayout(ly_total);
}
