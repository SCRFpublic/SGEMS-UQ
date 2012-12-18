#include "metricplotsmdiarea.h"

MetricPlotsMdiArea::MetricPlotsMdiArea(QWidget *parent, QString gridName) :
    QMdiArea(parent), selectedGridName(gridName)
{
    this->setAcceptDrops(true);

    // Create a new proxy model that we will use to look up values
    this->proxy_model = new Filter_root_proxy_model(this);

}

MetricPlotsMdiArea::~MetricPlotsMdiArea()
{

}

void MetricPlotsMdiArea::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
    {
        event->acceptProposedAction();
    }
}

void MetricPlotsMdiArea::dropEvent(QDropEvent *event)
{

    // Read mime data from drag event
    QByteArray encoded = event->mimeData()
            ->data("application/x-qabstractitemmodeldatalist");
    QDataStream stream(&encoded, QIODevice::ReadOnly);


    // Parse the names of the properties/metrics we are trying to plot
    QStringList objectNames;
    while (!stream.atEnd())
    {
        int row, col;
        QMap<int,  QVariant> roleDataMap;
        stream >> row >> col >> roleDataMap;

        objectNames.append(roleDataMap.value(0).toString());
    }

    this->addCharts(objectNames, this->selectedGridName);

    event->acceptProposedAction();
}

void MetricPlotsMdiArea::addCharts(QStringList propertiesToBeAdded,
                                   QString gridName)
{

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

    // Case 0: Empty string list; just return
    if (propertiesToBeAdded.size() < 1)
        return;
    else if (propertiesToBeAdded.size() == 1)
    {
        // Check if we are plotting all properties in metric
        std::vector<std::string>::iterator it;

        for(it = availableMetricNames.begin(); it!= availableMetricNames.end();
            it++)
        {
            if (*it == propertiesToBeAdded.at(0).toStdString())
            {
                plotFullMetric = true;
                break;
            }
        }
    }

    // Obtain pointer to metricToProperties map
    MetricDataToProperties  propFromMetricRegistrar =
            mRegistrar->get_properties_for_metric(
                this->selectedMetricName.toStdString());

    // Determine what kind of metricData we are looking at

    if (propFromMetricRegistrar.data_type().toStdString()
            == "MetricScalarData")
        this->metricDataType = 0;
    else if (propFromMetricRegistrar.data_type().toStdString()
             == "MetricVectorData")
        this->metricDataType = 1;
    else if (propFromMetricRegistrar.data_type().toStdString()
             == "MetricTimeSeriesData")
        this->metricDataType = 2;
    else if (propFromMetricRegistrar.data_type().toStdString()
             == "MetricContinuousDistributionData")
        this->metricDataType = 3;
    else
        this->metricDataType = -1;

    QStringList metricProps;

    if (plotFullMetric)
    {
        // Generate String List for all properties
        std::vector<std::string> prop_names;
        propFromMetricRegistrar.getPropertyNames(this->selectedGridName.
                                                 toStdString(),
                                                 prop_names);

        for (unsigned int i = 0; i < prop_names.size(); i++)
        {
            metricProps.append(QString::fromStdString(prop_names.at(i)));
        }
    }
    else
        metricProps = propertiesToBeAdded;

    if (this->metricDataType == 0)
    {
        MDSMetricPlotSubWindow *subwindow =
                new MDSMetricPlotSubWindow(this);

        bool ok = subwindow->initialize(vtkChart::BAR, gridName,
                                        this->selectedMetricName,metricProps);

        if (ok)
        {
            subwindow->show();
        }
    }
    else if (this->metricDataType == 1)
    {
        MDSMetricPlotSubWindow *subwindow =
                new MDSMetricPlotSubWindow(this);

        bool ok = subwindow->initializeVector(vtkChart::BAR,
                                              gridName,
                                              this->selectedMetricName,
                                              metricProps);

        if (ok)
        {
            subwindow->show();
        }
    }
    else if (this->metricDataType == 2)
    {
        MDSMetricPlotSubWindow *subwindow =
                new MDSMetricPlotSubWindow(this);

        bool ok = subwindow->initializeTimeSeries(vtkChart::BAR, gridName,
                                                  this->selectedMetricName,
                                                  metricProps);

        if (ok)
        {
            subwindow->show();
        }
    }

    else if (this->metricDataType == 3)
    {
        MDSMetricPlotSubWindow *subwindow =
                new MDSMetricPlotSubWindow(this);

        bool ok = subwindow->initializeDistribution(vtkChart::BAR,gridName,
                                                    this->selectedMetricName,
                                                    metricProps);
        if (ok)
        {
            subwindow->show();
        }

    }

}

void MetricPlotsMdiArea::changedSelectedMetric(QString name)
{
    this->selectedMetricName = name;
}

void MetricPlotsMdiArea::changeSelectedGrid(QString name)
{
    selectedGridName = name;
}
