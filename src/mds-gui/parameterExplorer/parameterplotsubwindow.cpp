#include "parameterplotsubwindow.h"
#include <multi_dim_scaling_space.h>
#include "ui_parameterplotsubwindow.h"

parameterPlotSubWindow::parameterPlotSubWindow(QWidget *parent,
                                               QString mdsObject,
                                               QStringList properties,
                                               QString parameter) :
    QWidget(parent),mdsSpaceName_(mdsObject), parameterName_(parameter),
    ui(new Ui::parameterPlotSubWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(parameter.split("$").join("\\"));

    // Look up MDS Object
    SmartPtr<Named_interface> ni =
            Root::instance()->interface( mds_manager+"/"+
                                         mdsObject.toStdString());
    this->mdsObject_ = dynamic_cast<MultiDimScalingSpace*>(ni.raw_ptr());

    // Connect to SQL database
    this->connectToDataBase();

    // Prepare VTK Widget
    qvtkWidget_ = new QVTKWidget(this);

    // Create a new view
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



    // Set up data
    // Here are plotting a bar plot with a bar for each property and the bar
    // length being the value of the property in question

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
        float currentValue = queryDatabase(parameter,
                                           properties.at(i)).toFloat();

        // Set ID
        table_->SetValue(i,0,double(i));

        // Set Label
        table_->SetValue(i,1,properties.at(i).toStdString().c_str());

        // Set Value
        table_->SetValue(i,2,currentValue);

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
    x_axis->GetLabelProperties()->SetFontSize(16);

    x_axis->SetTickPositions(arrId_);
    x_axis->SetTickLabels(labels_);
    x_axis->GetLabelProperties()->SetOrientation(90);
//    x_axis->GetLabelProperties()->SetVerticalJustification(VTK_TEXT_CENTERED);
//    x_axis->GetLabelProperties()->SetJustification(VTK_TEXT_RIGHT);

    vtkAxis *y_axis = chart_->GetAxis(vtkAxis::LEFT);
    y_axis->SetTitle("Parameter");
    y_axis->GetTitleProperties()->SetOrientation(90);
    y_axis->GetLabelProperties()->SetFontSize(12);
    chart_->Update();

   // view_->GetRenderWindow()->SetMultiSamples(0);
    view_->GetInteractor()->Initialize();
    view_->GetInteractor()->Start();

    ui->scrollArea->setWidget(qvtkWidget_);
}

parameterPlotSubWindow::~parameterPlotSubWindow()
{
    delete qvtkWidget_;
    delete ui;
}

bool parameterPlotSubWindow::connectToDataBase()
{
    this->db = QSqlDatabase::addDatabase("QSQLITE");

    // NOTE: We store the database in the tmp folder; should work under both
    // windows and linux
    QString path(QDir::tempPath().append(QDir::separator()));
    path.append(mdsSpaceName_);
    path.append(".sqlite");
    path = QDir::toNativeSeparators(path);

    db.setDatabaseName(path);

    if (!db.open())
    {
        qDebug() << "Failed to open database";
        return false;
    }
    else
    {
        qDebug() << "Read Parameters SQL";
        return true;
    }

}

QString parameterPlotSubWindow::queryDatabase(QString parameter,
                                              QString property)
{
    // Connect to our parameters database
    QSqlQuery query(db);

    // Find out which algorithm we are using (thus which table)
    QString algoName_ = mdsObject_->
            readPropertyAlgorithm(property);

    // Make sure we have read an algorithm
    if (algoName_ != "")
    {
        QString queryStr;
        queryStr.append("SELECT ");
        queryStr.append(parameter);
        queryStr.append(" FROM ");
        queryStr.append(algoName_);
        queryStr.append(" WHERE parameters$Property_Name$value = '");
        queryStr.append(property);
        queryStr.append("'");

        query.exec(queryStr);

        QString parameterValue;

        while (query.next())
        {
            parameterValue = query.value(0).toString();
        }

        return parameterValue;

    }
    else
    {
        std::cerr << "Warning: Property "
                  << property.toStdString()
                  << " does not appear to have an algorithm selected"
                  << std::endl;

        // If algorithm read fails, we return an empty string
        return QString();
    }
}
