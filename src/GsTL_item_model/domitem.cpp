/*
 * DomItem.cpp
 *
 *  Created on: Apr 26, 2011
 *      Author: aboucher
 */

#include "domitem.h"

 DomItem::DomItem(QDomNode &node, int row, DomItem *parent)
 {
     domNode = node;
     rowNumber = row;
     parentItem = parent;
 }


 DomItem::~DomItem()
 {
     QHash<int,DomItem*>::iterator it;
     for (it = childItems.begin(); it != childItems.end(); ++it)
         delete it.value();
 }

 DomItem *DomItem::parent()
  {
      return parentItem;
  }

  int DomItem::row()
  {
      return rowNumber;
  }

  QDomNode DomItem::node() const
  {
      return domNode;
  }

  DomItem *DomItem::child(int i)
  {
      if (childItems.contains(i))
          return childItems[i];

      if (i >= 0 && i < domNode.childNodes().count()) {
          QDomNode childNode = domNode.childNodes().item(i);
          DomItem *childItem = new DomItem(childNode, i, this);
          childItems[i] = childItem;
          return childItem;
      }
      return 0;
  }
