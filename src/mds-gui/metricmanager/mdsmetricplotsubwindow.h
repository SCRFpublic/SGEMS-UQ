#ifndef MDSMETRICPLOTSUBWINDOW_H
#define MDSMETRICPLOTSUBWINDOW_H

#include <QWidget>
#include <QMdiSubWindow>
#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QSpinBox>
#include <QtGui>

#include <QtGui>

#include <iostream>

#include <metricmanager/mdsresponseitemdelegate.h>
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
#include <vtkDoubleArray.h>

#include <QMdiSubWindow>
#include <QString>
#include <QStringList>

#include <metric_manager_repository.h>
#include <MetricData.h>
#include <MetricDataManager.h>
#include <GsTL/utils/smartptr.h>
#include <utils/manager.h>
#include <appli/manager_repository.h>

#include <vtkChartHistogram2D.h>
#include <vtkPlotBar.h>
#include <vtkAxis.h>

#include <QSplitter>
#include <QLayout>
#include <QSizePolicy>
#include "colorlisteditor.h"

namespace Ui {
class METRICSGUI_DECL MDSMetricPlotSubWindow;
}

struct ResponsePlotData
{
    vtkSmartPointer<vtkTable> table_;
    vtkSmartPointer<vtkIdType> plotID_;
    vtkSmartPointer<vtkPlot> plot_;
};

class MDSMetricPlotSubWindow : public QMdiSubWindow
{
    Q_OBJECT

public:
    explicit MDSMetricPlotSubWindow(QWidget *parent = 0);
    ~MDSMetricPlotSubWindow();

    // pairs of metric names and props
    // TODO: Merge these into one function
    virtual bool initialize(int plotType,
                            QString gridName,
                            QString metricName,
                            QStringList metricProps );

    bool initializeTimeSeries(int plotType,
                              QString gridName,
                              QString metricName,
                              QStringList metricProps);

    bool initializeVector(int plotType,
                          QString gridName,
                          QString metricName,
                          QStringList metricProps);

    bool initializeDistribution(int plotType,
                                QString gridName,
                                QString metricName,
                                QStringList metricProps);


private:
    Ui::MDSMetricPlotSubWindow *ui;

    MDSResponseItemTable *responseQtTable;

protected :

    QHash < QString, ResponsePlotData> responseHashTable;
    QStringList propertyList;
    QString gridName;
    QString metricName_;
    QVTKWidget *qvtkWidget_;
    vtkSmartPointer<vtkChartXY> chart_;
    vtkSmartPointer<vtkContextView> view_;
    vtkSmartPointer<vtkQtTableView> tableView_;
    vtkSmartPointer<vtkFloatArray>* arrMetricValues_;
    vtkSmartPointer<vtkStringArray> labels_;
    vtkSmartPointer<vtkDoubleArray> arrId_;
    vtkSmartPointer<vtkTable> table_;
public slots:
    void changeDataTableView(QString property);
    void updatePlotVisibility(QString property, bool visible);
    void updatePlotColor(QString property, QColor newColor);
};

#endif // MDSMETRICPLOTSUBWINDOW_H
