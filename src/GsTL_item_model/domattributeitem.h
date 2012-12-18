/*
 * domattributeitem.h
 *
 *  Created on: Apr 26, 2011
 *      Author: aboucher
 */

#ifndef DOMATTRIBUTEITEM_H_
#define DOMATTRIBUTEITEM_H_

#include "common.h"
#include <QDomNode>
#include <QHash>


class ITEM_MODEL_DECL DomAttributeItem {
public:
	DomAttributeItem(QDomNode &node, int row, DomAttributeItem *parent = 0);
    ~DomAttributeItem();
    DomAttributeItem *child(int i);
    DomAttributeItem *parent();
    QDomNode node() const;
    int row();
    int childCount();

private:
    QDomNode domNode;
    QHash<int,DomAttributeItem*> childItems;
    DomAttributeItem *parentItem;
    int rowNumber;
};

#endif /* DOMATTRIBUTEITEM_H_ */
