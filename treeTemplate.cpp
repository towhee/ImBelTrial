#include "treeTemplate.h"

#include "QLabel"
#include "QTreeWidgetItem"


QTreeWidgetItem *treeItem;

treeTemplate::treeTemplate(QWidget *parent)
{
    treeItem = new QTreeWidgetItem(this);
    treeItem->setText(0, "Template");
    this->insertTopLevelItem(0, treeItem);
}

//Ui_MainWindow::treeWidgetTemplate->insertTopLevelItem(0, treeItem);
//Ui_MainWindow::treeWidgetTemplate->addTopLevelItem(treeItem);
