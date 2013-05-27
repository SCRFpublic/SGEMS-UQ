/*
 * chartmdiarea.cpp
 *
 *  Created on: Apr 27, 2011
 *      Author: aboucher
 */

#include "chartmdiarea.h"
#include "data_from_metric_chart_mdi_subwindow.h"

#include <metric_manager_repository.h>
#include <MetricData.h>
#include <MetricDataManager.h>
#include <GsTL/utils/smartptr.h>
#include <utils/manager.h>
#include <appli/manager_repository.h>

ChartDataFromMetricMdiArea::ChartDataFromMetricMdiArea(QWidget* parent)
	: QMdiArea(parent)
{
	this->setAcceptDrops(true);
}

ChartDataFromMetricMdiArea::~ChartDataFromMetricMdiArea() {
}


void ChartDataFromMetricMdiArea::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat("text/plain")) {
		event->acceptProposedAction();
	}
}

void ChartDataFromMetricMdiArea::dropEvent(QDropEvent *event)
{

	if(event->mimeData()->hasText() ) {
		QString dropParam = event->mimeData()->text();

		if( !dropParam.contains("//MetricPath/") ) return;

		qDebug()<<dropParam;
		QStringList mimeTextList = dropParam.split("\n", QString::SkipEmptyParts);
		QHash<QString, QStringList> params;
		foreach (QString mimeText, mimeTextList) {
			QStringList dataDirectoryList = mimeText.split("//MetricPath/", QString::SkipEmptyParts);

			
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
		}
		
		this->addCharts(params);
	}
    event->acceptProposedAction();
}


void ChartDataFromMetricMdiArea::addCharts(QHash<QString, QStringList>& metricNameAndDataDirectoryPairs){

	// set up registrar
	    SmartPtr<Named_interface> ni =
	                            Root::instance()->interface( metricData_manager + "/metricRegistrar");

	MetricDataManager* mRegistrar = dynamic_cast<MetricDataManager*>(ni.raw_ptr());
	std::vector<std::string> metric_names = mRegistrar->get_metric_names();
	QHash<QString, QStringList> metricProps;

	// loop through metrics
	 QHash<QString, QStringList>::const_iterator it = metricNameAndDataDirectoryPairs.constBegin();
	 for( ; it != metricNameAndDataDirectoryPairs.constEnd(); ++it) {
		 QString metricName = it.key();
		 QStringList dataDirectoryList = it.value();

		 // check that this is a valid metric name
		 std::string metric_name = metricName.toStdString();
		 bool valid = false;
		 for (int i = 0; i < metric_names.size(); i++) {
			 if (metric_names[i].compare(metric_name) == 0) {
				valid = true;
				break;
			 }
		 }
		 if (!valid) continue;
		MetricDataToProperties  propFromMetricRegistrar = mRegistrar->get_properties_for_metric(metric_name);

		QStringList props;

		if(dataDirectoryList.isEmpty() )  {
				std::vector<std::string> grid_names;
				propFromMetricRegistrar.getGridNames(grid_names);
				for(int i=0; i<grid_names.size();i++) {
					std::vector<std::string> prop_names;
					propFromMetricRegistrar.getPropertyNames(grid_names[i],prop_names);
					for(int j=0; j<prop_names.size(); j++) {
						props.append( QString("%1/%2").arg(grid_names[i].c_str(),prop_names[j].c_str() ));
					}
				}
		} else {
			foreach (QString dataDirectory, dataDirectoryList) {
				QStringList dataDirList = dataDirectory.split("/",QString::SkipEmptyParts);

				//We need to build a list of list of grid/prop
				if(dataDirList.size() == 1)  {
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
		}

		props.sort();
		metricProps.insert(metricName, props);
	 }

	ChartDataFromMetricMdiSubWindow *barChart  = new ChartDataFromMetricMdiSubWindow(this);
	bool ok = barChart->initialize(vtkChart::BAR, metricProps);
	if (ok) {
		this->addSubWindow(barChart);
		barChart->show();
	}
	else
		delete barChart;
	
// Only build the scatter plot if more than one set of metrics were selected
	if(metricProps.size() > 1) {
		ChartDataFromMetricMdiSubWindow *scatterChart  = new ChartDataFromMetricMdiSubWindow(this);
		ok = scatterChart->initialize(vtkChart::POINTS, metricProps);
		if (ok) {
			this->addSubWindow(scatterChart);
			scatterChart->show();
		}
		else{
			delete scatterChart;
		}
	}



}
