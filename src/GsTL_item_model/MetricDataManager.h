/*
 * MetricDataManager.h
 *
 *  Created on: Apr 14, 2011
 *      Author: aboucher
 */

#ifndef METRICDATAMANAGER_H_
#define METRICDATAMANAGER_H_

#include "MetricData.h"
#include "dommodel.h"
#include "common.h"
#include <GsTLAppli/utils/named_interface.h>
#include <GsTLAppli/appli/root_model.h>

#include <QtXml/QtXml>

//#include <hash_map>
/*
class MetricDataNamed {
public :
 MetricDataNamed(std::string name, std::string type):name_(name), type_(type){}

 void registerMetric( std::string grid_name, std::string prop_name);
 void unregisterMetric( std::string grid_name, std::string prop_name);

 bool hasGrid(std::string grid_name) const;
 bool hasProperty(std::string grid_name, std::string prop_name) const;

 void getGridNames(std::vector<std::string>& names) const;
 void getPropertyNames(std::string grid_name, std::vector<std::string>& names) const;

protected :
 std::string name_;
 std::string type_;

 typedef std::map< std::string, std::set<std::string> > mapGridToProp;
 mapGridToProp map_;

};
*/


class ITEM_MODEL_DECL MetricDataToProperties : public GsTL_object_item {
public :
    MetricDataToProperties(GsTL_object_item* parent = 0):GsTL_object_item(parent), name_(""), type_(""){}
    MetricDataToProperties(std::string name, std::string type, GsTL_object_item* parent = 0);

    bool registerMetric( std::string grid_name, std::string prop_name, MetricData* metric);
    bool unregisterMetric( std::string grid_name, std::string prop_name);

    bool hasGrid(std::string grid_name) const;
    bool hasProperty(std::string grid_name, std::string prop_name) const;

    void getGridNames(std::vector<std::string>& names) const;
    void getPropertyNames(std::string grid_name, std::vector<std::string>& names) const;

    int size() const {return propToData_.size();}

    std::string name()const {return name_;}
    std::string type()const {return type_;}

    std::pair<std::string, MetricData*> index(int i) const;

    QDomDocument getDomDocument(){return doc_;}
    QDomElement getDomElement(){return rootMetric_;}
    //	QAbstractItemModel* getModel(){return rootMetric;}


    //GsTL_object_item
    virtual QString item_name() const {return name_.c_str();}
    virtual QString item_type() const {return QString::fromStdString("Metric ("+type_+")");}
    virtual QString data_type() const {return QString::fromStdString(type_);}
    virtual GsTL_object_item *child(int row)
    {
        if(row >= propToData_.size())
            return static_cast<GsTL_object_item *>(0);

        mapPropToDataItemT::iterator it = propToDataItem_.begin();
        std::advance(it,row);

        return it->second;

    }
    virtual int childCount() const {return propToData_.size();}
    virtual int columnCount() const {return 2;}

    virtual QVariant item_data(int column) const
    {
        if(column == 0) return this->item_name();
        else if(column == 1) return this->item_type();
        else return QVariant();
    }


protected :
    std::string name_;
    std::string type_;

    //The property are stored in the format gridName/propNames
    typedef std::map<std::string,  MetricData*> mapPropToDataT;
    mapPropToDataT  propToData_;

    // This is an extra wrapper that we used to display in a tree
    typedef std::map<std::string, MetricDataPropertyItem*> mapPropToDataItemT;
    mapPropToDataItemT propToDataItem_;

    QDomDocument doc_;
    QDomElement rootMetric_;

    Root_model*  model_ ;

    //	QDomModel domModel_;


};


class ITEM_MODEL_DECL MetricDataManager: public Named_interface {
public:
    MetricDataManager();
    virtual ~MetricDataManager();

    static Named_interface* create_new_interface(std::string& ){return new MetricDataManager;}

    MetricData* getMetricData(std::string grid_name, std::string prop_name, std::string metric_name);
    const MetricData* getMetricData(std::string grid_name, std::string prop_name, std::string metric_name) const;
    MetricDataCollection getMetricDataCollection(std::string grid_name, std::string prop_name,
                                                 std::vector<std::string> metric_names);
    MetricDataCollection getMetricDataCollection(std::string grid_name, std::string prop_name);


    void getPropertyNames(std::string metric_name, std::string grid_name, std::vector<std::string>& names) const;
    void getGridNames(std::string metric_name, std::vector<std::string>& names) const;
    bool hasMetric(std::string metric_name) const;
    bool hasGrid(std::string metric_name, std::string grid_name) const;
    bool hasProperty(std::string metric_name, std::string grid_name, std::string prop_name) const;


    void registerMetricData(std::string grid_name, std::string prop_name, MetricData* metric);

    //	const MetricDataNamed& get_metrics_named(const std::string& metric_name) const;

    const MetricDataToProperties& get_properties_for_metric(const std::string& metric_name) const;

    std::vector<std::string> get_metric_names() const;

    bool empty() const{ return topLevelMetricRegistrar_.empty();}

    //QDomDocument getMetricDomDocument(){return docMetric_;}
    QDomDocument getMetricDomDocument() const {return docMetric_;}


    //GsTL_object_item
    virtual QString item_name() const {return "Metrics";}
    virtual QString item_type() const {return "Metric Manager";}

    virtual GsTL_object_item *child(int row)
    {
        if(row >= topLevelPropRegistrar_.size())
            return static_cast<GsTL_object_item *>(0);

        mapMetricToProps::iterator it = topLevelPropRegistrar_.begin();
        std::advance(it,row);
        return &it->second;
    }
    virtual int childCount() const {return topLevelPropRegistrar_.size();}
    virtual int columnCount() const {return 2;}
    virtual QVariant item_data(int column) const {
        if(column == 0) return this->item_name();
        else if(column == 1) return this->item_type();
        else return QVariant();
    }

protected :
    // The key to retrieve the map of metric associated to one property is gridName::propName
    typedef std::map<std::string, MetricData*> mapPropLevelMetric;
    typedef std::map<std::string, mapPropLevelMetric> mapGridLevelMetricRegistrar;
    mapGridLevelMetricRegistrar topLevelMetricRegistrar_;

    // The opposite: retrieve all the property and grid name associated to MetricData
    //typedef std::map<std::string, MetricDataNamed> mapMetricToProps;
    typedef std::map<std::string, MetricDataToProperties> mapMetricToProps;
    mapMetricToProps topLevelPropRegistrar_;

    QDomDocument docMetric_;
    QDomElement rootMetric_;

    Root_model*  model_ ;

};

#endif /* METRICDATAMANAGER_H_ */
