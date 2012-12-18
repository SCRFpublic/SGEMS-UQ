#ifndef SHAREDPARAMETERSUBWINDOW_H
#define SHAREDPARAMETERSUBWINDOW_H

#include <QWidget>
#include <QFrame>
#include <common.h>
#include <metric_manager_repository.h>
#include <MetricData.h>
#include <MetricDataManager.h>
#include <GsTL/utils/smartptr.h>
#include <GsTLAppli/utils/manager.h>
#include <GsTLAppli/appli/manager_repository.h>

#include <QVTKWidget.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkMath.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkAxis.h>
#include <vtkTextProperty.h>

#include <vtkQtTableView.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkChartXY.h>
#include <vtkPlot.h>
#include <vtkTable.h>
#include <vtkStringArray.h>
#include <vtkFloatArray.h>
#include <vtkIntArray.h>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
/*
namespace Ui {
    class sharedParameterSubwindow;
}
*/
class MultiDimScalingSpace;

class METRICSGUI_DECL sharedParameterSubwindow : public QWidget
{
    Q_OBJECT

public:
    explicit sharedParameterSubwindow(QWidget *parent = 0,
                                      QString mdsObject = QString(),
                                      QStringList properties = QStringList(),
                                      QString parameter = QString());
    ~sharedParameterSubwindow();

protected:

    QSqlDatabase db;
    bool connectToDataBase();
    QString queryDatabase(QString parameter, QString property);

    MultiDimScalingSpace* mdsObject_;

    QVTKWidget *qvtkWidget_;
    vtkSmartPointer<vtkChartXY> chart_;
    vtkSmartPointer<vtkContextView> view_;
    vtkSmartPointer<vtkQtTableView> tableView_;


    vtkSmartPointer<vtkFloatArray> arrMetricValues_;
    vtkSmartPointer<vtkStringArray> labels_;
    vtkSmartPointer<vtkDoubleArray> arrId_;
    vtkSmartPointer<vtkTable> table_;

    QHash<QString,int> data_;

private:
 //   Ui::sharedParameterSubwindow *ui;
    QString parameterName_;
    QString mdsSpaceName_;
};

#endif // SHAREDPARAMETERSUBWINDOW_H
