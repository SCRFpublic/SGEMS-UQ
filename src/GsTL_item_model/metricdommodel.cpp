/*
 * metricdommodel.cpp
 *
 *  Created on: Apr 26, 2011
 *      Author: aboucher
 */

#include "metricdommodel.h"
#include "metric_manager_repository.h"
#include "MetricDataManager.h"

#include <GsTL/utils/smartptr.h>
#include <utils/manager.h>
#include <appli/manager_repository.h>
#include <appli/project.h>

#include <QFileDialog>

#include <vector>

using namespace std;

MetricDomModel::MetricDomModel(QDomDocument document, QObject *parent)
	:DomModel(document, parent)
{
}

MetricDomModel::~MetricDomModel()
{
}


QVariant
MetricDomModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0:
                return tr("Name");
            case 1:
                return tr("Type");
            case 2:
                return tr("Format");
            default:
                return QVariant();
        }
    }

    return QVariant();
}

QVariant MetricDomModel::data(const QModelIndex &index, int role) const
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
        	if( !attributeMap.contains("Type") ) {
        		return QVariant();
        	}
        	return attributeMap.namedItem("Type").nodeValue();
        case 2:
        	if( !attributeMap.contains("Format") ) {
        		return QVariant();
        	}
        	return attributeMap.namedItem("Format").nodeValue();
        default:
            return QVariant();
    }
}

QStringList MetricDomModel::mimeTypes() const
{
    QStringList types;
    types << "text/plain";
    return types;
}

QMimeData *MetricDomModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
	QString mimeString;

	for (int i = 0; i < indexes.size(); i+=3) {
		QModelIndex index = indexes.at(i);

    //The format is MetricName[/gridName[/propName]]

		QString mimeText("//MetricPath/");

		if (index.isValid()) {
			if (i != 0)
				mimeString.append("\n");
			QStringList itemNamesForIndex;
			QModelIndex indexItem(index);
			while(indexItem.isValid()) {
				itemNamesForIndex.prepend( data(indexItem, Qt::DisplayRole).toString()  );
				indexItem = indexItem.parent();
			}
			mimeText.append(itemNamesForIndex.join("/"));
		} else continue;

		mimeString.append(mimeText);
	}
    mimeData->setText(mimeString);

    return mimeData;
}

void MetricDomModel::AddQDomForProperty(MetricDataManager* registrar, QDomDocument& doc, QDomElement& root, string& metric_name, string& grid_name, string& property_name) {
		vector<string> metric_names;
		metric_names.push_back(metric_name);
		MetricDataCollection mc = registrar->getMetricDataCollection(grid_name, property_name, metric_names);

		std::vector<MetricData*>::iterator it;
		for (it = mc.begin(); it != mc.end(); ++it) {
			string class_name = (*it)->classname();
			string data_format = (*it)->dataFormat();
			string m_name = (*it)->name();
			QDomElement mdata = doc.createElement( "MetricData");
			mdata.setAttribute("name", m_name.c_str());
			mdata.setAttribute("class_name", class_name.c_str());
			mdata.setAttribute("data_format", data_format.c_str());
			mdata.setAttribute("grid", grid_name.c_str());
			mdata.setAttribute("property", property_name.c_str());
			mdata.appendChild((*it)->metaDataXml().cloneNode());
			mdata.appendChild((*it)->serializedDataXml().cloneNode());
			root.appendChild(mdata);
		}
}

// save to file
void MetricDomModel::SaveToFile(const QModelIndex& selectedItem, const QString& fileName) {
  QFile file( fileName );
  if( !file.open( QIODevice::WriteOnly ) ) {
	  printf("ERROR writing to file %s\n", fileName.toStdString().c_str());
	return ;
  }

    DomItem *item = static_cast<DomItem*>(selectedItem.internalPointer());
	vector<QDomNode> nodes;
	while(item) {
		nodes.push_back(item->node());
		item = item->parent();
	}

	SmartPtr<Named_interface> ni =
				Root::instance()->interface( metricData_manager + "/metricRegistrar");

	MetricDataManager* mDataRegistrar = (MetricDataManager*)(ni.raw_ptr());

	// create QDom
	QDomDocument doc("save");
	QDomElement root = doc.createElement( "MetricDataCollection" );
	doc.appendChild( root );
	
	if (nodes.size() == 4) {
		// save one property
		string metric_name = nodes[2].nodeName().toStdString();
		string grid_name = nodes[1].nodeName().toStdString();
		string property_name = nodes[0].nodeName().toStdString();
		AddQDomForProperty(mDataRegistrar, doc, root, metric_name,grid_name,property_name);
	} else if (nodes.size() == 3) {
		// save all properties in the grid
		string metric_name = nodes[1].nodeName().toStdString();
		string grid_name = nodes[0].nodeName().toStdString();
		string property_name;

		item = static_cast<DomItem*>(selectedItem.internalPointer());
		QHash<int,DomItem*>::iterator it;
		for (it = item->begin(); it != item->end(); ++it) {
			property_name = it.value()->node().nodeName().toStdString();
			AddQDomForProperty(mDataRegistrar, doc, root, metric_name,grid_name,property_name);
		}
		
	} else if (nodes.size() == 2) {
		// save all grids with these metric
		string metric_name = nodes[0].nodeName().toStdString();
		string grid_name;
		string property_name;
		DomItem* item1;

		item = static_cast<DomItem*>(selectedItem.internalPointer());
		QHash<int,DomItem*>::iterator it;
		QHash<int,DomItem*>::iterator it1;
		for (it = item->begin(); it != item->end(); ++it) {
			grid_name = it.value()->node().nodeName().toStdString();
			item1 = it.value();
			for (it1 = item1->begin(); it1 != item1->end(); ++it1) {
				property_name = it1.value()->node().nodeName().toStdString();
				AddQDomForProperty(mDataRegistrar, doc, root, metric_name,grid_name,property_name);
			}
		}
	}

  QTextStream ts( &file );
  ts << doc.toString();

  file.close();
}