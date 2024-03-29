#include "domitem.h"
#include "dommodel.h"

#include <QtXml>

DomModel::DomModel(QDomDocument document, QObject *parent)
    : QAbstractItemModel(parent), domDocument(document)
{
    rootItem = new DomItem(domDocument, 0);
}

DomModel::~DomModel()
{
    delete rootItem;
}

int DomModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 3;
}

QVariant DomModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    DomItem *item = static_cast<DomItem*>(index.internalPointer());

    QDomNode node = item->node();

    QStringList attributes;
    QDomNamedNodeMap attributeMap = node.attributes();

    switch (index.column()) {
        case 0:
            return node.nodeName();
        case 1:
            for (int i = 0; i < attributeMap.count(); ++i) {
                QDomNode attribute = attributeMap.item(i);
                attributes << attribute.nodeName() + "=\""
                              +attribute.nodeValue() + "\"";
            }
            return attributes.join(' ');
        case 2:
            return node.nodeValue().split("\n").join(' ');
        default:
            return QVariant();
    }
}

Qt::ItemFlags DomModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant DomModel::headerData(int section, Qt::Orientation orientation,
                              int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0:
                return tr("Name");
            case 1:
                return tr("Attributes");
            case 2:
                return tr("Value");
            default:
                return QVariant();
        }
    }

    return QVariant();
}

QModelIndex DomModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    DomItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<DomItem*>(parent.internalPointer());

    DomItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex DomModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    DomItem *childItem = static_cast<DomItem*>(child.internalPointer());
    DomItem *parentItem = childItem->parent();

    if (!parentItem || parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

void DomModel::walkTree(const QModelIndex &node)
{
    // Do model stuff with current index
    // qDebug() << this->data(node, Qt::DisplayRole).toString();
    qDebug() << this->data(index(node.row(),0,node.parent()), Qt::DisplayRole).toString() << " | "
             << this->data(index(node.row(),1,node.parent()), Qt::DisplayRole).toString() << " | "
             << this->data(index(node.row(),2,node.parent()), Qt::DisplayRole).toString() << " | "
             << this->hasChildren(); // << " | " << node.child(0,0).model()->hasChildren();

//                           Qt::DisplayRole).toString() << " | "
//             << parent.row() << " | " << parent.column() << " | " << parent.parent() << " | "
//             << this->data(parent, Qt::DisplayRole).toString();
//             << this->data(index(node.row(),0,node.parent()), Qt::DisplayRole).toString();

    // find out if there are children
    if (this->hasChildren()) {
        // repeat for each child
        for (int i = 0; i < rowCount(node); ++i){
            walkTree(this->index(i,0,node));
        }
    }
}

QModelIndex DomModel::findInModel(QModelIndex &branch, QString searchText, int col)
{
    // is there a match?
    if (searchText == this->data(index(branch.row(),col,branch.parent()), Qt::DisplayRole).toString())
        return branch;
    // find out if there are children
    if (this->hasChildren()) {
        // repeat for each child
        for (int i = 0; i < rowCount(branch); ++i){
            walkTree(this->index(i,0,branch));
        }
    }
    // return failure
}

int DomModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    DomItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<DomItem*>(parent.internalPointer());

    return parentItem->node().childNodes().count();
}
