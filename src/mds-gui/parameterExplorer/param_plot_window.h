#ifndef PARAM_PLOT_WINDOW_H
#define PARAM_PLOT_WINDOW_H

#include <QWidget>
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

#include <Eigen/Dense>
#include <vector>

using namespace Eigen;

namespace Ui {
    class param_plot_window;
}

class param_plot_window : public QWidget
{
    Q_OBJECT

public:
    explicit param_plot_window(QWidget *parent = 0);

    void plot(QStringList &properties, std::vector<float> &values,
              QString parameter);

    void plot(QStringList &parameters, VectorXf &senVector);

    ~param_plot_window();

protected:
    QVTKWidget *qvtkWidget_;
    vtkSmartPointer<vtkChartXY> chart_;
    vtkSmartPointer<vtkContextView> view_;
    vtkSmartPointer<vtkQtTableView> tableView_;

    vtkSmartPointer<vtkFloatArray> arrMetricValues_;
    vtkSmartPointer<vtkStringArray> labels_;
    vtkSmartPointer<vtkDoubleArray> arrId_;
    vtkSmartPointer<vtkTable> table_;

    Ui::param_plot_window *ui;
};

#endif // PARAM_PLOT_WINDOW_H
