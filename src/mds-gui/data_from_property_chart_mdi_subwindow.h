/*
 * chart_data_from_property_mdi_subwindow.h
 *
 *  Created on: Apr 29, 2011
 *      Author: aboucher
 */

#ifndef CHART_DATA_FROM_PROPERTY_MDI_SUBWINDOW_H_
#define CHART_DATA_FROM_PROPERTY_MDI_SUBWINDOW_H_

#include <common.h>
#include <QVTKWidget.h>
#include <vtkFloatArray.h>
#include <vtkMath.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>

#include <vtkQtTableView.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkChartXY.h>
#include <vtkPlot.h>
#include <vtkTable.h>
#include <vtkStringArray.h>
#include <vtkFloatArray.h>

#include <QMdiSubWindow>
#include <QString>
#include <QStringList>

class METRICSGUI_DECL ChartDataFromPropertyMdiSubWindow : public  QMdiSubWindow{
	Q_OBJECT

public:
	ChartDataFromPropertyMdiSubWindow(QWidget* parent=0);
	virtual ~ChartDataFromPropertyMdiSubWindow();

	virtual bool initialize(QString metricName, QStringList props );

protected :
	QString metricName_;
	QVTKWidget *qvtkWidget_;
	vtkSmartPointer<vtkChartXY> chart_;
	vtkSmartPointer<vtkContextView> view_;
	vtkSmartPointer<vtkTable> table_;
	vtkSmartPointer<vtkQtTableView> tableView_;

	vtkSmartPointer<vtkStringArray> arrX_;
	vtkSmartPointer<vtkFloatArray> arrY_;
};

#endif /* CHART_DATA_FROM_PROPERTY_MDI_SUBWINDOW_H_ */
