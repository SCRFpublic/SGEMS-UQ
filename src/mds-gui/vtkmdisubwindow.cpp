/*
 * vtkmdisubwindow.cpp
 *
 *  Created on: Apr 27, 2011
 *      Author: aboucher
 */

#include "vtkmdisubwindow.h"

#include <MetricDataManager.h>
#include <MetricData.h>

#include <QHBoxLayout>
#include <QSplitter>
#include <QDebug>

vtkChartMdiSubWindow::vtkChartMdiSubWindow(QWidget* parent) :
	QMdiSubWindow(parent)
{
//	QHBoxLayout* layout = new QHBoxLayout(this);
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

//	this->buildModel();
}

vtkChartMdiSubWindow::~vtkChartMdiSubWindow() {
	// TODO Auto-generated destructor stub
}


void vtkChartMdiSubWindow::buildModel(int i){

	vtkSmartPointer<vtkFloatArray> arrX = vtkSmartPointer<vtkFloatArray>::New();
	arrX->SetName("X Axis");
	table_->AddColumn(arrX);
	vtkSmartPointer<vtkFloatArray> arrC = vtkSmartPointer<vtkFloatArray>::New();
	arrC->SetName("MyMetric1");
	table_->AddColumn(arrC);
	vtkSmartPointer<vtkFloatArray> arrS = vtkSmartPointer<vtkFloatArray>::New();
	arrS->SetName("MyMetric2");
	table_->AddColumn(arrS);

	int numPoints = 29;
	float inc = 7.0 / (numPoints-1);
	table_->SetNumberOfRows(numPoints);
	for (int i = 0; i < numPoints; ++i)
	{
		table_->SetValue(i, 0, i * inc);
		table_->SetValue(i, 1, cos(i * inc) + 0.0);
		table_->SetValue(i, 2, sin(i * inc) + 0.0);
	}

	table_->Update();
	if(i==0) {
		vtkPlot *line = chart_->AddPlot(vtkChart::BAR);
		line->SetInput(table_, 0, 1);
		line->SetColor(255, 0, 0, 255);
		line = chart_->AddPlot(vtkChart::BAR);
		line->SetInput(table_, 0, 2);
		line->SetColor(0, 255, 0, 255);
		line->SetWidth(2.0);
		chart_->SetShowLegend(true);
	}
	else
	{
		vtkPlot *pt = chart_->AddPlot(vtkChart::POINTS);
		pt->SetInput(table_, 1, 2);
		pt->SetColor(255, 0, 0, 255);
	}


	tableView_->SetSplitMultiComponentColumns(true);
	tableView_->AddRepresentationFromInput(table_);
	tableView_->Update();
}
/*
void vtkChartMdiSubWindow::loadData(const QModelIndex& index){
	std::cout<<"drag and drop working"<< index.row()<<std::endl;
}

void vtkChartMdiSubWindow::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat("text/plain")) {
		event->acceptProposedAction();
	}
}

void vtkChartMdiSubWindow::dropEvent(QDropEvent *event)
{
	if(event->mimeData()->hasText() ) {
		QString dropParam = event->mimeData()->text();
		if( !dropParam.contains("MetricDirectory") ) return;

		QStringList dataDirectoryList = dropParam.split("MetricDirectory");
		foreach (QString dataDirectory, dataDirectoryList) {
			QStringList dataDirList = dataDirectory.split("/");
			this->addDataToChart(dataDirList);
		}
	}
    event->acceptProposedAction();
}

void vtkChartMdiSubWindow::setData(QStringList dataDirectoryList){
	foreach (QString dataDirectory, dataDirectoryList) {
		QStringList dataDirList = dataDirectory.split("/");

	}
}
*/
