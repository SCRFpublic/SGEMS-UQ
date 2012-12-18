/*
 * vtkmdisubwindow.h
 *
 *  Created on: Apr 27, 2011
 *      Author: aboucher
 */

#ifndef VTKMDISUBWINDOW_H_
#define VTKMDISUBWINDOW_H_

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

#include <QMdiSubWindow>
#include<QModelIndex>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QStringList>

class METRICSGUI_DECL vtkChartMdiSubWindow : public QMdiSubWindow
{
	Q_OBJECT

public:
	vtkChartMdiSubWindow(QWidget* parent=0);
	virtual ~vtkChartMdiSubWindow();

	void buildModel(int i=0);

public slots:
//	void loadData(const QModelIndex& index);
//	void dragEnterEvent(QDragEnterEvent *event);
//	void dropEvent(QDropEvent *event);

protected :
//	void setData(QStringList);
//	virtual void addDataToChart(QStringList dataDirectory)=0;

protected :
	QVTKWidget *qvtkWidget_;
	vtkSmartPointer<vtkChartXY> chart_;
	vtkSmartPointer<vtkContextView> view_;
	vtkSmartPointer<vtkTable> table_;
	vtkSmartPointer<vtkQtTableView> tableView_;

};

#endif /* VTKMDISUBWINDOW_H_ */
