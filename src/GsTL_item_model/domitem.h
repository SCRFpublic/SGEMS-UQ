/*
 * DomItem.h
 *
 *  Created on: Apr 26, 2011
 *      Author: aboucher
 */

#ifndef DOMITEM_H_
#define DOMITEM_H_

#include "common.h"
#include <QtXml/QtXml>
#include <QtCore/QHash>
#include "common.h"


class ITEM_MODEL_DECL DomItem
 {
 public:
     DomItem(QDomNode &node, int row, DomItem *parent = 0);
     ~DomItem();
     DomItem *child(int i);
     DomItem *parent();
     QDomNode node() const;
     int row();

	QHash<int,DomItem*>::iterator begin(){return childItems.begin();}
	QHash<int,DomItem*>::iterator end(){return childItems.end();}

 private:
     QDomNode domNode;
     QHash<int,DomItem*> childItems;
     DomItem *parentItem;
     int rowNumber;
 };

#endif /* DOMITEM_H_ */
