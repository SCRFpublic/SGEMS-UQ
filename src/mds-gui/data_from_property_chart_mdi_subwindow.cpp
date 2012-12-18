/*
 * chart_data_from_property_mdi_subwindow.cpp
 *
 *  Created on: Apr 29, 2011
 *      Author: aboucher
 */

#include "data_from_property_chart_mdi_subwindow.h"

#include <metric_manager_repository.h>
#include <MetricData.h>
#include <MetricDataManager.h>
#include <GsTL/utils/smartptr.h>
#include <GsTLAppli/utils/manager.h>
#include <GsTLAppli/appli/manager_repository.h>

#include <QSplitter>
#include <QLayout>

ChartDataFromPropertyMdiSubWindow::ChartDataFromPropertyMdiSubWindow(QWidget* parent) :
	QMdiSubWindow(parent)
{
	qvtkWidget_ = new QVTKWidget(this);
	view_ = vtkSmartPointer<vtkContextView>::New();
	view_->SetInteractor(qvtkWidget_->GetInteractor());
	qvtkWidget_->SetRenderWindow(view_->GetRenderWindow());
	chart_ = vtkSmartPointer<vtkChartXY>::New();
	view_->GetScene()->AddItem(chart_);
	tableView_ = vtkSmartPointer<vtkQtTableView>::New();
	table_ = vtkSmartPointer<vtkTable>::New();

	QSplitter* mainSplitter = new QSplitter(Qt::Horizontal, this);

	mainSplitter->addWidget(qvtkWidget_);
	mainSplitter->addWidget(tableView_->GetWidget());

	this->layout()->addWidget(mainSplitter);

	this->setAcceptDrops(true);


}

ChartDataFromPropertyMdiSubWindow::~ChartDataFromPropertyMdiSubWindow() {
	// TODO Auto-generated destructor stub
}


bool ChartDataFromPropertyMdiSubWindow::initialize(QString metricName, QStringList props ){

	SmartPtr<Named_interface> ni =
	         Root::instance()->interface( metricData_manager + "/metricRegistrar");

	//return dynamic_cast<MetricDataManager*>(ni.raw_ptr());
	MetricDataManager* mRegistrar = dynamic_cast<MetricDataManager*>(ni.raw_ptr());

	metricName_ = metricName;
	this->setWindowTitle("Metric name: "+metricName_);
//	table_->SetName(metricName.toStdString());

	//vtkSmartPointer<vtkStringArray> arrX = vtkSmartPointer<vtkStringArray>::New();
	arrX_ = vtkSmartPointer<vtkStringArray>::New();
	arrX_->SetName("Metrics");
	table_->AddColumn(arrX_);
	arrY_ = vtkSmartPointer<vtkFloatArray>::New();
	arrY_->SetName("Value");
	table_->AddColumn(arrY_);
	table_->SetNumberOfRows(props.size());

	std::string metricName_str = metricName_.toStdString();
	for( int i=0; i<props.size(); ++i  ) {
		QStringList path = props.at(i).split("/");
		MetricData* mdata = mRegistrar->getMetricData(path.at(0).toStdString(),
													  path.at(1).toStdString(),
													  metricName_str);
		if( mdata==0 ) return false;

//		QString valStr =
		arrX_->InsertValue(i,props.value(i).toStdString().c_str());
		arrY_->SetValue(i,mdata->data(0));

	}
	table_->Update();

	vtkPlot *bar = chart_->AddPlot(vtkChart::BAR);
	bar->SetInput(table_, 0, 1);


	tableView_->SetSplitMultiComponentColumns(true);
	tableView_->AddRepresentationFromInput(table_);
	tableView_->Update();

	return true;

}
