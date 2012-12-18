/*
 * MdsSpaceObjectModel.h
 *
 *  Created on: May 1, 2011
 *      Author: aboucher
 */

#ifndef MDSSPACEOBJECTMODEL_H_
#define MDSSPACEOBJECTMODEL_H_

#include "common.h"

#include <Qt>
#include <QModelIndex>
#include <QVariant>
#include <QStandardItemModel>


class ITEM_MODEL_DECL MdsSpaceObjectModel : public QStandardItemModel
 {
     Q_OBJECT

public:
	MdsSpaceObjectModel(QObject* parent = 0);
	virtual ~MdsSpaceObjectModel();
};

#endif /* MDSSPACEOBJECTMODEL_H_ */
