/*
 * domattributeitem.cpp
 *
 *  Created on: Apr 26, 2011
 *      Author: aboucher
 */

#include "domattributeitem.h"

#include <QDomNamedNodeMap>

 DomAttributeItem::DomAttributeItem(QDomNode &node, int row, DomAttributeItem *parent)
 {
     domNode = node;
     rowNumber = row;
     parentItem = parent;
 }


 DomAttributeItem::~DomAttributeItem()
 {
     QHash<int,DomAttributeItem*>::iterator it;
     for (it = childItems.begin(); it != childItems.end(); ++it)
         delete it.value();
 }

 DomAttributeItem *DomAttributeItem::parent()
  {
      return parentItem;
  }

  int DomAttributeItem::row()
  {
      return rowNumber;
  }

  QDomNode DomAttributeItem::node() const
  {
      return domNode;
  }

  DomAttributeItem *DomAttributeItem::child(int i)
  {
      if (childItems.contains(i))
          return childItems[i];

      if (i >= 0 && i < domNode.childNodes().count()) {
          QDomNode childNode = domNode.childNodes().item(i);
          DomAttributeItem *childItem = new DomAttributeItem(childNode, i, this);
          childItems[i] = childItem;
          return childItem;
      }
      QDomNamedNodeMap attributeMap = domNode.attributes();
      if( i>= domNode.childNodes().count() && i < attributeMap.count() ){
    	  QDomNode childNode = attributeMap.item(i - domNode.childNodes().count() );
    	  DomAttributeItem *childItem = new DomAttributeItem(childNode, i, this);
          childItems[i] = childItem;
          return childItem;
      }

      return 0;
  }

int DomAttributeItem::childCount()
{
	return domNode.childNodes().count() + domNode.attributes().count();

}

