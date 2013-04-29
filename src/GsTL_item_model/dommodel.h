/*
 * DomModel.h
 *
 *  Created on: Apr 26, 2011
 *      Author: aboucher
 */

#ifndef DOMMODEL_H_
#define DOMMODEL_H_

#include "domitem.h"
#include "common.h"

#include <QtCore/QAbstractItemModel>
#include <QtXml/QtXml>


 class ITEM_MODEL_DECL DomModel : public QAbstractItemModel
 {
     Q_OBJECT

 public:
     DomModel(QDomDocument document, QObject *parent = 0);
     virtual ~DomModel();

     virtual QVariant data(const QModelIndex &index, int role) const;
     virtual Qt::ItemFlags flags(const QModelIndex &index) const;
     virtual QVariant headerData(int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const;
     virtual QModelIndex index(int row, int column,
                       const QModelIndex &parent = QModelIndex()) const;
     virtual QModelIndex parent(const QModelIndex &child) const;
     virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
     virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

 protected:
     QDomDocument domDocument;
     DomItem *rootItem;
 };

#endif /* DOMMODEL_H_ */
