/*
 * parameterdommodel.cpp
 *
 *  Created on: Apr 26, 2011
 *      Author: aboucher
 */

#include "parameterdommodel.h"


 ParameterDomModel::ParameterDomModel(QDomDocument document, QObject *parent)
     : QAbstractItemModel(parent), domDocument(document)
 {
     rootItem = new DomAttributeItem(domDocument, 0);
 }

 ParameterDomModel::~ParameterDomModel()
 {
     delete rootItem;
 }


 int ParameterDomModel::columnCount(const QModelIndex &/*parent*/) const
 {
     return 2;
 }

 Qt::ItemFlags ParameterDomModel::flags(const QModelIndex &index) const
 {
     if (!index.isValid())
         return 0;

     return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
 }

 QVariant ParameterDomModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
 {
     if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
         switch (section) {
             case 0:
                 return tr("Parameter");
             case 1:
                 return tr("Value");
             default:
                 return QVariant();
         }
     }

     return QVariant();
 }



 QModelIndex ParameterDomModel::index(int row, int column, const QModelIndex &parent)
             const
 {
     if (!hasIndex(row, column, parent))
         return QModelIndex();

     DomAttributeItem *parentItem;

     if (!parent.isValid())
         parentItem = rootItem;
     else
         parentItem = static_cast<DomAttributeItem*>(parent.internalPointer());

     DomAttributeItem *childItem = parentItem->child(row);
     if (childItem)
         return createIndex(row, column, childItem);
     else
         return QModelIndex();
 }


 int ParameterDomModel::rowCount(const QModelIndex &parent) const
 {
     if (parent.column() > 0)
         return 0;

     DomAttributeItem *parentItem;

     if (!parent.isValid())
         parentItem = rootItem;
     else
         parentItem = static_cast<DomAttributeItem*>(parent.internalPointer());

     return parentItem->childCount();
 }


 QModelIndex ParameterDomModel::parent(const QModelIndex &child) const
 {
     if (!child.isValid())
         return QModelIndex();

     DomAttributeItem *childItem = static_cast<DomAttributeItem*>(child.internalPointer());
     DomAttributeItem *parentItem = childItem->parent();

     if (!parentItem || parentItem == rootItem)
         return QModelIndex();

     return createIndex(parentItem->row(), 0, parentItem);
 }


 QVariant ParameterDomModel::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
         return QVariant();

     if (role != Qt::DisplayRole)
         return QVariant();

     DomAttributeItem *item = static_cast<DomAttributeItem*>(index.internalPointer());

     QDomNode node = item->node();
     QDomNamedNodeMap attributeMap = node.attributes();

     switch (index.column()) {
         case 0:
             return node.nodeName();
         case 1:
        	  return node.nodeValue();

         default:
             return QVariant();
     }
 }

