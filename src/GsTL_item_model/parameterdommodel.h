/*
 * parameterdommodel.h
 *
 *  Created on: Apr 26, 2011
 *      Author: aboucher
 */

#ifndef PARAMETERDOMMODEL_H_
#define PARAMETERDOMMODEL_H_

#include "common.h"
#include "domattributeitem.h"
#include "common.h"

#include <QAbstractItemModel>
#include <QDomDocument>


 class ITEM_MODEL_DECL ParameterDomModel : public QAbstractItemModel
 {
     Q_OBJECT

 public:
     ParameterDomModel(QDomDocument document, QObject *parent = 0);
     virtual ~ParameterDomModel();

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
     DomAttributeItem *rootItem;
 };

#endif /* PARAMETERDOMMODEL_H_ */
