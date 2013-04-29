#ifndef MDSMETRICPLOTMDISUBWINDOW_H
#define MDSMETRICPLOTMDISUBWINDOW_H

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
#include <QHash>

#include <QTableWidget>

class METRICSGUI_DECL MDSMetricPlotMdiSubWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    explicit MDSMetricPlotMdiSubWindow(QWidget *parent = 0);


    // pairs of metric names and props
    virtual bool initialize(int plotType,
                            QString metricName,
                            QStringList metricProps );

    bool initializeTimeSeries(int plotType,
                              QString metricName,
                              QStringList metricProps);




protected :
    QString metricName_;
    QVTKWidget *qvtkWidget_;
    vtkSmartPointer<vtkChartXY> chart_;
    vtkSmartPointer<vtkContextView> view_;
    vtkSmartPointer<vtkTable> table_;
    vtkSmartPointer<vtkTable> table2_;
    vtkSmartPointer<vtkQtTableView> tableView_;

    vtkSmartPointer<vtkStringArray> labels_;
    vtkSmartPointer<vtkIntArray> arrId_;
    vtkSmartPointer<vtkFloatArray> time_;
    vtkSmartPointer<vtkFloatArray> time2_;
    vtkSmartPointer<vtkFloatArray> value_;
    vtkSmartPointer<vtkFloatArray> value2_;
    vtkSmartPointer<vtkFloatArray>* arrMetricValues_;

signals:

public slots:

};

#endif // MDSMETRICPLOTMDISUBWINDOW_H


