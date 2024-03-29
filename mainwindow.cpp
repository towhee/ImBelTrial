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
#include <QIcon>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), model(0)
{
    setupUi(this);
    this->setWindowTitle(tr("ImBel"));
    QPixmap pixmap("D:/Data/My Projects/Qt/Examples/ImBelTrial/hummer1.png");
    // resource not working
    //QPixmap pixmap(":/D:/Data/My Projects/Qt/Examples/ImBelTrial/hummer1.png");

    this->setWindowIcon(pixmap);
    InitImage();
    InitStatusBar();
    InitModel();
    InitTemplatesList();
    InitTreeViewTemplate();     // this must precede InitComboBoxTemplates()
    InitComboBoxTemplates();
    InitTreeViewData();

    //model->walkTree(model->index(0,0));
}


void MainWindow::InitModel()
{
    // Read ImBel.xml which has all the template data and program status data.
    // Populate a model that is used throughout the program.
    QString filePath = "D:/Data/My Projects/Qt/Examples/ImBelTrial/ImBel.xml";
    if (!filePath.isEmpty()) {
      QFile file(filePath);
      if (file.open(QIODevice::ReadOnly)) {
          QDomDocument document;
          // This will fail if xml is not well formed.
          if (document.setContent(&file)) {
              model = new DomModel(document, this);
              xmlPath = filePath;
          }
          file.close();
      }
  }
}

void MainWindow::InitComboBoxTemplates()
{
    // Add all the template descriptions to the combobox.  The descriptions
    // are sourced from the tag field (column 1) in the xml file and the model.
    for (int i = 0; i < strTemplateDescriptionList.count(); ++i)
        comboBoxTemplate->addItem(strTemplateDescriptionList[i]);
}

void MainWindow::on_comboBoxTemplate_currentIndexChanged(int index)
{
    // When a new template has been selected in comboBoxTemplate
    // the treeview root index is updated to the same template.
    UpdateTreeViewTemplate(strTemplateNameList[index]);
}

void MainWindow::InitTemplatesList()
{
    // Search for all occurences of "Template" in model of the xml data file.
    QModelIndexList &idxList = model->match(model->index(0, 0), Qt::DisplayRole,
                                "Template", 100, Qt::MatchStartsWith | Qt::MatchRecursive);
    int row = 0;
    // Ignore "Templates".  We just want Template1, Template2...
    if (idxList.size() > 0) {
        // Iterate through the list of model elements found
        for (int i = 0; i < idxList.size(); ++i) {
            // This is column 0 - the node name
            QModelIndex &idx0 = idxList[i];
            row = idx0.row();
            // Column 1 has the tag, which has the descriptive name.  We have to do this
            // because white spaces are not allowed in the node name in xml.
            QModelIndex idx1 = model->index(row, 1, idx0.parent());
            // Take a look at the data in column 1.
            QString text = model->data(idx1, Qt::DisplayRole).toString();
            // If there is no tag then it is the parent node "Templates".
            if (text.length() > 0) {
                // Build a list of the node names.  We will need it to filter the treeviewTemplate
                // each time the comboboxtemplates is updated.
                strTemplateNameList.append(model->data(idx0, Qt::DisplayRole).toString());
                // The tag takes the form "tag = "Zenfolio 1024"" and we need to parse down to
                // the description in quotes.
                int pos = text.indexOf('"', 0);
                // Build the list of template descriptions for comboboxTemplates.
                strTemplateDescriptionList.append(text.mid(pos + 1, text.length() - pos - 2));
            }
        }
    }
}

void MainWindow::InitTreeViewTemplate()
{
    // filter the model as we only want to see the template objects in the
    // template tree: template > image, borders, shapes, graphics etc
    treeModel = new QSortFilterProxyModel(this);
    treeModel->setSourceModel(model);
    treeModel->setFilterKeyColumn(0);
    QRegExp rx("ImBel|Template|Image|Border|Text|Shape|Graphic");
    treeModel->setFilterRegExp(rx);

    treeViewTemplate->setModel(treeModel);
    treeViewTemplate->header()->hide();
    treeViewTemplate->resizeColumnToContents(0);
    treeViewTemplate->setColumnHidden(1, true);
    treeViewTemplate->setColumnHidden(2, true);
}

void MainWindow::InitTreeViewData()
{
    treeViewData->setModel(model);
    treeViewData->expandAll();
    treeViewData->setColumnHidden(1, true);
    treeViewData->resizeColumnToContents(0);
    treeViewData->resizeColumnToContents(2);
}

void MainWindow::UpdateTreeViewTemplate(QString templateName)
{
    QModelIndexList idxList = treeModel->match(treeModel->index(0, 0), Qt::DisplayRole,
                                templateName, 1, Qt::MatchRecursive);
    treeViewTemplate->setRootIndex(idxList[0]);
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

void MainWindow::on_comboBoxTemplate_currentTextChanged(const QString &arg1)
{
    // A template name description has been changed.
    // 1.  Update the description (column 1 in model).
    // 2.  Save the change in ImBel.xml.
}

void MainWindow::on_treeViewTemplate_pressed(const QModelIndex &index)
{
    // When a leaf node in the treeViewTemplate is clicked we want to show all the
    // properties and values in the treeViewData.  The data takes the form
    //
    // Template1
    //     Object (Image, Border, Text, Shape, etc)
    //         ObjectN (Border1, Border2 etc or this level may not exist)
    //             Property
    //                 Value
    //
    // We only want to show properties and values, so we want to pick the next
    // level up as the treeViewData root (Object or ObjectN).  If any other nodes are clicked in
    // treeViewTemplates we will ignore.
    //
    // The tree in treeViewTemplates is
    //
    // Object
    //     ObjectN (Optional)
    //
    // so the test is simple - does the index passed from treeViewTemplates have a child.
    // if it does then we ignore.


//    qDebug() << " ";

//    qDebug() << "treeViewTemplate current: " << treeModel->data(index, Qt::DisplayRole).toString();
//    qDebug() << "treeViewTemplate root:    " << treeModel->data(treeModel->index(0,0), Qt::DisplayRole).toString();
//    qDebug() << "treeViewTemplate parent:  " << treeModel->data(treeModel->parent(index), Qt::DisplayRole).toString();
//    qDebug() << " ";
//    qDebug() << "comboBoxTemplate->currentIndex(): " << comboBoxTemplate->currentIndex();
//    qDebug() << "strTemplateNameList: " << strTemplateNameList[comboBoxTemplate->currentIndex()];

   // treeViewData->header()->hide();
    treeViewData->resizeColumnToContents(0);
    treeViewData->resizeColumnToContents(2);

    if (index.model()->rowCount(index) == 0) {
        QString templateName = strTemplateNameList[comboBoxTemplate->currentIndex()];
        QString nodeName = treeModel->data(index, Qt::DisplayRole).toString();
        QModelIndexList idxList = model->match(model->index(0, 0), Qt::DisplayRole,
                                    templateName, 1, Qt::MatchRecursive);
        QModelIndexList idxList1 = model->match(idxList[0], Qt::DisplayRole,
                                    nodeName, 1, Qt::MatchRecursive);
        treeViewData->setRootIndex((idxList1[0]));
        qDebug() << "root for treeviewData: " << model->data(idxList1[0], Qt::DisplayRole).toString();
    }

//    qDebug() << "Clicked node has child: " << index.model()->rowCount(index);

    // if the rowCount is zero then no children so we are good to go
//    if (index.model()->rowCount(index) == 0) {
//        // starting from the root, get the index for the template
//        QModelIndex idxModel = model->findInModel(model->index(0,0), templateName, 0);
//        qDebug() << templateName << "returned: " << model->data(model->idxModel(0,0), Qt::DisplayRole).toString();
//        //treeViewData->setRootIndex((index));
//    }
    //treeViewData->setRootIndex((index));
}
