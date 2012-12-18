#include "externalresponseinputfilter.h"

ExternalResponseInputFilter::ExternalResponseInputFilter()
{
}

void ExternalResponseInputFilter::exec()
{
    std::cout << "Executing External Response Input Filter" << std::endl;

    // First Parse The XML file
    QDomDocument doc("ExternalResponses");

    QFile file(QString::fromStdString(this->filename));

    if (!file.open(QIODevice::ReadOnly))
    {
        std::cerr << "Failed to open response file" << std::endl;
        return;
    }
    if (!doc.setContent(&file))
    {
        std::cerr << "Failed to parse response file" << std::endl;
        file.close();
        return;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName()!= "Responses")
    {
        std::cerr << "Failed to find root" << std::endl;
        return;

    }

    QDomNode n = root.firstChild();

    while (!n.isNull())
    {
        QDomElement e = n.toElement();

        if (!e.isNull())
        {
            if (e.tagName() == "Response")
            {
                // Check to make sure grid exists
                QString gridStr = e.attribute("grid");

                // Check to make sure property exists
                QString propStr = e.attribute("property");

                // Check to make sure name exists, it not create it
                QString nameStr = e.attribute("name");

                // Read type of data this response is storing
                QString typeStr = e.attribute("type");

                // If we have a time series, we store both time and values
                if (typeStr == "time-series")
                {
                    std::vector<float> time;
                    std::vector<float> value;

                    // Grab child node
                    QDomNode subNode = e.firstChild();

                    while (!subNode.isNull())
                    {
                        // Convert node to element
                        QDomElement subNodeElem = subNode.toElement();

                        // Read raw text
                        QString rawText = subNodeElem.text();

                        // Break raw text up into seperate values seperated by ;
                        istringstream seperatedString(rawText.toStdString());

                        while (seperatedString)
                        {
                            std::string s;
                            if (!getline(seperatedString,s,';'))
                                break;
                            else
                            {
                                // Convert from string to float
                                istringstream os(s);
                                float d;
                                os >> d;

                                if (subNodeElem.nodeName() == "Time")
                                {

                                    time.push_back(d);
                                }
                                else if (subNodeElem.nodeName() == "Value")
                                    value.push_back(d);
                                else
                                    break;
                            }
                        }
                        subNode = subNode.nextSibling();
                    }

                    // Ensure number of time and values are the same
                    if (time.size() != value.size())
                        std::cerr << "ERROR: time and values do not align"
                                  << std::endl;

                    // GENERATE NEW METRIC DATA HERE
                    // Need: Pointer to property
                    //       -QDomElement metaDataXml
                    // which has <algorithm name = "whatever">
                    // and       <Name value = "whatever">

                    // Find pointer to appropiate property
                    // Obtain smart point to current grid


                    SmartPtr<Named_interface> grid_ni =
                            Root::instance()->interface(gridModels_manager +
                                                        "/" +
                                                        gridStr.toStdString());

                    if (grid_ni.raw_ptr() == 0)
                    {
                        std::cerr << "Grid not found" << std::endl;
                        file.close();
                        return;

                    }



                    Geostat_grid* grid =
                            dynamic_cast<Geostat_grid*> (grid_ni.raw_ptr());

                    // Grab GsTLGridProperty from Grid
                    GsTLGridProperty* currentProperty =
                            grid->select_property(propStr.toStdString());

                    // Generate the required metaDataXml
                    QDomDocument doc("metaDataXml");
                    QDomElement metaDataXml = doc.createElement("metaRoot");
                    doc.appendChild(metaDataXml);

                    QDomElement algoXml = doc.createElement("algorithm");
                    algoXml.setAttribute("name",nameStr);
                    metaDataXml.appendChild(algoXml);

                    QDomElement nameXml = doc.createElement("Name");
                    nameXml.setAttribute("value",nameStr);
                    metaDataXml.appendChild(nameXml);

                    SmartPtr<Named_interface> ni =
                            Root::instance()->interface(
                                metricData_manager + "/metricRegistrar");

                    MetricDataManager* mDataRegistrar =
                            dynamic_cast<MetricDataManager*>(ni.raw_ptr());

                    MetricTimeSeriesData *metric = new
                            MetricTimeSeriesData(currentProperty,
                                                 metaDataXml,value,time);

                    mDataRegistrar->registerMetricData(gridStr.toStdString(),
                                                       propStr.toStdString(),
                                                       metric );

                    // Move onto next response
                    time.clear();
                    value.clear();
                }

                else if (typeStr == "vector")
                {
                    std::vector<float> value;

                    // Grab child node
                    QDomNode subNode = e.firstChild();

                    while (!subNode.isNull())
                    {
                        // Convert node to element
                        QDomElement subNodeElem = subNode.toElement();

                        // Read raw text
                        QString rawText = subNodeElem.text();

                        // Break raw text up into seperate values seperated by ;
                        istringstream seperatedString(rawText.toStdString());

                        while (seperatedString)
                        {
                            std::string s;
                            if (!getline(seperatedString,s,';'))
                                break;
                            else
                            {
                                // Convert from string to float
                                istringstream os(s);
                                float d;
                                os >> d;

                                if (subNodeElem.nodeName() == "Value")
                                    value.push_back(d);
                                else
                                    break;
                            }
                        }
                        subNode = subNode.nextSibling();
                    }


                    // GENERATE NEW METRIC DATA HERE
                    // Need: Pointer to property
                    //       -QDomElement metaDataXml
                    // which has <algorithm name = "whatever">
                    // and       <Name value = "whatever">

                    // Find pointer to appropiate property
                    // Obtain smart point to current grid


                    SmartPtr<Named_interface> grid_ni =
                            Root::instance()->interface(gridModels_manager +
                                                        "/" +
                                                        gridStr.toStdString());

                    if (grid_ni.raw_ptr() == 0)
                    {
                        std::cerr << "Grid not found" << std::endl;
                        file.close();
                        return;

                    }



                    Geostat_grid* grid =
                            dynamic_cast<Geostat_grid*> (grid_ni.raw_ptr());

                    // Grab GsTLGridProperty from Grid
                    GsTLGridProperty* currentProperty =
                            grid->select_property(propStr.toStdString());

                    // Generate the required metaDataXml
                    QDomDocument doc("metaDataXml");
                    QDomElement metaDataXml = doc.createElement("metaRoot");
                    doc.appendChild(metaDataXml);

                    QDomElement algoXml = doc.createElement("algorithm");
                    algoXml.setAttribute("name",nameStr);
                    metaDataXml.appendChild(algoXml);

                    QDomElement nameXml = doc.createElement("Name");
                    nameXml.setAttribute("value",nameStr);
                    metaDataXml.appendChild(nameXml);

                    SmartPtr<Named_interface> ni =
                            Root::instance()->interface(
                                metricData_manager + "/metricRegistrar");

                    MetricDataManager* mDataRegistrar =
                            dynamic_cast<MetricDataManager*>(ni.raw_ptr());

                    MetricVectorData *metric = new
                            MetricVectorData(currentProperty,
                                                 metaDataXml,value);

                    mDataRegistrar->registerMetricData(gridStr.toStdString(),
                                                       propStr.toStdString(),
                                                       metric );

                    // Move onto next response

                    value.clear();

                }
            }
        }

        n = n.nextSibling();
    }

    file.close();
}
