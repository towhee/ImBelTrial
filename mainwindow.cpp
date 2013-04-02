#include "dommodel.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QLabel"
#include "QTreeView"
#include <QDomDocument>
#include <QFileDialog>
#include <QDate>
#include <QDebug>
#include <QTableWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), model(0)
{
    setupUi(this);
    this->setWindowTitle(tr("ImBel"));
    InitImage();
    InitStatusBar();
    InitComboBoxTemplates();
    InitModel();
    InitTreeViewTemplate();
}

void MainWindow::InitModel()
{
    QString filePath = "D:/Data/My Projects/Qt/Examples/ImBelTrial/ImBel.xml";
    if (!filePath.isEmpty()) {
      QFile file(filePath);
      if (file.open(QIODevice::ReadOnly)) {
          QDomDocument document;
          if (document.setContent(&file)) {
              model = new DomModel(document, this);
              xmlPath = filePath;
          }
          file.close();
      }
  }
}

void MainWindow::InitTreeViewTemplate()
{
    // filter the model as we only want to see the template objects in the
    // template tree: template > image, borders, shapes, graphics etc

    //QSortFilterProxyModel *treeModel = new QSortFilterProxyModel(this);
    //treeModel->setFilterKeyColumn(1);
    //treeModel->setFilterFixedString("Zenfolio");
    //QRegExp rx("[A-Z]");
    //treeModel->setFilterRegExp(rx);
    //treeModel->setSourceModel(model);

    treeViewTemplate->setModel(model);
    treeViewTemplate->header()->hide();
    treeViewTemplate->resizeColumnToContents(0);
    treeViewTemplate->setColumnHidden(1, true);
    treeViewTemplate->setColumnHidden(2, true);

//    QModelIndexList idxList = model->match(treeModel->index(0, 0), Qt::DisplayRole,
//                                "Template1", 2, Qt::MatchRecursive);
    //treeViewTemplate->setRootIndex(idxList[0]);
}

void MainWindow::openFile()
{
    showModelInTree();
//    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"),
//        xmlPath, tr("XML files (*.xml);;HTML files (*.html);;"
//                    "SVG files (*.svg);;User Interface files (*.ui)"));

//      QString filePath = "D:/Data/My Projects/Qt/Examples/ImBelTrial/Status.xml";
//      if (!filePath.isEmpty()) {
//        QFile file(filePath);
//        if (file.open(QIODevice::ReadOnly)) {

//            QDomDocument document;
//            if (document.setContent(&file)) {
//                DomModel *newModel = new DomModel(document, this);
//                treeViewTemplate->setModel(newModel);
//                delete model;
//                model = newModel;
//                xmlPath = filePath;
//            }
//            file.close();
//        }
//    }
}

void MainWindow::InitStatusBar()
{
    Ui_MainWindow::statusBar->showMessage(tr(""));

    // Add a label to the statusbar - this works or use statusBar->showMessage
//    QLabel* statusMsg;
//    statusMsg = new QLabel(Ui_MainWindow::statusBar);
//    statusMsg->setText("This is a test");

    // Add a progressbar to the statusbar
    pb = new QProgressBar(Ui_MainWindow::statusBar);
    pb->setRange(0, 100);
    pb->setValue(25);
    pb->hide();
    Ui_MainWindow::statusBar->addPermanentWidget(pb);
}

void MainWindow::InitImage()
{
    QGraphicsScene *scene = new QGraphicsScene;
    QPixmap pixmap("D:/Data/My Projects/Qt/Examples/ImBelTrial/hummer.jpg");
    scene->addPixmap(pixmap);
    graphicsView->setScene(scene);
    graphicsView->show();
}

void MainWindow::InitComboBoxTemplates()
{
    // populate templates combobox with some fake names for now
    QStringList txt;
    txt << "Zenfolio 1024" << "Zenfolio homepage" << "800px with no border" <<
           "Classic frame 1024" << "24x36 inches";
    for ( QStringList::Iterator it = txt.begin(); it != txt.end(); ++it )
        comboBoxTemplate->addItem(*it);
}

void MainWindow::showModelInTree()
{
    QTreeView *tree = new QTreeView;
    tree->setModel(model);
    tree->expandAll();
    tree->resizeColumnToContents(0);
    tree->resizeColumnToContents(1);
    tree->resizeColumnToContents(2);
    tree->setAlternatingRowColors(true);
    tree->setMinimumSize(600,800);
    tree->show();
}
