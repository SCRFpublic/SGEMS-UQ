/*
 * multi_dim_scaling_space.cpp
 *
 *  Created on: Apr 28, 2011
 *      Author: aboucher
 */

#include "multi_dim_scaling_space.h"
#include "mds.h"
#include "kmeans.h" // kernel k means clustering
#include "metric_manager_repository.h"

#include <GsTL/utils/smartptr.h>
#include <appli/manager_repository.h>

//#include <vtkFloatArray.h>

#include <QDomElement>
#include <QSqlError>

Named_interface* MultiDimScalingSpace::create_new_interface(std::string &){
    return new MultiDimScalingSpace;
}

bool MultiDimScalingSpace::initialize(std::string name,
                                      std::vector< std::pair< std::string,
                                      std::string> > props,
                                      std::vector<std::string> metricNames)
{
    name_= name;
    properties_ = props;
    metricNames_ =metricNames;
    nProps_ = properties_.size();

    // Generate a new database here in respective folder
    this->createSQLDatabase();

    // Clear parameter vector
    this->propertyParameters.clear();

    // Build the document for the member properties
    std::vector< grid_prop_pairT >::const_iterator it = properties_.begin();

    for( ; it != properties_.end() ; ++it)
    {
        QString gridName = QString::fromStdString(it->first);
        std::string gridNameStr = gridName.toStdString();

        // Obtain smart point to current grid
        SmartPtr<Named_interface> grid_ni = Root::instance()->interface(
                    gridModels_manager + "/" + gridNameStr);
        Geostat_grid* grid = dynamic_cast<Geostat_grid*> (grid_ni.raw_ptr());

        // Obtain name of the property itself
        QString propName = QString::fromStdString(it->second);
        std::string propNameStr = propName.toStdString();

        // Grab Grid_continuous_property from Grid
        Grid_continuous_property* currentProperty = grid->select_property(propNameStr);
        this->propertyParameters.push_back(currentProperty->parameters());
        props_.push_back(currentProperty);

        // Create a QDomDocument containing the parameters for each property
        QDomDocument currentParameters;
        currentParameters.setContent(QString::fromStdString(
                                         currentProperty->parameters()));

        this->addData(currentParameters,propName);

        QDomElement elem = doc_.firstChildElement( gridName );

        if(elem.isNull())
        {
            elem = doc_.createElement(gridName);
            doc_.appendChild(elem);
        }

        elem = doc_.createElement( QString::fromStdString( it->second ) );

        doc_.appendChild(elem);
    }

    // Read parameters used to generate properties
    this->getMetricDataCollections();

    this->computeDistanceMetricSpace();
    this->computeMDS();
    this->computeDistanceMDSSpace();

    //Build the gstl_object_item for the
    metric_property_items_ = new Metric_space_properties(this);
    metric_response_items_ = new Metric_space_responses(this);

    this->idCount = 0;
    this->propertyIDHash.clear();

    return true;


}

//TODO: here's a memory leak, doesn't look like this destructor is ever called
MultiDimScalingSpace::~MultiDimScalingSpace()
{
    // Close database
    qDebug() << "Closing database";
    this->db.close();

    QString path(QDir::tempPath().append(QDir::separator()));
    path.append(QString::fromStdString(this->name_));
    path.append(".sqlite");
    path = QDir::toNativeSeparators(path);
    QFile::remove(path);

    delete metric_property_items_;
    delete metric_response_items_;
}

bool MultiDimScalingSpace::createSQLDatabase()
{
    this->db = QSqlDatabase::addDatabase("QSQLITE");

    // NOTE: We store the database in the tmp folder; should work under both
    // windows and linux
    QString path(QDir::tempPath().append(QDir::separator()));
    path.append(QString::fromStdString(this->name_));
    path.append(".sqlite");
    path = QDir::toNativeSeparators(path);

    // Check if database already exists and delete it if it does
    if (QFile::exists(path))
    {
        QFile::remove(path);
    }

    db.setDatabaseName(path);

    if (!db.open())
    {
        qDebug() << "Failed to create database";
        return false;
    }
    else
    {
        qDebug() << "Database Created Successfully";
        return true;
    }

}

// Function to read metric data
void MultiDimScalingSpace::getMetricDataCollections()
{

    SmartPtr<Named_interface> ni =
            Root::instance()->interface( metricData_manager +
                                         "/metricRegistrar");

    MetricDataManager* mDataRegistrar =
            dynamic_cast<MetricDataManager*>(ni.raw_ptr());

    std::vector< grid_prop_pairT >::const_iterator it = properties_.begin();

    for( ; it != properties_.end() ; ++it)
    {
        MetricDataCollection mdataCollection =
                mDataRegistrar->getMetricDataCollection(it->first,
                                                        it->second,
                                                        metricNames_);

        if(mdataCollection.empty())
        {
            mdataCollections_.clear();
            return;
        }

        mdataCollections_.push_back(mdataCollection);
    }
}

bool MultiDimScalingSpace::addData(QDomDocument &Parameters,
                                   QString propertyName)
{
    // Parse the parameters XML Document
    QDomElement root = Parameters.documentElement();
    if( root.tagName() != "parameters" )
    {
        qDebug() << "Read incomplete parameter file";
        return false;
    }

    // Look up which algorithm was used to generate this particular property
    // We will be using this to decide which table to put the parameters in
    QString algorithmName;
    QDomNode e = root.firstChild();
    while (!e.isNull())
    {
        QDomElement nodeElem = e.toElement();
        if (nodeElem.tagName() == QString::fromStdString("algorithm"))
        {
            algorithmName = nodeElem.attribute("name","");
            break;
        }
    }

    // Make sure we have read a real algorithm name
    bool initializeTable = false;
    if (algorithmName != "")
    {
        // Check a SQL Table for this algorithm already exists; if not we
        // we set a flag to create a new one
        initializeTable = !this->db.tables().contains(algorithmName);
    }
    else
        return false;

    // Add property's algorithm name to the hash table. This will allow us to
    // find out which table to read from later when we are looking up properties
    // Assume that there aren't two properties with the same name but different
    // algorithms
    propertyAlgorithmHash_[propertyName] = algorithmName;

    // If table doesn't exist we will need to add it
    QString *parameterStr;
    if (initializeTable)
    {
        parameterStr =
                new QString("create table ");
        parameterStr->append(algorithmName);
        parameterStr->append(" (id int primary key,");
    }

    // Regardless of if the table exists, we need to add the curent property's
    // set of parameters
    QString *valueStr = new QString("insert into ");
    valueStr->append(algorithmName+=" ");
    valueStr->append("values(");

    // Find row of the current line
    int currentID;
    if (!this->propertyIDHash.contains(propertyName))
    {
        // Not in hash, we need to add
        this->propertyIDHash[propertyName] = this->idCount;
        currentID = this->idCount;
        this->idCount++;
    }
    else
    {
        currentID = this->propertyIDHash[propertyName];
    }

    // The value string is the string that stores the values of all the
    // parameters
    QString idStr;
    idStr.setNum(currentID);
    idStr.append(",");
    valueStr->append(idStr);

    // The document is set up as a XML which is a tree-like format, we traverse
    // this tree recursively and flatten it into 2 stringlists that stores the
    // parameter name and parameter value
    QStringList *parameterStrList = new QStringList();
    this->traverseNode(root.firstChild(),QString::fromStdString("parameters"),
                       valueStr,parameterStrList,initializeTable);

    // remove final comma and add a closing bracket
    valueStr->remove(valueStr->size()-1,1);
    valueStr->append(")");

    QSqlQuery query;
    // If we are creating a new table; that is this is the first time we are
    // seeing an object with its algorithm type
    if (initializeTable)
    {
        // Put name of parameters into QHash
        QString truncatedAlgoName = algorithmName.left(algorithmName.size()-1);

        this->parameterNameHashTable[truncatedAlgoName] = parameterStrList;

        // Make a copy of the complete parameter string list
        this->completeParameterNames = *parameterStrList;

        // Parse parameter names into SQL command for creating new table
        for (int i = 0; i <parameterStrList->size(); ++i)
        {

            // This is a hack to fix fact that some algorithms are saved
            // with property_value and others property_sim_value
            if (parameterStrList->at(i).contains("parameters$Property$value"))
                parameterStr->append(QString("parameters$Property_Name$value"));
            else if (parameterStrList->at(i).contains("parameters$Property_Name_Sim$value"))
                parameterStr->append(QString("parameters$Property_Name$value"));
            else
                parameterStr->append(parameterStrList->at(i));

            parameterStr->append(" varchar(255),");
        }
        parameterStr->remove(parameterStr->size()-1,1);
        parameterStr->append(")");

        bool ok = query.exec(*parameterStr);

        if (!ok)
        {
            QSqlError queryError = query.lastError();
            qDebug() << "Error " << queryError.databaseText();
            return false;
        }
    }


    // Add row
    bool ok = query.exec(*valueStr);
    if (!ok)
    {
        QSqlError queryError = query.lastError();
        qDebug() << "Error " << queryError.databaseText();
        return false;
    }

    // SQL doesn't allow dynamic bindings of table names; we either have to use
    // this string substituion or come up with a better database design; my
    // money is on the latter - lewisli
    QString updateStr;
    updateStr.append("UPDATE ");
    updateStr.append(algorithmName);

    if (query.prepare(updateStr.append("SET parameters$Property_Name$value = "
                                       ":propName WHERE id = :idStr")))
    {
        QString idNum;
        query.bindValue(":propName", propertyName);
        query.bindValue(":idStr", idNum.setNum(currentID));
        query.exec();
    }
    else
    {
        QSqlError queryError = query.lastError();
        qDebug() << "SQL error " << queryError.databaseText();
        return false;
    }

    return true;
}

// Recursively expands the XML document and flattens it into a stringlist for
// the parameters and one for the values
void MultiDimScalingSpace::traverseNode(QDomNode node, QString path,
                                        QString *values,
                                        QStringList *parameterList,
                                        bool initial)
{
    // Make sure node is not null
    if (node.isNull())
        return;

    // Get all attributes of node
    QDomElement e = node.toElement();
    QDomNamedNodeMap att = e.attributes();

    // Read All Attributes
    // Note: We are using $ as seperator as _ is used as seperator in parameter
    for (int i = 0; i < att.count(); ++i)
    {
        QDomNode v = att.item(i);
        if (initial)
        {
            QString currentParameter;
            currentParameter.append(path);
            currentParameter.append("$");
            currentParameter.append(e.tagName());
            currentParameter.append("$");
            currentParameter.append(v.nodeName());

            // SQL does not let us add two strings that have the exact same name
            // we need to make sure the parameterList does not contain two
            // nodes with exactly the same name, this often happens when we are
            // adding multiple elements and assign them a counter value
            currentParameter = checkRepeatedParamName(currentParameter,
                                                      parameterList,0);

            parameterList->append(currentParameter);
        }
        values->append("'");
        values->append(v.nodeValue());
        values->append("'");
        values->append(",");
    }

    // Traverse Children
    QString childPath = path;
    childPath.append("$");
    childPath.append(e.tagName());
    this->traverseNode(node.firstChild(),childPath,values,
                       parameterList,initial);

    // Traverse Siblings
    this->traverseNode(node.nextSibling(),path,values,
                       parameterList,initial);
}

QString MultiDimScalingSpace::checkRepeatedParamName(QString parameter,
                                                     QStringList *parameterList,
                                                     unsigned int copyID)
{
    // If the parameterList does not contain the parameter, we simply just
    // return its value
    if (!parameterList->contains(parameter))
        return parameter;
    else
    {

        QString newParameter = parameter;
        QString number;
        number.setNum(copyID);
        newParameter.append(number);
        if (!parameterList->contains(newParameter))
            return newParameter;
        else
        {
            return checkRepeatedParamName(parameter,parameterList,copyID+1);
        }
    }
}

void MultiDimScalingSpace::computeDistanceMetricSpace()
{
    // Create upper triangular distance matrix
    distanceMetric.resize(nProps_, nProps_);
    for (int i = 0; i < nProps_; i++)
    {
        for (int j = 0; j < nProps_; j++)
        {
            if (i < j)
                distanceMetric(i,j) =
                        mdataCollections_[i].distance(mdataCollections_[j]);
            else
                distanceMetric(i,j) = 0.0f;
        }
    }
}
void MultiDimScalingSpace::computeDistanceMDSSpace()
{
    distanceMDS.resize(nProps_, nProps_);
    for (int i = 0; i < nProps_; i++) {
        for (int j = 0; j < nProps_; j++)
        {
            if (i < j)
            {
                distanceMDS(i,j) = std::pow(pointCoords_[0][i]-
                                            pointCoords_[0][j],2) +
                        std::pow(pointCoords_[1][i]-pointCoords_[1][j],2) +
                        std::pow(pointCoords_[2][i]-pointCoords_[2][j],2);
            }
            else
                distanceMDS(i,j) = 0.0f;
        }
    }

}

void MultiDimScalingSpace::computeMDS(){
    float* output_points = new float[MDS_DIMENSIONS * nProps_];
    mds mds_algo;
    mds_algo.do_mds(distanceMetric, nProps_, output_points, MDS_DIMENSIONS);

    pointCoords_.resize(3, std::vector<float>(nProps_, 0));
    for(int i=0; i< nProps_; ++i ) {
        pointCoords_[0][i] =  output_points[i*3+0];
        pointCoords_[1][i] =  output_points[i*3+1];
        pointCoords_[2][i] =  output_points[i*3+2];
    }
    delete output_points;
}

GsTLPoint MultiDimScalingSpace::getCoords(int index) const{
    return GsTLPoint(pointCoords_[0][index], pointCoords_[1][index], pointCoords_[2][index]);
}

float MultiDimScalingSpace::getXCoords(int index) const{
    return pointCoords_[0][index];
}

float MultiDimScalingSpace::getYCoords(int index) const{
    return pointCoords_[1][index];
}

float MultiDimScalingSpace::getZCoords(int index) const{
    return pointCoords_[2][index];
}

float MultiDimScalingSpace::getDistanceMetrics(int i, int j) const{
    return distanceMetric(i,j);
}

float MultiDimScalingSpace::getDistanceMdsSpace(int i, int j) const{
    return distanceMDS(i,j);
}

std::vector<MultiDimScalingSpace::Cluster> MultiDimScalingSpace::getClusters() const {
    std::vector<Cluster> clusters;
    map<unsigned int, Cluster>::const_iterator it;
    for (it = clusters_.begin() ; it != clusters_.end(); it++ )
        clusters.push_back((*it).second);

    return clusters;
}

void MultiDimScalingSpace::kmeans(unsigned int k) {
    // check if k-means for k has already been done
    map<unsigned int, Cluster>::const_iterator it = clusters_.find(k);
    if (it == clusters_.end()) {
        // doesn't exist, execute k means
        Cluster kmeans_result;
        char name_buf[20];
        if (k == 1)
            sprintf(name_buf, "%d Cluster", k);
        else
            sprintf(name_buf, "%d Clusters", k);

        kmeans_result.name = std::string(name_buf);
        kmeans_result.num_clusters = k;

        // populate kmeans_result.clusters with result of k-means clustering
        int num_collections = mdataCollections_.size();
        int* cluster_assignments = new int[num_collections];
        kmeans::execute(mdataCollections_, cluster_assignments, num_collections, k);
        kmeans_result.clusters.resize(num_collections);
        for (int i = 0; i < num_collections; i++)
            kmeans_result.clusters[i] = cluster_assignments[i];
        delete[] cluster_assignments;

        /* debug
  printf("Cluster assignments:\n");
  vector<unsigned int>::iterator vit;
  for (vit = kmeans_result.clusters.begin(); vit != kmeans_result.clusters.end(); vit++)
   printf("%d,", *vit);
  printf("\n");
  */
        clusters_[k] = kmeans_result;
    }
}


// This function returns the hash table and returns a pointer to the list of
// parameters that was used to generate the algorithm
QStringList* MultiDimScalingSpace::getParameterNames(QString algoName)
{
    if (parameterNameHashTable.contains(algoName))
        return parameterNameHashTable[algoName];
    else
        return 0;
}

// This function reads the hash table and returns a QString containing the name
// of the algorithm for the inputted property name
QString MultiDimScalingSpace::readPropertyAlgorithm(QString prop_)
{
    QString algoName_ = "";

    if (propertyAlgorithmHash_.contains(prop_))
        algoName_ = propertyAlgorithmHash_[prop_];

    return algoName_;
}

QString MultiDimScalingSpace::item_type() const{
    return "MetricSpace";
}
QString MultiDimScalingSpace::item_name() const{
    return name_.c_str();
}
GsTL_object_item *MultiDimScalingSpace::child(int row){
    if(row == 0) {
        return metric_property_items_;
    }
    else if(row  == 1) {
        return metric_response_items_;
    }
    else return (GsTL_object_item*)0;
}
int MultiDimScalingSpace::childCount() const{
    return 2;
}

int MultiDimScalingSpace::columnCount() const{
    return 2;
}
QVariant MultiDimScalingSpace::item_data(int column) const{
    if(column == 0) return this->item_name();
    else if(column == 1) return this->item_type();
    else return QVariant();
}


/* -----------------------------

--------------------------------*/

Metric_space_properties::
Metric_space_properties(MultiDimScalingSpace* parent) : GsTL_object_item(parent), mds_parent_(parent)
{
    //Get the metrics manager

    for(int i=0; i<mds_parent_->pointCount(); ++i ) {
        Metric_space_property_item* prop_item = new Metric_space_property_item(mds_parent_->getProperty(i),mds_parent_->getMetricDataCollection(i), this);
        items_.push_back(prop_item);
    }

}


Metric_space_responses::
Metric_space_responses(MultiDimScalingSpace* parent): GsTL_object_item(parent), mds_parent_(parent)
{

    for(int i=0; i<mds_parent_->metricCount(); ++i ) {
        Metric_space_response_item* response_item = new Metric_space_response_item(mds_parent_->getMetricName(i), mds_parent_->getProperties(), this);
        items_.push_back(response_item);
    }

}
