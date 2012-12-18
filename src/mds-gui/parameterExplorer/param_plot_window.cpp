#include "param_plot_window.h"
#include "ui_param_plot_window.h"

param_plot_window::param_plot_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::param_plot_window)
{
    ui->setupUi(this);
}

param_plot_window::~param_plot_window()
{
    delete ui;
}

void param_plot_window::plot(QStringList &properties,
                             std::vector<float> &values, QString parameter)
{
    qvtkWidget_ = new QVTKWidget(this);

    setWindowTitle(parameter);

    view_ = vtkSmartPointer<vtkContextView>::New();
    view_->GetRenderer()->SetBackground(1.0,1.0,1.0);
    view_->GetRenderWindow()->SetSize(400,300);
    view_->SetInteractor(qvtkWidget_->GetInteractor());
    qvtkWidget_->SetRenderWindow(view_->GetRenderWindow());

    // Create new table with some points
    chart_ = vtkSmartPointer<vtkChartXY>::New();
    view_->GetScene()->AddItem(chart_);

    QSizePolicy vtkPolicy = qvtkWidget_->sizePolicy();
    vtkPolicy.setHorizontalStretch(3);
    qvtkWidget_->setSizePolicy(vtkPolicy);

    // Create a VTK table for storing data
    table_ = vtkSmartPointer<vtkTable>::New();

    // Create an array for listing property index
    arrId_ = vtkSmartPointer<vtkDoubleArray>::New();
    arrId_->SetName("Id");

    // Label for storing name of property
    labels_ = vtkSmartPointer<vtkStringArray>::New();
    labels_->SetName("Property");

    // Create a float array for storing metric data
    arrMetricValues_ = vtkSmartPointer<vtkFloatArray>::New();
    arrMetricValues_->SetName("Values");

    // Add columns to table
    table_->AddColumn(arrId_);
    table_->AddColumn(labels_);
    table_->AddColumn(arrMetricValues_);
    table_->SetNumberOfRows(properties.size());

    // Write data
    for (unsigned int i = 0; i < properties.size(); ++i)
    {
        // Set ID
        table_->SetValue(i,0,float(i));

        // Set Label
        table_->SetValue(i,1,properties.at(i).toStdString().c_str());

        // Set Value
        table_->SetValue(i,2,values.at(i));
    }

    // Add multiple line plots, setting the colors etc
    vtkPlot *line = 0;
    line = chart_->AddPlot(vtkChart::BAR);
    line->SetInput(table_, 0, 2);
    line->SetColor(67,	110	,238, 255);

    vtkAxis *x_axis = chart_->GetAxis(vtkAxis::BOTTOM);
    x_axis->SetBehavior(1);
    x_axis->SetMinimum(-1);
    x_axis->SetMaximum(properties.size());
    x_axis->SetTitle("Realizations");
    x_axis->GetTitleProperties()->SetFontSize(16);

    x_axis->SetTickPositions(arrId_);
    x_axis->SetTickLabels(labels_);
    x_axis->GetLabelProperties()->SetOrientation(90);
    x_axis->GetLabelProperties()->SetFontSize(16);



    vtkAxis *y_axis = chart_->GetAxis(vtkAxis::LEFT);
    y_axis->SetTitle("Parameter");
    y_axis->GetTitleProperties()->SetFontSize(14);
    x_axis->GetLabelProperties()->SetFontSize(14);

    chart_->Update();

    view_->GetInteractor()->Initialize();
    view_->GetInteractor()->Start();

    ui->scrollArea->setWidget(qvtkWidget_);




}
