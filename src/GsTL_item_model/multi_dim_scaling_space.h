/*
 * multi_dim_scaling_space.h
 *
 *  Created on: Apr 28, 2011
 *      Author: aboucher
 */

#ifndef MULTI_DIM_SCALING_SPACE_H_
#define MULTI_DIM_SCALING_SPACE_H_

#include <vector>
#include <string>
#include <math.h>

#include "common.h"
#include "MetricData.h"
#include "MetricDataManager.h"
#include "common.h"
#include <eigen3/Eigen/Dense>

#include <GsTLAppli/utils/named_interface.h>
#include <GsTLAppli/math/gstlpoint.h>

#include <QtXml/QtXml>
#include <QtXml/QDomDocument>
#include <mdsutil.h>

#include <GsTLAppli/appli/manager_repository.h>
#include <GsTLAppli/grid/grid_model/geostat_grid.h>


#include <QDebug>

// Headers for performing SQL operations
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>

#include <QFile>

#include <QHash>
#include <QDomDocument>
#include <QXmlDefaultHandler>
#include <iostream>

#include <QStringList>

using namespace std;

#define MDS_DIMENSIONS 3

class Metric_space_properties;
class Metric_space_responses;

class ITEM_MODEL_DECL MultiDimScalingSpace : public Named_interface
{
public:

    struct Cluster {
        string name;
        unsigned int num_clusters;
        vector<unsigned int> clusters;

        // other params
        string kernel_name;
    };

    typedef std::pair< std::string, std::string> grid_prop_pairT;

    static Named_interface* create_new_interface(std::string &);

    MultiDimScalingSpace():metric_property_items_(0),metric_response_items_(0),
        idCount(0){}
    virtual ~MultiDimScalingSpace();

    bool initialize(std::string name,
                    std::vector< std::pair< std::string, std::string> > props,
                    std::vector<std::string> metricNames);

    int pointCount()const {return nProps_;}
    int metricCount()const {return metricNames_.size();}

    GsTLPoint getCoords(int index) const;
    float getXCoords(int index) const;
    float getYCoords(int index) const;
    float getZCoords(int index) const;
    std::string getPropertyName(int index)  const{return properties_[index].second;}
    std::string getGridName(int index)  const{return properties_[index].first;}

    GsTLGridProperty* getProperty( int index ) {return props_[index];}
    const GsTLGridProperty* getProperty( int index ) const {return props_[index];}

    std::vector<GsTLGridProperty*> getProperties() {return props_;}
    const std::vector<GsTLGridProperty*> getProperties(  ) const {return props_;}

    void kmeans(unsigned int k);

    std::string name()const {return name_;}

    QDomDocument getDomDocument(){return doc_;}

    std::vector<std::string> getMetricNames() const {return metricNames_;}
    std::string getMetricName(int index ) const {return metricNames_[index];}
    MetricDataCollection*  getMetricDataCollection(int index) {return &mdataCollections_[index];}

    std::vector<Cluster> getClusters() const;

    std::vector<MDSUtil::clusterBlock> *getKmeans() {return &kmeansResult;}
    const std::vector<MDSUtil::clusterBlock> *getKmeans() const {return &kmeansResult;}
    std::vector<string> *getPropertyParameters() {return &propertyParameters;}
    void setKMeanResults(std::vector<MDSUtil::clusterBlock> results) {kmeansResult = results;}

    float getDistanceMetrics(int i, int j) const;
    float getDistanceMdsSpace(int i, int j) const;

    std::vector<std::string> propertyParameters;

    QStringList* getParameterNames(QString algoName);


    // Complete list of parameters
    QStringList completeParameterNames;

    // GsTL_object_item
    virtual QString item_type() const;
    virtual QString item_name() const;
    virtual GsTL_object_item *child(int row);
    virtual int childCount() const;
    virtual int columnCount() const;
    virtual QVariant item_data(int column) const;

    QString readPropertyAlgorithm(QString prop_);

protected :

    void computeDistanceMetricSpace();
    void computeDistanceMDSSpace();
    void getMetricDataCollections();
    void computeMDS();

    // Create a new SQLite database for the particular MDS space
    bool createSQLDatabase();

    QString checkRepeatedParamName(QString parameter,
                                   QStringList *parameterList,
                                   unsigned int copyID);


protected :
    int nProps_;
    std::string name_;
    QDomDocument doc_;
    std::vector< grid_prop_pairT > properties_;
    std::vector<GsTLGridProperty*> props_;

    std::vector<std::string> metricNames_;
    std::map<unsigned int, Cluster> clusters_;
    std::vector<std::vector<float> > pointCoords_;
    std::vector<MetricDataCollection> mdataCollections_;

    Eigen::MatrixXf distanceMetric;
    Eigen::MatrixXf distanceMDS;

    std::vector<MDSUtil::clusterBlock> kmeansResult;

    Metric_space_properties* metric_property_items_;
    Metric_space_responses* metric_response_items_;

    QSqlDatabase db;

    // This store a hash table of the IDs of each of the algorithms
    QHash<QString,int> propertyIDHash;

    // This table gives the name of the algorithm of the particular property
    QHash<QString, QString> propertyAlgorithmHash_;

    // This stores a stringlist for each algorithm type that contains
    // the names of the parameters used for that algorithm
    QHash<QString,QStringList*> parameterNameHashTable;

    void traverseNode(QDomNode node, QString path, QString *values,
                      QStringList *parameterList,
                      bool initial);
    bool addData(QDomDocument &Parameters, QString propertyName);

    int idCount;
};

//Class for dislay within Model/View framework



class Metric_space_property_item : public GsTL_object_item {
public :
    Metric_space_property_item();
    Metric_space_property_item(GsTLGridProperty* prop, MetricDataCollection* metric_collection, GsTL_object_item* parent)
        : GsTL_object_item(parent), prop_(prop){}
    virtual  ~Metric_space_property_item(){}

    virtual void* data_pointer(){return static_cast<void*>(prop_);}

    GsTLGridProperty* property(){return prop_;}
    void property(GsTLGridProperty* prop){prop_ = prop;}

    // GsTL_object_item
    virtual QString item_type() const {return prop_->item_type();}
    virtual GsTL_object_item *child(int row) {
        return prop_->child(row);}
    virtual int childCount() const {return 0;}
    virtual int columnCount() const {return 2;}
    virtual QVariant item_data(int column) const{
        return prop_->item_data(column);
    }

private :
    GsTLGridProperty* prop_;
    MetricDataCollection* metrics_;
};


class Metric_space_response_item : public GsTL_object_item {
public :
    Metric_space_response_item();
    Metric_space_response_item(std::string metric_name, std::vector<GsTLGridProperty*> props, GsTL_object_item* parent)
        : GsTL_object_item(parent),metric_name_(metric_name.c_str()), props_(props){}


    virtual  ~Metric_space_response_item(){}
    // virtual void* data_pointer(){return static_cast<void*>(metric_);}

    // GsTL_object_item
    virtual QString item_type() const {return "Responses";}
    virtual GsTL_object_item *child(int row) {return (GsTL_object_item*)0;}
    virtual int childCount() const {return 0;}
    virtual int columnCount() const {return 2;}
    virtual QVariant item_data(int column) const{
        if(column == 0 )return metric_name_;
        else if(column == 1 )return this->item_type();
        else return QVariant();
    }

private :
    QString metric_name_;
    //MetricData* metric_;
    std::vector<GsTLGridProperty*> props_;
};

class ITEM_MODEL_DECL Metric_space_properties : public GsTL_object_item {
public:

    // GsTL_object_item
    Metric_space_properties(MultiDimScalingSpace* parent);

    virtual QString item_type() const {return "";}
    virtual QString item_name() const {return "MetricProperties";}
    virtual GsTL_object_item *child(int row)
    {
        if(row<items_.size())
        {
            return items_[row];
        }
        return (GsTL_object_item*)(0);
    }
    virtual int childCount() const {return mds_parent_->pointCount(); }
    virtual int columnCount() const {return 2;}
    virtual QVariant item_data(int column) const{
        if(column == 0) return this->item_name();
        if(column == 1) return this->item_type();
        else return QVariant();
    }

protected :
    MultiDimScalingSpace* mds_parent_;
    std::vector<Metric_space_property_item*> items_;

};


class ITEM_MODEL_DECL Metric_space_responses : public GsTL_object_item {

public:
    // GsTL_object_item
    Metric_space_responses(MultiDimScalingSpace* parent);

    virtual QString item_type() const {return "";}
    virtual QString item_name() const {return "MetricResponses";}
    virtual GsTL_object_item *child(int row){
        if(row<items_.size()) return items_[row];
        return (GsTL_object_item*)(0);
    }
    virtual int childCount() const {return mds_parent_->metricCount(); }
    virtual int columnCount() const {return 2;}
    virtual QVariant item_data(int column) const {
        if(column == 0) return this->item_name();
        if(column == 1) return this->item_type();
        else return QVariant();
    }

protected :
    MultiDimScalingSpace* mds_parent_;
    std::vector<Metric_space_response_item*> items_;


};





#endif /* MULTI_DIM_SCALING_SPACE_H_ */
