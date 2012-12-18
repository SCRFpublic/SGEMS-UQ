/*
 * MdsObjectMdiSubWindow.h
 *
 *  Created on: May 1, 2011
 *      Author: aboucher
 */

#ifndef MDSOBJECTMDISUBWINDOW_H_
#define MDSOBJECTMDISUBWINDOW_H_

#include <common.h>
#include <multi_dim_scaling_space.h>

#include <QVTKWidget.h>
#include <vtkTable.h>
#include <vtkQtTableView.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkMaskPoints.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkLookupTable.h>

#include <QMdiSubWindow>
#include <QString>
#include <QSpinBox>
#include <QTableWidgetItem>
#include <QListWidget>

class METRICSGUI_DECL MdsObjectMdiSubWindow : public QMdiSubWindow
{
	Q_OBJECT

public:
	MdsObjectMdiSubWindow(QString mdsObjectName, QWidget* parent=0);
	virtual ~MdsObjectMdiSubWindow();

public slots:
	void highlightSelectedPoint(QTableWidgetItem* item);
	void highlightSelectedPairPoints(QTableWidgetItem* item);
	void toolboxIndexChanged(int index);
	void create_clusters_button_clicked();
	void clusterList_selected(int);

protected :
	void setUpVtkRenderingWidget();

protected :

	int nPoints_;
	std::pair<int,int> selectedPairPoints_;
	MultiDimScalingSpace* mdsObject_;

	QString mdsObjectName_;
	QSpinBox* num_clusters_spinbox_;
	QListWidget* clusterList_;
	QVTKWidget *qvtkWidget_;

	vtkSmartPointer<vtkTable> table_;
	vtkSmartPointer<vtkQtTableView> tableView_;

	vtkSmartPointer<vtkPolyData> vtkData_;
/*
	vtkSmartPointer<vtkPolyDataMapper> mapper_;

	vtkSmartPointer<vtkRenderer> ren_;
	vtkSmartPointer<vtkRenderWindow> renWindow_;
	vtkSmartPointer<vtkRenderWindowInteractor> iren_;
	vtkSmartPointer<vtkLookupTable> colorTable_;
*/
	vtkSmartPointer<vtkOrientationMarkerWidget> orientationWidget_;


	vtkSmartPointer<vtkMaskPoints> vtk_points_;
//	vtkSmartPointer<vtkLookupTableWithEnabling> color_table_;

};

#endif /* MDSOBJECTMDISUBWINDOW_H_ */
