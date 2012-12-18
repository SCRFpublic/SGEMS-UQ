#include "mdsmetricplotmdisubwindow.h"

MDSMetricPlotMdiSubWindow::MDSMetricPlotMdiSubWindow(QWidget *parent) :
    QMdiSubWindow(parent), arrMetricValues_(0)
{

    qvtkWidget_ = new QVTKWidget(this);
    view_ = vtkSmartPointer<vtkContextView>::New();
    view_->SetInteractor(qvtkWidget_->GetInteractor());
    qvtkWidget_->SetRenderWindow(view_->GetRenderWindow());

    chart_ = vtkSmartPointer<vtkChartXY>::New();
    view_->GetScene()->AddItem(chart_);
    tableView_ = vtkSmartPointer<vtkQtTableView>::New();
    table_ = vtkSmartPointer<vtkTable>::New();
    table2_ = vtkSmartPointer<vtkTable>::New();

    QSizePolicy vtkPolicy = qvtkWidget_->sizePolicy();
    vtkPolicy.setHorizontalStretch(3);
    qvtkWidget_->setSizePolicy(vtkPolicy);
    QSizePolicy tablePolicy = tableView_->GetWidget()->sizePolicy();
    tablePolicy.setHorizontalStretch(1);
    tableView_->GetWidget()->setSizePolicy(tablePolicy);

    QSplitter* mainSplitter = new QSplitter(Qt::Horizontal, this);

    mainSplitter->addWidget(qvtkWidget_);
    mainSplitter->addWidget(tableView_->GetWidget());

    this->layout()->addWidget(mainSplitter);

    this->setAcceptDrops(true);


}

bool MDSMetricPlotMdiSubWindow::initialize(int plotType,
                                           QString metricName,
                                           QStringList metricProps )
{

    // Load up colors
    std::vector<int> red;
    std::vector<int> green;
    std::vector<int> blue;
    std::vector<int> alpha;

    red.push_back(100);   red.push_back(205);   red.push_back(112);
    green.push_back(149); green.push_back(175); green.push_back(138);
    blue.push_back(237);  blue.push_back(149);  blue.push_back(144);
    alpha.push_back(200); alpha.push_back(200); alpha.push_back(200);

    // Create side table and get Registrar
    SmartPtr<Named_interface> ni =
            Root::instance()->interface( metricData_manager +
                                         "/metricRegistrar");

    MetricDataManager* mRegistrar =
            dynamic_cast<MetricDataManager*>(ni.raw_ptr());

    arrId_ = vtkSmartPointer<vtkIntArray>::New();
    labels_ = vtkSmartPointer<vtkStringArray>::New();

    arrId_->SetName("Id");
    table_->AddColumn(arrId_);

    labels_->SetName("Property");
    table_->AddColumn(labels_);

    // Create a set of properties
    QSet<QString> propertySet;

    // loop through metric name, prop pairs
    int numMetrics = metricProps.size();
    arrMetricValues_ = new vtkSmartPointer<vtkFloatArray>[numMetrics];
    for (int i = 0; i < metricProps.size(); i++)
    {
        arrMetricValues_[i] = vtkSmartPointer<vtkFloatArray>::New();
        propertySet.insert(metricProps.at(i));
    }

    arrMetricValues_[0]->SetName(metricName.toStdString().c_str());
    table_->AddColumn(arrMetricValues_[0]);
    table_->SetNumberOfRows(numMetrics);
    QList<QString> sortedProperties = propertySet.toList();
    qSort(sortedProperties);
    for (int i = 0; i < sortedProperties.size(); i++) {
        labels_->SetValue(i,sortedProperties[i].toStdString().c_str());

        table_->SetValue(i, 0, i); // id
        table_->SetValue(i, 1, sortedProperties[i].toStdString().c_str());
        for (int j = 0; j < numMetrics; j++)
            table_->SetValue(i, 2 + j, 0);
    }

    for (unsigned int i = 0; i < numMetrics; i++)
    {
        int rowIndex = sortedProperties.indexOf(metricProps.at(i));
        MetricData* mdata = mRegistrar->getMetricData("simul-grid",
                                                      metricProps.at(i).
                                                      toStdString(),
                                                      metricName.toStdString());
        table_->SetValue( i,2,mdata->data(0) );
    }

    table_->Update();

    // add bars

    int ct = 0;
    if( plotType == vtkChart::BAR )
    {
        vtkPlot *plot = chart_->AddPlot(plotType);
        plot->SetInput(table_, 0, 2);
        plot->SetLabel(vtkStdString(metricName.toStdString().c_str()));
        plot->SetColor( red[ct],green[ct],blue[ct], alpha[ct] );

        if (numMetrics > 1)
            chart_->SetShowLegend(true);
        chart_->GetAxis(vtkAxis::BOTTOM)->SetTitle(vtkStdString("Property"));
        chart_->GetAxis(vtkAxis::LEFT)->SetTitle(vtkStdString("Metric value"));
    }

    tableView_->SetSplitMultiComponentColumns(true);
    tableView_->SetRepresentationFromInput(table_);
    tableView_->Update();

    return true;
}

bool MDSMetricPlotMdiSubWindow::initializeTimeSeries(int plotType,
                                                     QString metricName,
                                                     QStringList metricProps)
{
    // Load up colors
    std::vector<int> red;
    std::vector<int> green;
    std::vector<int> blue;
    std::vector<int> alpha;

    red.push_back(100);   red.push_back(205);   red.push_back(112);
    green.push_back(149); green.push_back(175); green.push_back(138);
    blue.push_back(237);  blue.push_back(149);  blue.push_back(144);
    alpha.push_back(200); alpha.push_back(200); alpha.push_back(200);

    // Create side table and get Registrar
    SmartPtr<Named_interface> ni =
            Root::instance()->interface( metricData_manager +
                                         "/metricRegistrar");

    MetricDataManager* mRegistrar =
            dynamic_cast<MetricDataManager*>(ni.raw_ptr());

    arrId_ = vtkSmartPointer<vtkIntArray>::New();
    labels_ = vtkSmartPointer<vtkStringArray>::New();
    time_ = vtkSmartPointer<vtkFloatArray>::New();
    time2_ = vtkSmartPointer<vtkFloatArray>::New();
    value_ = vtkSmartPointer<vtkFloatArray>::New();
    value2_ = vtkSmartPointer<vtkFloatArray>::New();

    // Set 4 Columns For ID, Label, Time and Data
    arrId_->SetName("Id");
    table_->AddColumn(arrId_);
    labels_->SetName("Property");
    table_->AddColumn(labels_);
    time_->SetName("Time");
    table_->AddColumn(time_);
    time2_->SetName("Time");
    table2_->AddColumn(time2_);

    // Create Set of Properties that we will be adding
    QSet<QString> propertySet;

    // Loop through metric name, prop pairs
    int numMetrics = metricProps.size(); // This is misleading; actually number of properties
    int numDataPoints;
    int currentNumberOfRows = 0;
    int prevNumberOfRows = 0;

//    // Create a floating point array for each metric
//    arrMetricValues_ = new vtkSmartPointer<vtkFloatArray>[numMetrics];

    for (int i = 0; i < metricProps.size(); i++)
    {
        // Put in name of each property
        propertySet.insert(metricProps.at(i));
    }

    // Sets the header for "Data", which is just the actual metric name
    value_->SetName(metricName.toStdString().c_str());
    value2_->SetName(metricName.toStdString().c_str());
    table_->AddColumn(value_);
    table2_->AddColumn(value2_);

    // Add a seperate column for storing rows
    table_->SetNumberOfRows(currentNumberOfRows);
    table2_->SetNumberOfRows(currentNumberOfRows);

    // Make a list of the properties name in string format we are plotting
    QList<QString> sortedProperties = propertySet.toList();
    qSort(sortedProperties);

    // For each property, we write the ID, path, time and response value
    for (int i = 0; i < sortedProperties.size(); i++)
    {
        // Read the response data structure for each property
        // TODO: make the grid name not hard coded
        MetricData* mdata = mRegistrar->getMetricData("grid",
                                                      metricProps.at(i).
                                                      toStdString(),
                                                      metricName.toStdString());

        // Should be a safe cast as this function is only called for data that
        // has form "TimeSeries"
        MetricTimeSeriesData *mDataTS =
                static_cast<MetricTimeSeriesData*>(mdata);

        // Get number of data points in this response for particular property
        // We assume that the data/time have same number of points
        numDataPoints = mDataTS->data().size();

        // We add enough rows to our table to represent all of data
        currentNumberOfRows += numDataPoints;
        this->table_->SetNumberOfRows(currentNumberOfRows);
        this->table2_->SetNumberOfRows(currentNumberOfRows);

        for (unsigned int j = 0; j < numDataPoints; ++j)
        {
            int tableIndex = prevNumberOfRows+j;
            // Write labels
            labels_->SetValue(tableIndex,
                              sortedProperties[i].toStdString().c_str());

            // Write ID
            table_->SetValue(tableIndex,0,i);

            // Write Time
            time_->SetValue(tableIndex,mDataTS->time(j));
            time2_->SetValue(tableIndex,mDataTS->time(j));

            // Write Value
            value_->SetValue(tableIndex,mDataTS->data(j));
            value2_->SetValue(tableIndex,mDataTS->data(j)*0.5);

        }

        prevNumberOfRows = currentNumberOfRows;
    }

    table_->Update();

    // Plot data
    vtkPlot *plot = chart_->AddPlot(vtkChart::LINE);
    plot->SetInput(table_,2,3);
    plot->SetColor(red[0],green[0],blue[0],alpha[0]);


    vtkPlot *plot2 = chart_->AddPlot(vtkChart::LINE);
    plot2->SetInput(table2_,0,1);
    plot2->SetColor(red[1],green[1],blue[1],alpha[1]);

    chart_->GetAxis(vtkAxis::BOTTOM)->SetTitle(vtkStdString("Time"));
    chart_->GetAxis(vtkAxis::LEFT)->SetTitle(vtkStdString(metricName.
                                                          toStdString().
                                                          c_str()));


    int ct = 0;
    //    if( plotType == vtkChart::BAR )
    //    {
    //        vtkPlot *plot = chart_->AddPlot(plotType);
    //        plot->SetInput(table_, 0, 2);
    //        plot->SetLabel(vtkStdString(metricName.toStdString().c_str()));
    //        plot->SetColor( red[ct],green[ct],blue[ct], alpha[ct] );

    //        if (numMetrics > 1)
    //            chart_->SetShowLegend(true);

    //    }

    //tableView_->SetSplitMultiComponentColumns(true);
    tableView_->SetRepresentationFromInput(table_);
    tableView_->Update();

    return true;
}
