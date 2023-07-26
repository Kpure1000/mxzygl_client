#include "versionwidget.h"
#include "ui_versionwidget.h"

#include <QMessageBox>
#include <QJsonArray>
#include <QDebug>

#include "function/versioncontroller.h"
#include "inputdailog.h"
#include "infotablewidget.h"

VersionWidget::VersionWidget(VersionController * vctrl, QWidget *parent) :
    QWidget(parent), ui(new Ui::VersionWidget), m_vctrl{vctrl}
{
    ui->setupUi(this);

    ui->bt_create       ->setEnabled(false);
    ui->bt_add          ->setEnabled(false);
    ui->bt_delet        ->setEnabled(false);
    ui->bt_query        ->setEnabled(false);
    ui->bt_sync         ->setEnabled(false);
    ui->bt_rollback     ->setEnabled(false);
    ui->gb_repoSel      ->setEnabled(false);
    ui->gb_versionInfo  ->setEnabled(false);

    connect(ui->w_painter, &VersionPainter::onVersionSelected, this, [=](int index) {
        m_selected_version_index = index;
        refreshVersionInfo();
    });
    connect(ui->w_painter, &VersionPainter::onVersionSelectionCleared, this, [=]() {
        m_selected_version_index = -1;
        clearVersionInfo();
    });

    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
        qDebug() << "QComboBox::currentIndexChanged>> " << index;
        m_selected_repo_index = index;
        if (m_selected_repo_index >= 0) {
            this->refreshRepoInfo();
            this->ui->w_painter->clearVersion();
            this->ui->w_painter->refreshVersion(m_vctrl->getVersions(m_selected_repo_index));
        }
    });

    connect(ui->bt_pull, &QPushButton::clicked, this, [=](){
        m_vctrl->pullRepo();
        this->refresh();
    });
    connect(ui->bt_create, &QPushButton::clicked, this, [=]() {
        auto inputDia = new InputDailog(tr("构建版本库"), tr("输入版本库名称"), [=](const QString &input, QString &info) {
            if (m_vctrl->hasRepo(input)) {
                info = tr("同名版本库已存在");
                return false;
            } else if (input.isEmpty()) {
                info = tr("版本库名不能为空");
                return false;
            } else if (std::find(input.begin(), input.end(), '"') != input.end()) {
                info = tr("不能包含字符 \" ");
                return false;
            }
            return true;
        });
        connect(inputDia, &InputDailog::onAccecpted, this, [=](const QString& input) {
            m_vctrl->createRepo(input);
        });
        inputDia->show();
    });
    connect(ui->bt_add, &QPushButton::clicked, this, [=](){
        auto inputDia = new InputDailog(tr("新增版本"), tr("输入版本名称"), [=](const QString &input, QString &info) {
            if (input.isEmpty()) {
                info = tr("版本名不能为空");
                return false;
            } else if (std::find(input.begin(), input.end(), '"') != input.end()) {
                info = tr("不能包含字符 \" ");
                return false;
            }
            return true;
        });
        connect(inputDia, &InputDailog::onAccecpted, this, [=](const QString& input) {
            m_vctrl->addVersion(m_vctrl->getRepo(m_selected_repo_index)["id"].toString(), input);
        });
        inputDia->show();
    });
    connect(ui->bt_sync, &QPushButton::clicked, this, [=]() {
        if (QMessageBox::question(this, tr("确认同步"), tr("同步将导致当前工作区数据库被选中版本替代, 是否确认同步?"),
                                  QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No,
                                  QMessageBox::StandardButton::Yes)
            == QMessageBox::StandardButton::Yes) {
            m_vctrl->syncVersion(m_selected_repo_index, m_selected_version_index);
        }
    });
    connect(ui->bt_query, &QPushButton::clicked, this, [=](){
        m_vctrl->queryVersion();
    });
    connect(ui->bt_delet, &QPushButton::clicked, this, [=](){
        if (QMessageBox::question(this, tr("确认删除"), tr("版本删除无法恢复, 是否确认删除?"),
                                  QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No,
                                  QMessageBox::StandardButton::Yes)
            == QMessageBox::StandardButton::Yes) {
            m_vctrl->deleteVersion(m_selected_repo_index, m_selected_version_index);
        }
    });
    connect(ui->bt_rollback, &QPushButton::clicked, this, [=](){
        if (QMessageBox::question(this, tr("确认回退"), tr("版本回退将丢失所有在此之后的版本且无法恢复, 是否确认回退?"),
                                  QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No,
                                  QMessageBox::StandardButton::Yes)
            == QMessageBox::StandardButton::Yes) {
            m_vctrl->rollbackVersion(m_selected_repo_index, m_selected_version_index);
        }
    });

    connect(m_vctrl, &VersionController::onPullReopSuccessful, this, [=]() {
        ui->w_painter->clearVersion();
        this->refresh();
        QMessageBox::information(this, tr("版本库拉取"), tr("版本库拉取成功!"));
    });

    connect(m_vctrl, &VersionController::onCreateReopSuccessful, this, [=](const QJsonObject &repo_info) {
        QMessageBox::information(this, tr("版本库创建"), tr("版本库创建成功!"));
        // 自动切换至新建的版本库
        m_vctrl->addRepoTmp(repo_info); // 十分糟糕的实现,不过work就行
        ui->comboBox->addItem(repo_info["name"].toString());
        ui->comboBox->setCurrentIndex(ui->comboBox->count() - 1);
        // 自动拉取
        emit m_vctrl->onResponsing(tr("自动拉取版本库以刷新"), true);
        ui->bt_pull->click();
    });

    connect(m_vctrl, &VersionController::onAddVersionSuccessful, this, [=]() {
        QMessageBox::information(this, tr("版本增加"), tr("版本增加成功!"));
        // 自动拉取
        emit m_vctrl->onResponsing(tr("自动拉取版本库以刷新"), true);
        ui->bt_pull->click();
    });

    connect(m_vctrl, &VersionController::onSyncVersionSuccessful, this, [=]() {
        QMessageBox::information(this, tr("版本同步"), tr("版本同步成功!"));
        // 自动拉取
        emit m_vctrl->onResponsing(tr("自动拉取版本库以刷新"), true);
        ui->bt_pull->click();
    });

    connect(m_vctrl, &VersionController::onQueryVersionSuccessful, this, [=](const QJsonObject &repo_info) {
        // 弹窗显示当前工作区的版本分布
        auto _info_ref = new QJsonObject(repo_info);
        auto _infoDialog = new QDialog(this);
        _infoDialog->setWindowFlags(Qt::Dialog
                             | Qt::WindowMinMaxButtonsHint
                             | Qt::WindowCloseButtonHint);
        _infoDialog->resize(this->size() * 0.8);
        auto _ly_total = new QVBoxLayout(_infoDialog);
        auto _label = new QLabel(tr("双击<span style='color:#d6c540;'><b>预览组序号</b></span>可跳转至相应版本库及版本界面"), _infoDialog);
        _ly_total->addWidget(_label, 0);
        auto _infotable = new InfoTableWidget(_info_ref, 1, false, true, _infoDialog);
        _ly_total->addWidget(_infotable, 1);
        _infoDialog->setWindowTitle("版本查询: 当前工作区的版本分布");
        connect(_infoDialog, &QDialog::destroyed, this, [=](QObject *) {
            delete _info_ref;
        });
        connect(_infotable, &InfoTableWidget::onGroupSelected, this, [=](const std::vector<int> &rows) {
            if (rows.size() == 1) {
                int row = rows[0];
                auto data = (*_info_ref)["data"].toArray();
                const auto & rowObj = data[row].toObject();
                auto repo_name = rowObj["repo_name"].toString();
                auto version_hash = rowObj["hash"].toString();
                auto repo_index = m_vctrl->getRepoIndexFromName(repo_name);
                auto version_index = m_vctrl->getVersionIndexFromHash(repo_index, version_hash);

                ui->comboBox->setCurrentIndex(repo_index);
                ui->w_painter->setCurrentVersion(version_index);
            }
        });
        _infotable->refresh();
        _infoDialog->show();
        QMessageBox::information(this, tr("版本查询"), tr("版本查询成功!"));
    });

    connect(m_vctrl, &VersionController::onDeleteVersionSuccessful, this, [=]() {
        QMessageBox::information(this, tr("版本删除"), tr("版本删除成功!"));
        // 自动拉取
        emit m_vctrl->onResponsing(tr("自动拉取版本库以刷新"), true);
        ui->bt_pull->click();
    });

    connect(m_vctrl, &VersionController::onRollbackVersionSuccessful, this, [=]() {
        QMessageBox::information(this, tr("版本回退"), tr("版本回退成功!"));
        // 自动拉取
        emit m_vctrl->onResponsing(tr("自动拉取版本库以刷新"), true);
        ui->bt_pull->click();
    });
}

VersionWidget::~VersionWidget()
{
    delete ui;
}

void VersionWidget::refresh()
{
    clearRepoInfo();
    clearVersionInfo();

    ui->bt_create       ->setEnabled(true);
    ui->bt_add          ->setEnabled(true);
    ui->bt_query        ->setEnabled(true);

    ui->gb_repoSel      ->setEnabled(true);
    ui->gb_versionInfo  ->setEnabled(true);

    const auto &data_repo = *m_vctrl->getRepoInfo();

    ui->comboBox->clear();
    // 找上次选择的repo index
    int _last_selected_repo_index = -1;
    for (const auto &d : data_repo) {
        ui->comboBox->addItem(d["name"].toString());
        if (d["id"].toString() == last_selected_repo) {
            _last_selected_repo_index = ui->comboBox->count() - 1;
        }
    }
    // 恢复上次选择的repo
    if (!data_repo.isEmpty()) {
        ui->comboBox->setCurrentIndex(_last_selected_repo_index > -1 ? _last_selected_repo_index : 0);
    }
}

void VersionWidget::refreshRepoInfo()
{
    const auto &repoObj = m_vctrl->getRepo(m_selected_repo_index);
    if (!repoObj.isEmpty()) {
        ui->lb_repoID   ->setText(repoObj[  "id"          ].toString());
        ui->lb_repoHEAD ->setText(repoObj[  "cur_version" ].toString());
        ui->lb_repoTime ->setText(repoObj[  "create_time" ].toString());
    }
}

void VersionWidget::clearRepoInfo()
{
    if (m_selected_repo_index > -1) {
        const auto &repoObj = m_vctrl->getRepo(m_selected_repo_index);
        last_selected_repo = repoObj["id"].toString();
    }
    m_selected_repo_index = -1;
    ui->lb_repoID   ->setText("-");
    ui->lb_repoHEAD ->setText("-");
    ui->lb_repoTime ->setText("-");
}

void VersionWidget::refreshVersionInfo()
{
    ui->bt_delet   ->setEnabled(true);
    ui->bt_sync    ->setEnabled(true);
    ui->bt_rollback->setEnabled(true);
    const auto &versionObj = m_vctrl->getVersion(m_selected_repo_index, m_selected_version_index);
    if (!versionObj.isEmpty()) {
        ui->lb_versionName->setText(versionObj[ "name"  ].toString());
        ui->lb_versionHash->setText(versionObj[ "hash"  ].toString());
        ui->lb_versionRepo->setText(versionObj[ "repo"  ].toString());
        ui->lb_versionTime->setText(versionObj[ "time"  ].toString());
    }
}

void VersionWidget::clearVersionInfo()
{
    ui->bt_delet   ->setEnabled(false);
    ui->bt_sync    ->setEnabled(false);
    ui->bt_rollback->setEnabled(false);
    m_selected_version_index = -1;
    ui->lb_versionName->setText("-");
    ui->lb_versionHash->setText("-");
    ui->lb_versionRepo->setText("-");
    ui->lb_versionTime->setText("-");
}


void VersionWidget::showEvent(QShowEvent *event)
{
    m_vctrl->pullRepo();
}
