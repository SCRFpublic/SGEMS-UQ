/*
 * parametertreeview.cpp
 *
 *  Created on: Apr 26, 2011
 *      Author: aboucher
 */

#include "parametertreeview.h"
#include <metric_manager_repository.h>
#include <domitem.h>
#include <MetricData.h>
#include <MetricDataManager.h>
#include <parameterdommodel.h>

#include <GsTL/utils/smartptr.h>
#include <GsTLAppli/utils/manager.h>
#include <GsTLAppli/appli/manager_repository.h>

#include <QDomNode>
#include <QVBoxLayout>

ParameterTreeView::ParameterTreeView(QWidget* parent) :
	QWidget(parent), parameterModel_(0)
{
	treeView_ = new QTreeView(this);
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(treeView_);

	// TODO Auto-generated constructor stub

}

ParameterTreeView::~ParameterTreeView() {
	// TODO Auto-generated destructor stub
}

void ParameterTreeView::showParameters(const QModelIndex& index){
// We need to have the property passed,
//  one level up : grid name
// two level up: metric name

	if(!index.isValid()  ||                              //Property level
	   !index.parent().isValid() ||                      //Grid level
	   !index.parent().parent().isValid()) return;         //Metric Level

	DomItem *itemProp = static_cast<DomItem*>(index.internalPointer());
	DomItem *itemGrid = static_cast<DomItem*>(index.parent().internalPointer());
	DomItem *itemMetric = static_cast<DomItem*>(index.parent().parent().internalPointer());

	QString propName = itemProp->node().nodeName();
	QString gridName = itemGrid->node().nodeName();
	QString metricName = itemMetric->node().nodeName();

    SmartPtr<Named_interface> ni =
                            Root::instance()->interface( metricData_manager + "/metricRegistrar");

    //return dynamic_cast<MetricDataManager*>(ni.raw_ptr());
    MetricDataManager* mRegistrar = dynamic_cast<MetricDataManager*>(ni.raw_ptr());

    MetricData* metric =
    		mRegistrar->getMetricData(gridName.toStdString(),
									  propName.toStdString(), metricName.toStdString() );



    delete parameterModel_;
    treeView_->reset();

    QDomElement domElem =  docParam_.createElement("Parameters");

/*
    QDomElement child = metric->metaDataXml().firstChildElement();
    for(; child.isNull(); child = child.nextSiblingElement() ) {
    	QDomElement domElem =  docParam_.createElement("");
    	domElem = child;
    	docParam_.appendChild(domElem);
    }
*/
    domElem = metric->metaDataXml();
    docParam_.appendChild(domElem);

    parameterModel_ = new ParameterDomModel( docParam_ );

    treeView_->setModel(parameterModel_);



}
