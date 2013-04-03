#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include "ui_mainwindow.h"
#include "QProgressBar"
#include "QString"
#include <QSortFilterProxyModel>

class DomModel;
QT_BEGIN_NAMESPACE
class QTreeView;
QT_END_NAMESPACE

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    QProgressBar* pb;
    QStringList strTemplateNameList;
    QStringList strTemplateDescriptionList;

public slots:
    void openFile();

private slots:
    void on_comboBoxTemplate_currentIndexChanged(int index);

    void on_comboBoxTemplate_currentTextChanged(const QString &arg1);

private:
    void InitStatusBar();
    void InitModel();
    void InitTreeViewTemplate();
    void UpdateTreeViewTemplate(QString);
    void InitComboBoxTemplates();
    void UpdateComboBoxTemplates();
    void InitImage();
    void showModelInTree();
    void InitTemplatesList();
    DomModel* model;
    QSortFilterProxyModel *treeModel;
    QString xmlPath;
    QTreeView* view;
};

#endif // MAINWINDOW_H
