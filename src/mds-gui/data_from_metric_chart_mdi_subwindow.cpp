/*
 * vtk_data_from_metric_chart_mdi_subwindow.cpp
 *
 *  Created on: Apr 27, 2011
 *      Author: aboucher
 */

#include "data_from_metric_chart_mdi_subwindow.h"

#include <metric_manager_repository.h>
#include <MetricData.h>
#include <MetricDataManager.h>
#include <GsTL/utils/smartptr.h>
#include <GsTLAppli/utils/manager.h>
#include <GsTLAppli/appli/manager_repository.h>

#include <vtkChartHistogram2D.h>
#include <vtkPlotBar.h>
#include <vtkAxis.h>

#include <QSplitter>
#include <QLayout>
#include <QSizePolicy>

ChartDataFromMetricMdiSubWindow::ChartDataFromMetricMdiSubWindow(QWidget* parent) :
	QMdiSubWindow(parent), arrMetricValues_(0)
{
	qvtkWidget_ = new QVTKWidget(this);
	view_ = vtkSmartPointer<vtkContextView>::New();
	view_->SetInteractor(qvtkWidget_->GetInteractor());
	qvtkWidget_->SetRenderWindow(view_->GetRenderWindow());

	chart_ = vtkSmartPointer<vtkChartXY>::New();
	view_->GetScene()->AddItem(chart_);
	tableView_ = vtkSmartPointer<vtkQtTableView>::New();
	table_ = vtkSmartPointer<vtkTable>::New();


	QSizePolicy vtkPolicy = qvtkWidget_->sizePolicy();
	vtkPolicy.setHorizontalStretch(3);
	qvtkWidget_->setSizePolicy(vtkPolicy);
	QSizePolicy tablePolicy = tableView_->GetWidget()->sizePolicy();
	tablePolicy.setHorizontalStretch(1);
	tableView_->GetWidget()->setSizePolicy(tablePolicy);

	QSplitter* mainSplitter = new QSplitter(Qt::Horizontal, this);

	mainSplitter->addWidget(qvtkWidget_);
	mainSplitter->addWidget(tableView_->GetWidget());

	this->layout()->addWidget(mainSplitter);

	this->setAcceptDrops(true);

}

ChartDataFromMetricMdiSubWindow::~ChartDataFromMetricMdiSubWindow() {
	// reclaim memory
	if (arrMetricValues_) {
		delete[] arrMetricValues_;
	}
}


bool ChartDataFromMetricMdiSubWindow::initialize(int plotType, QHash<QString, QStringList> metricNamePropsPairs){

	std::vector<int> red;
	std::vector<int> green;
	std::vector<int> blue;
	std::vector<int> alpha;

	red.push_back(100);
	green.push_back(149);
	blue.push_back(237);
	alpha.push_back(200);

	red.push_back(205);
	green.push_back(175);
	blue.push_back(149);
	alpha.push_back(200);

	red.push_back(112);
	green.push_back(138);
	blue.push_back(144);
	alpha.push_back(200);
	// create table and get registrar
	SmartPtr<Named_interface> ni =
	         Root::instance()->interface( metricData_manager + "/metricRegistrar");

	MetricDataManager* mRegistrar = dynamic_cast<MetricDataManager*>(ni.raw_ptr());

	QString windowTitle;
	if (metricNamePropsPairs.size() <= 1)
		windowTitle.append("Metric name: ");
	else
		windowTitle.append("Metrics: ");

	arrId_ = vtkSmartPointer<vtkIntArray>::New();
	arrId_->SetName("Id");
	table_->AddColumn(arrId_);
	labels_ = vtkSmartPointer<vtkStringArray>::New();
	labels_->SetName("Property");
	table_->AddColumn(labels_);

	QSet<QString> propertySet; // set of properties

	// loop through metric name, prop pairs
	int numMetrics = metricNamePropsPairs.size();
	arrMetricValues_ = new vtkSmartPointer<vtkFloatArray>[numMetrics];
	for (int i = 0; i < metricNamePropsPairs.size(); i++) {
		arrMetricValues_[i] = vtkSmartPointer<vtkFloatArray>::New();
	}

	QHash<QString, QStringList>::const_iterator it = metricNamePropsPairs.constBegin();
	int ct = 0;
	 for( ; it != metricNamePropsPairs.constEnd(); ++it, ++ct) {
		 QString metricName = it.key();
		 QStringList props = it.value();


		 if (ct)
			 // not first item
			 windowTitle.append(", " + metricName);
		 else
			windowTitle.append(metricName);

		 arrMetricValues_[ct]->SetName(metricName.toStdString().c_str());

		table_->AddColumn(arrMetricValues_[ct]);

		for( int i=0; i<props.size(); ++i  ) {
			QStringList path = props.at(i).split("/");
			propertySet.insert(props.value(i));
		}
	 }
	this->setWindowTitle(windowTitle);
	table_->SetNumberOfRows(propertySet.size());

	QList<QString> sortedProperties = propertySet.toList();
	qSort(sortedProperties);
	for (int i = 0; i < sortedProperties.size(); i++) {
		labels_->SetValue(i,sortedProperties[i].toStdString().c_str());

		table_->SetValue(i, 0, i); // id
		table_->SetValue(i, 1, sortedProperties[i].toStdString().c_str()); // property name
		for (int j = 0; j < numMetrics; j++)
			table_->SetValue(i, 2 + j, 0);
	}

	it = metricNamePropsPairs.constBegin();
	ct = 0;
	// loop through metric name, prop pairs
	 for( ; it != metricNamePropsPairs.constEnd(); ++it, ++ct) {
		 QString metricName = it.key();
		 QStringList props = it.value();
		std::string metricName_str = metricName.toStdString();
		for( int i=0; i<props.size(); ++i  ) {
			QStringList path = props.at(i).split("/");
			MetricData* mdata = mRegistrar->getMetricData(path.at(0).toStdString(),
														  path.at(1).toStdString(),
														  metricName_str);
			if( mdata==0 ) return false;

			int rowIndex = sortedProperties.indexOf(props.value(i));
			table_->SetValue( rowIndex,ct + 2,mdata->data(0) );

		}
	 }
//	table_->Update();

	// add bars
	it = metricNamePropsPairs.constBegin();
	ct = 0;
	if( plotType == vtkChart::BAR ) {
	 for( ; it != metricNamePropsPairs.constEnd(); ++it, ++ct) {
		 QString metricName = it.key();
			vtkPlot *plot = chart_->AddPlot(plotType);
            plot->SetInputData(table_, 0,ct +2);
			plot->SetLabel(vtkStdString(metricName.toStdString().c_str()));
			plot->SetColor( red[ct],green[ct],blue[ct], alpha[ct] );
	 }
	 if (numMetrics > 1)
		chart_->SetShowLegend(true);
	 chart_->GetAxis(vtkAxis::BOTTOM)->SetTitle(vtkStdString("Property"));
	 chart_->GetAxis(vtkAxis::LEFT)->SetTitle(vtkStdString("Metric value"));
	}
	else if( plotType == vtkChart::POINTS ) {
			 for( ; it != metricNamePropsPairs.constEnd(); ++it, ++ct) {
				 QString metricName = it.key();
				 int ct_second = ct+1;
				 QHash<QString, QStringList>::const_iterator it_second = it;
				 ++it_second;
				 for( ; it_second != metricNamePropsPairs.constEnd(); ++it_second, ++ct_second) {
					QString metricNameSecond = it_second.key();
					vtkPlot *plot = chart_->AddPlot(plotType);
                    plot->SetInputData(table_, ct+2,ct_second +2);
					std::string labelPlot =  metricName.toStdString() + " and "
											 + metricNameSecond.toStdString();
					plot->SetLabel(vtkStdString(labelPlot.c_str()));
					plot->SetColor( red[ct],green[ct],blue[ct], alpha[ct] );

					plot->GetXAxis()->SetTitle(vtkStdString(metricName.toStdString().c_str()));
					plot->GetYAxis()->SetTitle(vtkStdString(metricNameSecond.toStdString().c_str()));

			 }
		 }

	}


	




	tableView_->SetSplitMultiComponentColumns(true);
	tableView_->SetRepresentationFromInput(table_);
	tableView_->Update();

	return true;

}
