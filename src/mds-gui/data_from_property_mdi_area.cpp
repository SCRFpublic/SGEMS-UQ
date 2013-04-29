/*
 * chart_data_from_property_mdi_area.cpp
 *
 *  Created on: Apr 29, 2011
 *      Author: aboucher
 */

#include "data_from_property_mdi_area.h"

#include "data_from_property_chart_mdi_subwindow.h"

#include <metric_manager_repository.h>
#include <MetricData.h>
#include <MetricDataManager.h>
#include <GsTL/utils/smartptr.h>
#include <GsTLAppli/utils/manager.h>
#include <GsTLAppli/appli/manager_repository.h>

ChartDataFromPropertyMdiArea::ChartDataFromPropertyMdiArea(QWidget* parent)
	: QMdiArea(parent)
{
	this->setAcceptDrops(true);
}

ChartDataFromPropertyMdiArea::~ChartDataFromPropertyMdiArea() {
}


void ChartDataFromPropertyMdiArea::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat("text/plain")) {
		event->acceptProposedAction();
	}
}

void ChartDataFromPropertyMdiArea::dropEvent(QDropEvent *event)
{
	if(event->mimeData()->hasText() ) {
		QString dropParam = event->mimeData()->text();
		if( !dropParam.contains("//MetricPath/") ) return;

		qDebug()<<dropParam;
		QStringList dataDirectoryList = dropParam.split("//MetricPath/", QString::SkipEmptyParts);

		QHash<QString, QStringList> params;
		foreach (QString dataDirectory, dataDirectoryList) {
			qDebug()<<dataDirectory;
			QStringList dataDirList = dataDirectory.split("/", QString::SkipEmptyParts);
			QString metricName = dataDirList.value(0);
			qDebug()<<"Name of metrics "<<metricName;
			dataDirList.pop_front();
			if( !params.contains(metricName) )
				params[metricName] = QStringList();

			if( !dataDirList.isEmpty())
				params[metricName].append(dataDirList.join("/"));

		}

		 QHash<QString, QStringList>::const_iterator i = params.constBegin();
		 for( ; i != params.constEnd(); ++i){
			 this->addChart(i.key(),i.value());
		 }
	}
    event->acceptProposedAction();
}


void ChartDataFromPropertyMdiArea::addChart(QString metricName, QStringList dataDirectoryList){
/*
	SmartPtr<Named_interface> ni =
	               Root::instance()->interface( metricData_manager + "/metricRegistrar");

	//return dynamic_cast<MetricDataManager*>(ni.raw_ptr());
	MetricDataManager* mRegistrar = dynamic_cast<MetricDataManager*>(ni.raw_ptr());
	MetricDataToProperties  propFromMetricRegistrar =
			mRegistrar->get_properties_for_metric(metricName.toStdString());

	QStringList props;

	foreach (QString dataDirectory, dataDirectoryList) {
		QStringList dataDirList = dataDirectory.split("/",QString::SkipEmptyParts);

		//We need to build a list of list of grid/prop
		if(dataDirList.isEmpty() )  {
			std::vector<std::string> grid_names;
			propFromMetricRegistrar.getGridNames(grid_names);
			for(int i=0; i<grid_names.size();i++) {
				std::vector<std::string> prop_names;
				propFromMetricRegistrar.getPropertyNames(grid_names[i],prop_names);
				for(int j=0; j<prop_names.size(); j++) {
					props.append( QString("%1/%2").arg(grid_names[i].c_str(),prop_names[j].c_str() ));
				}
			}
		}
		else if(dataDirList.size() == 1)  {
			std::string grid_name = dataDirList.value(0).toStdString();
			std::vector<std::string> prop_names;
			propFromMetricRegistrar.getPropertyNames(grid_name,prop_names);
			for(int j=0; j<prop_names.size(); j++) {
				props.append( QString("%1/%2").arg(grid_name.c_str(),prop_names[j].c_str() ));
			}

		}
		else if(dataDirList.size() == 2){
			props.append( QString("%1/%2").arg(dataDirList.value(0)).arg(dataDirList.value(1)) );
		}
	}

	props.sort();

	ChartDataFromMetricMdiSubWindow *chart  = new ChartDataFromMetricMdiSubWindow(this);

	bool ok = true;// chart->initialize(metricName,props);

	if(ok )
		this->addSubWindow(chart);
	else
		delete chart;
		*/

}

