#include "sharedparametersubwindow.h"
//#include "ui_sharedparametersubwindow.h"
#include "multi_dim_scaling_space.h"

#include <QVBoxLayout>

/*
sharedParameterSubwindow::sharedParameterSubwindow(QWidget *parent,
                                                   QString mdsObject,
                                                   QStringList properties,
                                                   QString parameter) :
    QWidget(parent),
    ui(new Ui::sharedParameterSubwindow),mdsSpaceName_(mdsObject), parameterName_(parameter)
*/
sharedParameterSubwindow::sharedParameterSubwindow(QWidget *parent,
                                                   QString mdsObject,
                                                   QStringList properties,
                                                   QString parameter) :
    QWidget(parent),
    mdsSpaceName_(mdsObject), parameterName_(parameter)
{
  //  ui->setupUi(this);
  this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
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
    view_ = vtkSmartPointer<vtkContextView>::New();
    view_->SetInteractor(qvtkWidget_->GetInteractor());
    qvtkWidget_->SetRenderWindow(view_->GetRenderWindow());
    chart_ = vtkSmartPointer<vtkChartXY>::New();
    view_->GetScene()->AddItem(chart_);
    QSizePolicy vtkPolicy = qvtkWidget_->sizePolicy();
//    vtkPolicy.setHorizontalStretch(3);
//    qvtkWidget_->setSizePolicy(vtkPolicy);

    QString currentParameter = parameter;
    QStringList currentProperties = properties;

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(qvtkWidget_);
    this->setLayout(layout);


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


    // Write data
    // we build a hash table with the key being
    // the value of the parameter and each time we see it,
    // we increment by 1
    for (unsigned int i = 0; i < currentProperties.size(); ++i)
    {
        QString currentValue = queryDatabase(currentParameter,
                                           currentProperties.at(i));

        // See if hash table contains it
        if (data_.contains(currentValue))
        {
            data_[currentValue] += 1;
        }
        // If not we put it in
        else
        {
            data_[currentValue] = 0;

        }
    }

    int numUniqueParam = 0;

    table_->SetNumberOfRows(data_.size());
    QHash<QString, int>::iterator it = data_.begin();
    for (; it != data_.end(); it++)
    {
        // Set ID
        table_->SetValue(numUniqueParam,0,double(numUniqueParam));

        // Set Label
        table_->SetValue(numUniqueParam,1,it.key().toStdString().c_str());

        // Set Value
        table_->SetValue(numUniqueParam,2,it.value()+1.0f);
        numUniqueParam++;
    }
    // Add multiple line plots, setting the colors etc

    QStringList params = currentParameter.split("$");

    vtkPlot *line = 0;
    line = chart_->AddPlot(vtkChart::BAR);
    line->SetInput(table_, 0, 2);

    line->SetColor(67,	110	,238, 255);

    vtkAxis *x_axis = chart_->GetAxis(vtkAxis::BOTTOM);
    vtkAxis *y_axis = chart_->GetAxis(vtkAxis::LEFT);

    y_axis->SetMaximum(currentProperties.size()+3);
    y_axis->SetMinimum(-1);
    y_axis->SetTitle("Frequency");
    x_axis->SetBehavior(1);
    x_axis->SetMinimum(-1);
    x_axis->SetMaximum(data_.size()+1);
    x_axis->SetTitle(params.back().toStdString());

    x_axis->SetTickPositions(arrId_);
    x_axis->SetTickLabels(labels_);
 //   x_axis->GetLabelProperties()->SetVerticalJustification(VTK_TEXT_CENTERED);
 //   x_axis->GetLabelProperties()->SetJustification(VTK_TEXT_CENTERED);
    x_axis->GetLabelProperties()->SetOrientation(90);
    x_axis->GetLabelProperties()->SetVerticalJustification(VTK_TEXT_CENTERED);
    x_axis->GetLabelProperties()->SetJustification(VTK_TEXT_RIGHT);
    //chart_->SetTitle(params.join("\\").toStdString());
    chart_->Update();

 //   view_->GetRenderWindow()->SetMultiSamples(0);
 //   view_->GetInteractor()->Initialize();
 //   view_->GetInteractor()->Start();

 //   ui->scrollArea->setWidget(qvtkWidget_);
}

sharedParameterSubwindow::~sharedParameterSubwindow()
{
  delete qvtkWidget_;
//    delete ui;
}

bool sharedParameterSubwindow::connectToDataBase()
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
        qDebug() << "Shared Parameter Window Successfully connected";
        return true;
    }

}

QString sharedParameterSubwindow::queryDatabase(QString parameter,
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
