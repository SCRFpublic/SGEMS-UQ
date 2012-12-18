#include "eda_response.h"
#include "chart_response.h"
#include <metric_utilities.h>

EDA_response::EDA_response(Chart_mdi_area* mdi_area, QWidget *parent) :
    EDA_chart(mdi_area,parent)
{
    
    QVBoxLayout* main_layout = new QVBoxLayout(this);
    main_layout->addWidget(new QLabel("Available Grids",this));
    gridListTreeView_ = new QTreeView(this);
    main_layout->addWidget(gridListTreeView_);
    
    main_layout->addWidget(new QLabel("Available Responses",this));    
    responseListTreeView_ = new QTreeView(this);
    main_layout->addWidget(responseListTreeView_);

    main_layout->addWidget(new QLabel("Available Realizations",this));
    propertyListTreeView_  = new QTreeView(this);
    main_layout->addWidget(propertyListTreeView_);

    QHBoxLayout* button_layout = new QHBoxLayout(this);
    QPushButton* clear_button = new QPushButton("Clear",this);
    QPushButton* show_button = new QPushButton("Display",this);
    button_layout->addWidget(clear_button);
    button_layout->addWidget(show_button);
    QWidget* button_holder = new QWidget(this);
    button_holder->setLayout(button_layout);
    main_layout->addWidget(button_holder);

    this->setLayout(main_layout);

    // Set up Tree-Views
    this->setGridTreeView();
    this->setResponseTreeView();


    SmartPtr<Named_interface> metricMan=
            Root::instance()->interface(metricData_manager);

    Manager* manager = dynamic_cast<Manager*>(metricMan.raw_ptr());

    connect(responseListTreeView_,SIGNAL(clicked(QModelIndex)),
            this,SLOT(updatePropertyExplorer(QModelIndex)));
    connect(responseListTreeView_,SIGNAL(activated(QModelIndex)),
            this,SLOT(updatePropertyExplorer(QModelIndex)));
    connect(responseListTreeView_,SIGNAL(pressed(QModelIndex)),
            this,SLOT(updatePropertyExplorer(QModelIndex)));
    connect(gridListTreeView_,SIGNAL(clicked(QModelIndex)),
            this,SLOT(updateGridSelection(QModelIndex)));

    connect( clear_button, SIGNAL(clicked()),
             gridListTreeView_->selectionModel(), SLOT(clearSelection ())   );
    connect( show_button, SIGNAL(clicked()),
             this, SLOT(show_responses())   );
}

EDA_response::~EDA_response()
{
}

void EDA_response::setGridTreeView()
{
    grid_proxy_model = new Filter_root_proxy_model(this);
    gridListTreeView_->setModel(grid_proxy_model);
    gridListTreeView_->setRootIndex(grid_proxy_model->project_root_index());
    gridListTreeView_->setItemsExpandable(false);
}

void EDA_response::updateGridSelection(QModelIndex gridIndex)
{
    // Get name of grid
    qDebug() << grid_proxy_model->data(
                    gridIndex,Qt::DisplayRole).
                toString();
    
    //    metricPlotsMdi->changeSelectedGrid(grid_proxy_model->data(
    //                                           gridIndex,Qt::DisplayRole).
    //                                           toString());
}

void EDA_response::setResponseTreeView()
{
    this->proxy_model = new Filter_root_proxy_model(this);
    responseListTreeView_->setModel(proxy_model);
    this->responseBaseIndex =
            ModelUtilities::response_root_index(this->proxy_model);
    responseListTreeView_->setRootIndex(this->responseBaseIndex);
    responseListTreeView_->setItemsExpandable(false);

    // Allow for objects to be dragged from list view into MDI area
    responseListTreeView_->setDragEnabled(true);
    responseListTreeView_->setDragDropMode(QAbstractItemView::DragOnly);

    // Allow for individual models to be dragged in as well
    propertyListTreeView_->setDragEnabled(true);
    propertyListTreeView_->setDragDropMode(QAbstractItemView::DragOnly);
    propertyListTreeView_->setSelectionMode(QAbstractItemView::MultiSelection);
}

void EDA_response::updatePropertyExplorer(QModelIndex responseIndex)
{
    // Create a sort filter
    QString responseName = this->proxy_model->data(responseIndex,
                                                   Qt::DisplayRole).toString();

    //    this->metricPlotsMdi->changedSelectedMetric(responseName);

    propertyListTreeView_->setModel(this->proxy_model);
    propertyListTreeView_->setRootIndex(
                ModelUtilities::response_property_root_index(
                    this->proxy_model,responseName));
}

void EDA_response::show_responses(){

    QModelIndexList grid_indexes =
            gridListTreeView_->selectionModel()->selectedIndexes();
    if(grid_indexes.isEmpty())
        return;

    QModelIndexList response_indexes =
            responseListTreeView_->selectionModel()->selectedIndexes();
    if(response_indexes.isEmpty())
        return;

    QModelIndexList property_indexes =
            propertyListTreeView_->selectionModel()->selectedIndexes();
    if(property_indexes.isEmpty())
        return;

    // Get pointer to metric registrar
    SmartPtr<Named_interface> ni = Root::instance()->
            interface(metricData_manager + "/metricRegistrar");

    MetricDataManager* mRegistrar =
            dynamic_cast<MetricDataManager*>(ni.raw_ptr());    


    // Obtain list of available metric names
    std::vector<std::string> availableMetricNames =
            mRegistrar->get_metric_names();

    // Flag to indicate if we are plotting an entire metric's properties
    bool plotFullMetric = false;

    QString grid_name = grid_indexes.at(0).data().toString();
    QString response_name = response_indexes.at(0).data().toString();
    QStringList prop_names;

    for(int i=0; i<property_indexes.size(); ++i)
    {
        prop_names.append(property_indexes.at(i).data().toString());
    }


    // Obtain pointer to metricToProperties map
    MetricDataToProperties  propFromMetricRegistrar =
            mRegistrar->get_properties_for_metric(response_name.toStdString());

    // Determine what kind of metricData we are looking at
    std::string response_type = propFromMetricRegistrar.
            data_type().toStdString();

    Chart_response* chart=0;
    if(response_type == "MetricScalarData")
    {
        chart = new Chart_response();
        chart->initialize(vtkChart::BAR,grid_name,response_name,prop_names);
    }
    else if(response_type == "MetricVectorData")
    {
        chart = new Chart_response();
        chart->initializeVector(vtkChart::BAR,grid_name,
                                response_name,prop_names);
    }
    else if(response_type == "MetricTimeSeriesData")
    {
        chart = new Chart_response();
        chart->initializeTimeSeries(vtkChart::BAR,grid_name,
                                    response_name,prop_names);
    }
    else if(response_type == "MetricContinuousDistributionData")
    {
        chart = new Chart_response();
        chart->initializeDistribution(vtkChart::BAR,grid_name,
                                      response_name,prop_names);
    }

    if(chart != 0) {
        QMdiSubWindow* sub_window = mdi_area_->addSubWindow(chart);
        sub_window->setAttribute( Qt::WA_DeleteOnClose );
        sub_window->show();
    }

}
