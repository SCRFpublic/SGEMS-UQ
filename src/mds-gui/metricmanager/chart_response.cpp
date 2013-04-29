#include "chart_response.h"
#include "ui_mdsmetricplotsubwindow.h"
#include <vtkPlotPoints.h>
#include <vtkAxis.h>
#include <vtkTextProperty.h>

Chart_response::Chart_response(QWidget *parent) :
    Chart_base(parent),arrMetricValues_(0),
    ui(new Ui::MDSMetricPlotSubWindow)
{
    ui->setupUi(this);

    qvtkWidget_ = new QVTKWidget(this);
    context_view_ = vtkSmartPointer<vtkContextView>::New();
    context_view_->SetInteractor(qvtkWidget_->GetInteractor());
    qvtkWidget_->SetRenderWindow(context_view_->GetRenderWindow());

    chart_ = vtkSmartPointer<vtkChartXY>::New();
    context_view_->GetScene()->AddItem(chart_);

    QSizePolicy vtkPolicy = qvtkWidget_->sizePolicy();
    vtkPolicy.setHorizontalStretch(3);
    qvtkWidget_->setSizePolicy(vtkPolicy);

    tableView_ = vtkSmartPointer<vtkQtTableView>::New();

//HACK
//    QSizePolicy tablePolicy = tableView_->GetWidget()->sizePolicy();
//    tableView_->GetWidget()->setSizePolicy(tablePolicy);

    this->ui->vtkPlotArea->setWidget(qvtkWidget_);
    //this->ui->tabWidget->setHidden(true);



}

bool Chart_response::initialize(int plotType,
                                        QString gridName,
                                        QString metricName,
                                        QStringList metricProps)
{

    if(metricProps.empty()) return false; 
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

    // Create Title
    this->setWindowTitle(metricName);

    // Copy property names
    this->propertyList = metricProps;

    // Copy grid name
    this->gridName = gridName;

    arrId_ = vtkSmartPointer<vtkDoubleArray>::New();
    labels_ = vtkSmartPointer<vtkStringArray>::New();

    table_ = vtkSmartPointer<vtkTable>::New();

    arrId_->SetName("Id");
    table_->AddColumn(arrId_);

    labels_->SetName("Realization");
    table_->AddColumn(labels_);

    // Create a set of properties
    QSet<QString> propertySet;

    // loop through metric name, prop pairs
    int numMetrics = metricProps.size();
    this->arrMetricValues_ = new vtkSmartPointer<vtkFloatArray>[numMetrics];
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
        MetricData* mdata = mRegistrar->getMetricData(gridName.toStdString(),
                                                      metricProps.at(i).
                                                      toStdString(),
                                                      metricName.toStdString());
        if(mdata==0) continue;
        table_->SetValue( i,2,mdata->data(0) );
    }

   // table_->Update();

    // add bars

    int ct = 0;
    if( plotType == vtkChart::BAR )
    {
        vtkPlot *plot = chart_->AddPlot(plotType);
        plot->SetInputData(table_, 0, 2);
        plot->SetLabel(vtkStdString(metricName.toStdString().c_str()));
        plot->SetColor( red[ct],green[ct],blue[ct], alpha[ct] );

        //        if (numMetrics > 1)
        //            chart_->SetShowLegend(true);
        chart_->GetAxis(vtkAxis::BOTTOM)->SetTitle(vtkStdString("Realizations"));


        vtkAxis *x_axis = chart_->GetAxis(vtkAxis::BOTTOM);
        x_axis->SetBehavior(1);
        x_axis->SetMinimum(-1);
        x_axis->SetMaximum(numMetrics);
        x_axis->SetTitle("Realizations");


        x_axis->SetTickPositions(arrId_);
        x_axis->SetTickLabels(labels_);
        x_axis->GetLabelProperties()->SetVerticalJustification(VTK_TEXT_CENTERED);
        x_axis->GetLabelProperties()->SetJustification(VTK_TEXT_CENTERED);
        x_axis->GetLabelProperties()->SetOrientation(90);
        x_axis->GetLabelProperties()->SetLineOffset(100);
        x_axis->GetLabelProperties()->SetFontSize(16);
    }

    tableView_->SetSplitMultiComponentColumns(true);
    tableView_->SetRepresentationFromInput(table_);
    tableView_->Update();
    this->ui->scrollArea->setWidget(tableView_->GetWidget());

    return true;

}

bool Chart_response::initializeVector(int plotType,
                                              QString gridName,
                                              QString metricName,
                                              QStringList metricProps)
{
    // Create side table and get Registrar
    SmartPtr<Named_interface> ni =
            Root::instance()->interface( metricData_manager +
                                         "/metricRegistrar");

    MetricDataManager* mRegistrar =
            dynamic_cast<MetricDataManager*>(ni.raw_ptr());

    // Create Title
    this->setWindowTitle(metricName);

    // Copy property names
    this->propertyList = metricProps;

    // Copy grid name
    this->gridName = gridName;

    // Create Response Display Table (On the left)
    this->responseQtTable = new MDSResponseItemTable(0,metricProps.size(),3);
    this->responseQtTable->fillTable(metricProps);

    connect(this->responseQtTable,SIGNAL(visibilityModified(QString,bool)),
            this,SLOT(updatePlotVisibility(QString,bool)));
    connect(this->responseQtTable,SIGNAL(colorModified(QString,QColor)),
            this,SLOT(updatePlotColor(QString,QColor)));

    // Create a table for each set of data
    QStringList masterColorList = QColor::colorNames();
    for (unsigned int i = 0; i<metricProps.size(); ++i)
    {
        // Read Metric Data
        MetricData* mdata = mRegistrar->getMetricData(this->gridName.
                                                      toStdString(),
                                                      metricProps.at(i).
                                                      toStdString(),
                                                      metricName.toStdString());

        // Should be a safe cast as this function is only called for data that
        // has form "TimeSeries"
        MetricVectorData *mDataV =
                static_cast<MetricVectorData*>(mdata);

        // Get number of data points in this response for particular property
        // We assume that the data/time have same number of points
        int numDataPoints = mDataV->size();

        // Set Combo list for viewing data
        //this->ui->comboBox->addItem(metricProps.at(i));

        // Generate VTKTable for this property
        vtkSmartPointer<vtkTable> responseTable_ =
                vtkSmartPointer<vtkTable>::New();

        vtkSmartPointer<vtkFloatArray> time_ =
                vtkSmartPointer<vtkFloatArray>::New();

        vtkSmartPointer<vtkFloatArray> value_ =
                vtkSmartPointer<vtkFloatArray>::New();

        time_->SetName("X Axis");
        responseTable_->AddColumn(time_);

        value_->SetName(metricName.toStdString().c_str());
        responseTable_->AddColumn(value_);

        responseTable_->SetNumberOfRows(numDataPoints);
        time_->SetNumberOfValues(numDataPoints);
        value_->SetNumberOfValues(numDataPoints);

        // Copy Data To VTK float arrays
        for (unsigned int j = 0; j < numDataPoints; ++j)
        {
            time_->SetValue(j,j);
            value_->SetValue(j,mDataV->data(j));
        }



        // Create a plot for each table
        Response_plot_data responseVTKData;
        responseVTKData.table_ = responseTable_;
        responseVTKData.plot_ = this->chart_->AddPlot(vtkChart::POINTS);
        responseVTKData.plot_->SetInputData(responseVTKData.table_,0,1);

        int colorIndex = (i+15) % masterColorList.size();
        QColor defaultColor = QColor(masterColorList.at(colorIndex));

        responseVTKData.plot_->SetColor(defaultColor.red(),
                                        defaultColor.blue(),
                                        defaultColor.green(),
                                        defaultColor.alpha());

        // Add multiple scatter plots, setting the colors etc
        vtkPlotPoints::SafeDownCast(responseVTKData.plot_)->SetMarkerStyle(vtkPlotPoints::CIRCLE);
        vtkPlotPoints::SafeDownCast(responseVTKData.plot_)->SetMarkerSize(12.0);

        // Put response table onto hash table
        this->responseHashTable.insert(metricProps.at(i),responseVTKData);
    }

    // Set Axis Labels
    chart_->GetAxis(vtkAxis::BOTTOM)->SetTitle(vtkStdString("X-Axis"));
    chart_->GetAxis(vtkAxis::LEFT)->SetTitle(vtkStdString(metricName.
                                                          toStdString().
                                                          c_str()));


    // Display
    //this->ui->dataScrollArea->setWidget(tableView_->GetWidget());
    this->ui->scrollArea->setWidget(this->responseQtTable);

    // Set combo box
    //connect(this->ui->comboBox,SIGNAL(currentIndexChanged(QString)),
            //this,SLOT(changeDataTableView(QString)));

    //this->ui->comboBox->setCurrentIndex(0);
    this->changeDataTableView(metricProps.at(0));

    return true;


}

bool Chart_response::initializeTimeSeries(int plotType,
                                                  QString gridName,
                                                  QString metricName,
                                                  QStringList metricProps)
{

    // Create side table and get Registrar
    SmartPtr<Named_interface> ni =
            Root::instance()->interface( metricData_manager +
                                         "/metricRegistrar");

    MetricDataManager* mRegistrar =
            dynamic_cast<MetricDataManager*>(ni.raw_ptr());

    // Create Title
    this->setWindowTitle(metricName);

    // Copy property names
    this->propertyList = metricProps;

    // Copy grid name
    this->gridName = gridName;

    // Create Response Display Table (On the left)
    this->responseQtTable = new MDSResponseItemTable(0,metricProps.size(),3);
    this->responseQtTable->fillTable(metricProps);

    connect(this->responseQtTable,SIGNAL(visibilityModified(QString,bool)),
            this,SLOT(updatePlotVisibility(QString,bool)));
    connect(this->responseQtTable,SIGNAL(colorModified(QString,QColor)),
            this,SLOT(updatePlotColor(QString,QColor)));

    // Create a table for each set of data
    QStringList masterColorList = QColor::colorNames();
    for (unsigned int i = 0; i<metricProps.size(); ++i)
    {
        // Read Metric Data
        MetricData* mdata = mRegistrar->getMetricData(this->gridName.
                                                      toStdString(),
                                                      metricProps.at(i).
                                                      toStdString(),
                                                      metricName.toStdString());

        // Should be a safe cast as this function is only called for data that
        // has form "TimeSeries"
        MetricTimeSeriesData *mDataTS =
                static_cast<MetricTimeSeriesData*>(mdata);

        // Get number of data points in this response for particular property
        // We assume that the data/time have same number of points
        int numDataPoints = mDataTS->data().size();

        // Set Combo list for viewing data
        //this->ui->comboBox->addItem(metricProps.at(i));

        // Generate VTKTable for this property
        vtkSmartPointer<vtkTable> responseTable_ =
                vtkSmartPointer<vtkTable>::New();

        vtkSmartPointer<vtkFloatArray> time_ =
                vtkSmartPointer<vtkFloatArray>::New();

        vtkSmartPointer<vtkFloatArray> value_ =
                vtkSmartPointer<vtkFloatArray>::New();

        time_->SetName("Time");
        responseTable_->AddColumn(time_);

        value_->SetName(metricName.toStdString().c_str());
        responseTable_->AddColumn(value_);

        responseTable_->SetNumberOfRows(numDataPoints);
        time_->SetNumberOfValues(numDataPoints);
        value_->SetNumberOfValues(numDataPoints);

        // Copy Data To VTK float arrays
        for (unsigned int j = 0; j < numDataPoints; ++j)
        {
            time_->SetValue(j,mDataTS->time(j));
            value_->SetValue(j,mDataTS->data(j));
        }

        // Create a plot for each table
        Response_plot_data responseVTKData;
        responseVTKData.table_ = responseTable_;
        responseVTKData.plot_ = this->chart_->AddPlot(vtkChart::LINE);
        responseVTKData.plot_->SetInputData(responseVTKData.table_,0,1);

        int colorIndex = (i+15) % masterColorList.size();
        QColor defaultColor = QColor(masterColorList.at(colorIndex));

        responseVTKData.plot_->SetColor(defaultColor.red(),
                                        defaultColor.blue(),
                                        defaultColor.green(),
                                        defaultColor.alpha());

        // Put response table onto hash table
        this->responseHashTable.insert(metricProps.at(i),responseVTKData);
    }

    // Set Axis Labels
    chart_->GetAxis(vtkAxis::BOTTOM)->SetTitle(vtkStdString("Time"));
    chart_->GetAxis(vtkAxis::BOTTOM)->GetTitleProperties()->SetFontSize(18);
    chart_->GetAxis(vtkAxis::LEFT)->GetLabelProperties()->SetFontSize(14);
    chart_->GetAxis(vtkAxis::LEFT)->GetTitleProperties()->SetFontSize(18);
    chart_->GetAxis(vtkAxis::BOTTOM)->GetLabelProperties()->SetFontSize(14);
    chart_->GetAxis(vtkAxis::LEFT)->SetTitle(vtkStdString(metricName.
                                                          toStdString().
                                                          c_str()));

    // Display
    //this->ui->dataScrollArea->setWidget(tableView_->GetWidget());
    this->ui->scrollArea->setWidget(this->responseQtTable);

    // Set combo box
    //connect(this->ui->comboBox,SIGNAL(currentIndexChanged(QString)),
            //this,SLOT(changeDataTableView(QString)));

    //this->ui->comboBox->setCurrentIndex(0);
    this->changeDataTableView(metricProps.at(0));

    return true;

}

bool Chart_response::initializeDistribution(int plotType,
                                                    QString gridName,
                                                    QString metricName,
                                                    QStringList metricProps)
{
    // Create side table and get Registrar
    SmartPtr<Named_interface> ni =
            Root::instance()->interface( metricData_manager +
                                         "/metricRegistrar");

    MetricDataManager* mRegistrar =
            dynamic_cast<MetricDataManager*>(ni.raw_ptr());

    // Create Title
    this->setWindowTitle(metricName);

    // Copy property names
    this->propertyList = metricProps;

    // Copy grid name
    this->gridName = gridName;

    // Create Response Display Table (On the left)
    this->responseQtTable = new MDSResponseItemTable(0,metricProps.size(),3);
    this->responseQtTable->fillTable(metricProps);

    connect(this->responseQtTable,SIGNAL(visibilityModified(QString,bool)),
            this,SLOT(updatePlotVisibility(QString,bool)));
    connect(this->responseQtTable,SIGNAL(colorModified(QString,QColor)),
            this,SLOT(updatePlotColor(QString,QColor)));

    // Create a table for each set of data
    QStringList masterColorList = QColor::colorNames();
    for (unsigned int i = 0; i<metricProps.size(); ++i)
    {
        // Read Metric Data
        MetricData* mdata = mRegistrar->getMetricData(this->gridName.
                                                      toStdString(),
                                                      metricProps.at(i).
                                                      toStdString(),
                                                      metricName.toStdString());

        // Should be a safe cast as this function is only called for data that
        // has form "TimeSeries"
        MetricContinuousDistributionData *mData =
                static_cast<MetricContinuousDistributionData*>(mdata);

        // Unlike the other metrics, we will generate a fix number of points
        int numDataPoints = 100;

        // Generate VTKTable for this property
        vtkSmartPointer<vtkTable> responseTable_ =
                vtkSmartPointer<vtkTable>::New();

        vtkSmartPointer<vtkFloatArray> time_ =
                vtkSmartPointer<vtkFloatArray>::New();

        vtkSmartPointer<vtkFloatArray> value_ =
                vtkSmartPointer<vtkFloatArray>::New();

        time_->SetName("X-axis");
        responseTable_->AddColumn(time_);

        value_->SetName(metricName.toStdString().c_str());
        responseTable_->AddColumn(value_);

        responseTable_->SetNumberOfRows(numDataPoints);
        time_->SetNumberOfValues(numDataPoints);
        value_->SetNumberOfValues(numDataPoints);

        double zmin = mData->distribution()->inverse(0.00001);
        double zmax = mData->distribution()->inverse(0.99999);

        // Compute the increment we need for numDataPoints
        double zIncrement = (zmax-zmin)/numDataPoints;

        for (unsigned int j = 0; j < numDataPoints; ++j)
        {
            double z = zmin+zIncrement*j;
            time_->SetValue(j,z);
            value_->SetValue(j,mData->distribution()->pdf(z));
        }

        // Create a plot for each table
        Response_plot_data responseVTKData;
        responseVTKData.table_ = responseTable_;
        responseVTKData.plot_ = this->chart_->AddPlot(vtkChart::LINE);
        responseVTKData.plot_->SetInputData(responseVTKData.table_,0,1);

        int colorIndex = (i+15) % masterColorList.size();
        QColor defaultColor = QColor(masterColorList.at(colorIndex));

        responseVTKData.plot_->SetColor(defaultColor.red(),
                                        defaultColor.blue(),
                                        defaultColor.green(),
                                        defaultColor.alpha());

        // Put response table onto hash table
        this->responseHashTable.insert(metricProps.at(i),responseVTKData);
    }

    // Set Axis Labels
    chart_->GetAxis(vtkAxis::BOTTOM)->SetTitle(vtkStdString("Quantities"));
    chart_->GetAxis(vtkAxis::LEFT)->SetTitle(vtkStdString("PDF"));

    // Display
    //this->ui->dataScrollArea->setWidget(tableView_->GetWidget());
    this->ui->scrollArea->setWidget(this->responseQtTable);

    return true;

}

Chart_response::~Chart_response()
{
    delete ui;
}

void Chart_response::updatePlotVisibility(QString property,
                                                  bool visible)
{
    // Delete Plot
    if (!visible)
    {
        bool removeSuccess =
                this->chart_->RemovePlotInstance(
                    this->responseHashTable[property].plot_);
    }
    else if (visible)
    {
        vtkIdType plotID =
                this->chart_->AddPlot(this->responseHashTable[property].plot_);

    }
}

void Chart_response::updatePlotColor(QString property,
                                             QColor newColor)
{
    int r = newColor.red();
    int b = newColor.blue();
    int g = newColor.green();
    int a = newColor.alpha();

    bool removeSuccess =
            this->chart_->RemovePlotInstance(
                this->responseHashTable[property].plot_);

    this->responseHashTable[property].plot_->SetColor(r,g,b,a);

    if (removeSuccess)
    {
        vtkIdType plotID =
                this->chart_->AddPlot(this->responseHashTable[property].plot_);
    }
}

void Chart_response::changeDataTableView(QString property)
{
    tableView_->SetSplitMultiComponentColumns(true);
    tableView_->SetRepresentationFromInput(
                this->responseHashTable[property].table_);
    tableView_->Update();
}

