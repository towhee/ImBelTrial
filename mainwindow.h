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

public slots:
    void openFile();

private:
    void InitStatusBar();
    void InitModel();
    void InitTreeViewTemplate();
    void InitComboBoxTemplates();
    void InitImage();
    void showModelInTree();
    DomModel* model;
    QString xmlPath;
    QTreeView* view;
};

#endif // MAINWINDOW_H
