/*
 * MetricDataManager.cpp
 *
 *  Created on: Apr 14, 2011
 *      Author: aboucher
 */

#include "MetricDataManager.h"
#include <GsTLAppli/utils/string_manipulation.h>
#include <GsTLAppli/utils/manager.h>

MetricDataToProperties::MetricDataToProperties(std::string name,
                                               std::string type,
                                               GsTL_object_item* parent)
    : GsTL_object_item(parent), name_(name), type_(type)
{

    model_ = dynamic_cast<Root_model*>(Root::instance()->model());
    rootMetric_ = doc_.createElement(name.c_str());
    doc_.appendChild(rootMetric_);
}

bool MetricDataToProperties::
registerMetric( std::string grid_name, std::string prop_name,
                MetricData* metric)
{
    std::string key(grid_name+"/"+prop_name);
    mapPropToDataT::const_iterator it = propToData_.find(key);

    if(it != propToData_.end())
        return false;

    // Inform the model that a new property is added
    if(model_) {
        // Need to find this node in the tree: First go up to the root then down
        mapPropToDataT::iterator it = propToData_.lower_bound(key);
        int row = std::distance( propToData_.begin(), it);
        model_->begin_insert_item(this,row);
    }

    propToData_[key] = metric;

    MetricDataPropertyItem *metricItem =
            new MetricDataPropertyItem(metric,this);

    propToDataItem_[key] = metricItem;

    if(model_) model_->end_insert_item();

    // Get the metric dom structure
    if( !rootMetric_.hasAttribute("Type") ){
        rootMetric_.setAttribute("Type",metric->algoName().c_str());
    }
    if( !rootMetric_.hasAttribute("Format") ){
        rootMetric_.setAttribute("Type",metric->dataFormat().c_str());
    }

    // Get the grid dom structure
    QDomElement gridElem  = rootMetric_.firstChildElement (grid_name.c_str());
    if( gridElem.isNull() )
    {
        // First time with that grid
        gridElem = doc_.createElement(grid_name.c_str());
        rootMetric_.appendChild(gridElem);
    }

    QDomElement propElem  = doc_.createElement( prop_name.c_str()  );
    gridElem.appendChild(propElem);

    return true;
}

bool MetricDataToProperties::unregisterMetric( std::string grid_name,
                                               std::string prop_name)
{
    std::string key(grid_name+"/"+prop_name);
    mapPropToDataT::iterator it = propToData_.find(key);
    mapPropToDataItemT::iterator itItem = propToDataItem_.find(key);
    if( it == propToData_.end()) return false;
    propToData_.erase(it);
    propToDataItem_.erase(itItem);
    return true;
}


bool MetricDataToProperties::hasProperty(std::string grid_name,
                                         std::string prop_name) const
{
    std::string key(grid_name+"/"+prop_name);
    return propToData_.find(key) != propToData_.end();
}

bool MetricDataToProperties::hasGrid(std::string grid_name ) const
{
    std::vector<std::string> grid_names;
    this->getGridNames(grid_names);
    if(grid_name.empty()) return false;
    std::vector<std::string>::const_iterator it =
            std::find(grid_names.begin(),grid_names.end(),grid_name  );
    return it != grid_names.end();
}


void MetricDataToProperties::getGridNames(std::vector<std::string>& names) const
{
    names.clear();
    std::set<std::string> grid_names;
    mapPropToDataT::const_iterator it = propToData_.begin();
    for( ; it!= propToData_.end(); ++it)
    {
        std::pair<std::string,std::string> grid_prop =
                String_Op::split_string(it->first,"/",false);
  	grid_names.insert(grid_names.begin(),grid_prop.first);
    }
    names.insert(names.begin(),grid_names.begin(), grid_names.end());
}

void MetricDataToProperties::getPropertyNames(std::string grid_name,
                                              std::vector<std::string>& names) const
{
    names.clear();
    std::set<std::string> grid_names;
    mapPropToDataT::const_iterator it = propToData_.begin();
    for( ; it!= propToData_.end(); ++it) {
        std::pair<std::string,std::string> grid_prop =
                String_Op::split_string(it->first,"/",false);
  	if(grid_prop.first == grid_name)
            names.push_back(grid_prop.second);
    }
}

std::pair<std::string, MetricData*> MetricDataToProperties::index(int index)
const{
    mapPropToDataT::const_iterator it = propToData_.begin();
    for(int i=0; i<index; ++i, ++it){}
    return std::pair<std::string, MetricData*>("test",
                                               static_cast<MetricData*>(0));
    //return (*it);
}

MetricDataManager::MetricDataManager() {

    model_ = dynamic_cast<Root_model*>(Root::instance()->model());
}

MetricDataManager::~MetricDataManager() {
    // TODO Auto-generated destructor stub
}

void MetricDataManager::registerMetricData(std::string grid_name,
                                           std::string prop_name,
                                           MetricData* metric)
{

    std::string keyName = grid_name + "/" + prop_name;

    mapPropLevelMetric* mRegistrar = &topLevelMetricRegistrar_[keyName];

    //Check to see if there is already one metric Data with the same name
    mapPropLevelMetric::iterator it = mRegistrar->find(metric->name());

    if(it != mRegistrar->end())
        return;  //Something is wrong, need a smarter way to handle that

    (*mRegistrar)[metric->name()] = metric;


    // The inverse; register all the properties associated with a metricData
    // (the key is the name of the metricData)
    mapMetricToProps::iterator it_p =
            topLevelPropRegistrar_.find(metric->name());

    if( it_p == topLevelPropRegistrar_.end() )
    {
        // Inform the model that a new property is added
        if(model_)
        {
            // Need to find this node in the tree:
            // First go up to the root then down
            mapMetricToProps::iterator it =
                    topLevelPropRegistrar_.lower_bound(metric->name());

            int row = std::distance( topLevelPropRegistrar_.begin(), it);
            model_->begin_insert_item(this,row);

        }

        it_p = topLevelPropRegistrar_.insert(topLevelPropRegistrar_.begin(),
               std::pair<std::string,MetricDataToProperties>
               (metric->name(),MetricDataToProperties(metric->name(),
                                                      metric->classname(),
                                                      this)));

        if(model_)
            model_->end_insert_item();

    }
    it_p->second.registerMetric(grid_name,prop_name,metric);



    QDomElement metricElem  = docMetric_.firstChildElement (metric->name().c_str());
    if( metricElem.isNull() ) {  // First time with that grid
        metricElem = docMetric_.createElement(metric->name().c_str());
        metricElem.setAttribute("Type",metric->algoName().c_str());
        metricElem.setAttribute("Format",metric->dataFormat().c_str());
        docMetric_.appendChild(metricElem);
    }
    // Get the grid dom structure
    QDomElement gridElem  = metricElem.firstChildElement (grid_name.c_str());
    if( gridElem.isNull() ) {  // First time with that grid
        gridElem = docMetric_.createElement(grid_name.c_str());
        metricElem.appendChild(gridElem);
    }
    QDomElement propElem  = docMetric_.createElement( prop_name.c_str()  );
    gridElem.appendChild(propElem);

}


MetricData* MetricDataManager::getMetricData(std::string grid_name,
                                             std::string prop_name,
                                             std::string metric_name)
{

    std::string keyName = grid_name+"/"+prop_name;

    mapGridLevelMetricRegistrar::iterator it =
            topLevelMetricRegistrar_.find(keyName);

    if(it == topLevelMetricRegistrar_.end())
        return 0;

    // Is the metricData exists for that property
    mapPropLevelMetric::iterator it_m = it->second.find(metric_name);

    if(it_m == it->second.end()) return 0;
    return it_m->second;

}


const MetricData* MetricDataManager::getMetricData(std::string grid_name,
                                                   std::string prop_name,
                                                   std::string metric_name) const
{
    std::string keyName = grid_name+"/"+prop_name;

    mapGridLevelMetricRegistrar::const_iterator it =
            topLevelMetricRegistrar_.find(keyName);

    if(it == topLevelMetricRegistrar_.end())
        return 0;

    // Is the metricData exists for that property
    mapPropLevelMetric::const_iterator it_m = it->second.find(metric_name);

    if(it_m == it->second.end()) return 0;
    return it_m->second;
}


MetricDataCollection
MetricDataManager::getMetricDataCollection(std::string grid_name,
                                           std::string prop_name,
                                           std::vector<std::string> metric_names)
{
    std::vector<std::string>::iterator it = metric_names.begin();
    MetricDataCollection mCollection;
    std::vector<MetricData*> mdata_vec;
    for( ; it != metric_names.end(); ++it)
    {
        // Grabs metric data for each property and grid
        MetricData* mdata = this->getMetricData(grid_name, prop_name, *it);

        if( mdata == 0 )
            return mCollection;

        mdata_vec.push_back(mdata);
    }

    // All the metricData exist
    std::vector<MetricData*>::iterator it_m = mdata_vec.begin();
    for( ; it_m!= mdata_vec.end() ; ++it_m){
        mCollection.addMetricData(*it_m);
    }
    return mCollection;

}

MetricDataCollection
MetricDataManager::getMetricDataCollection(std::string grid_name,
                                           std::string prop_name)
{

    MetricDataCollection mCollection;

    std::string keyName = grid_name+"/"+prop_name;

    mapGridLevelMetricRegistrar::iterator it =
            topLevelMetricRegistrar_.find(keyName);

    if(it == topLevelMetricRegistrar_.end()) return mCollection;

    mapPropLevelMetric::iterator it_m = it->second.begin();
    int i = 0;
    for( ; it_m != it->second.end(); ++it_m )
    {
        i++;
        mCollection.addMetricData(it_m->second);
    }

    return mCollection;

}
const MetricDataToProperties& MetricDataManager::
get_properties_for_metric(const std::string& metric_name) const
{
    mapMetricToProps::const_iterator it =
            topLevelPropRegistrar_.find(metric_name);
    if( it == topLevelPropRegistrar_.end() )
        return MetricDataToProperties();

    return (it->second);
}


void MetricDataManager::
getGridNames(std::string metric_name, std::vector<std::string>& names) const
{
    names.clear();
    mapMetricToProps::const_iterator it = topLevelPropRegistrar_.find(metric_name);
    if( it == topLevelPropRegistrar_.end() ) return;
    it->second.getGridNames(names);

}

void MetricDataManager::
getPropertyNames(std::string metric_name, std::string grid_name,
                 std::vector<std::string>& names) const{
    names.clear();
    mapMetricToProps::const_iterator it = topLevelPropRegistrar_.find(metric_name);
    if(it==topLevelPropRegistrar_.end()) return;
    it->second.getPropertyNames(grid_name,names);
}


bool MetricDataManager::hasMetric(std::string metric_name) const{
    mapMetricToProps::const_iterator it = topLevelPropRegistrar_.find(metric_name);
    if( it == topLevelPropRegistrar_.end()) return false;
    return true;
}

bool MetricDataManager::hasGrid(std::string metric_name, std::string grid_name) const{
    mapMetricToProps::const_iterator it = topLevelPropRegistrar_.find(metric_name);
    if( it == topLevelPropRegistrar_.end() ) return false;
    return it->second.hasGrid(grid_name);
}

bool MetricDataManager::hasProperty(std::string metric_name, std::string grid_name, std::string prop_name) const{
    mapMetricToProps::const_iterator it = topLevelPropRegistrar_.find(metric_name);
    if( it == topLevelPropRegistrar_.end() ) return false;
    return it->second.hasProperty(grid_name, prop_name);

}

std::vector<std::string> MetricDataManager::get_metric_names() const{
    std::vector<std::string> names;
    mapMetricToProps::const_iterator it = topLevelPropRegistrar_.begin();
    for( ; it != topLevelPropRegistrar_.end(); ++it)
        names.push_back(it->first);
    return names;
}

/*
const MetricDataNamed& MetricDataManager::
get_metrics_named(const std::string& metric_name) const
{
 mapMetricToProps::const_iterator it = topLevelPropRegistrar_.find(metric_name);
 return (it->second);

}

*/

