/*
 * vtk_data_from_metric_chart_mdi_subwindow.h
 *
 *  Created on: Apr 27, 2011
 *      Author: aboucher
 */

#ifndef VTK_DATA_FROM_METRIC_CHART_MDI_SUBWINDOW_H_
#define VTK_DATA_FROM_METRIC_CHART_MDI_SUBWINDOW_H_

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
#include <vtkIntArray.h>

#include <QMdiSubWindow>
#include <QString>
#include <QStringList>

class METRICSGUI_DECL ChartDataFromMetricMdiSubWindow: public QMdiSubWindow
{
	Q_OBJECT

public:
	ChartDataFromMetricMdiSubWindow(QWidget* parent=0);
	virtual ~ChartDataFromMetricMdiSubWindow();

	// pairs of metric names and props
	virtual bool initialize(int plotType, QHash<QString, QStringList> metricNamePropsPairs );

protected :
	QString metricName_;
	QVTKWidget *qvtkWidget_;
	vtkSmartPointer<vtkChartXY> chart_;
	vtkSmartPointer<vtkContextView> view_;
	vtkSmartPointer<vtkTable> table_;
	vtkSmartPointer<vtkQtTableView> tableView_;

	vtkSmartPointer<vtkStringArray> labels_;
	vtkSmartPointer<vtkIntArray> arrId_;
	vtkSmartPointer<vtkFloatArray>* arrMetricValues_;

};

#endif /* VTK_DATA_FROM_METRIC_CHART_MDI_SUBWINDOW_H_ */
