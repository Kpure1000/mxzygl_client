#ifndef MX_MODELSEARCHWIDGET_H
#define MX_MODELSEARCHWIDGET_H

#include <QWidget>

#include "function/modelsearch.h"

namespace Ui {
class ModelSearchWidget;
}

class PreviewWidget;
class Downloader;

class ModelSearchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ModelSearchWidget(ModelSearch::SearchType searchType, QWidget *parent = nullptr);
    ~ModelSearchWidget();

    ModelSearch *getSearcher() const { return m_modelSearch; }

    Downloader *getDownloader() const;

private slots:
    void on_le_input_textChanged(const QString &input);

    void on_bt_browser_clicked();

    void on_bt_search_clicked();

    void on_bt_export_clicked();

private:
    Ui::ModelSearchWidget *ui;

    Downloader *m_downloader;
    ModelSearch *m_modelSearch;

    PreviewWidget *m_single_preview, *m_preview;

    QString m_selectedFile;
};

#endif // MX_MODELSEARCHWIDGET_H
