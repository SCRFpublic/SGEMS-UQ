#include "gstl_item.h"

#include "metric_manager_repository.h"
#include <GsTL/utils/smartptr.h>
#include <utils/manager.h>
#include <appli/manager_repository.h>
#include <grid/grid_property.h>
#include <grid/grid_categorical_property.h>

#include <QStringList>


GsTL_item::GsTL_item()
{
}

MetricDataManager *GsTL_item::gridDataManager() const{
    SmartPtr<Named_interface> ni =
                            Root::instance()->interface( metricData_manager + "/metricRegistrar");

    //return dynamic_cast<MetricDataManager*>(ni.raw_ptr());
    return (MetricDataManager*)(ni.raw_ptr());

}


/*
   GsTL_root_item
*/


GsTL_root_item::GsTL_root_item(GsTL_project* proj )
    :GsTL_item(0),proj_(proj)
{

}

GsTL_root_item::~GsTL_root_item(){}


GsTL_item *GsTL_root_item::child(int row){

    std::list<std::string>::const_iterator it = proj_->objects_list().begin();
    for(int i=0; i< row; ++i, ++it){}

    Geostat_grid* grid = dynamic_cast<Geostat_grid*>(
                  Root::instance()->interface(
                                              gridModels_manager + "/" + *it
                                              ).raw_ptr()
                  );

 //   return new GsTL_root_item( proj_);
    return new GsTL_grid_item(proj_, grid, QString::fromStdString(*it),this);
}

int GsTL_root_item::childCount() const{
	if(proj_==0) return 0;
    return proj_->objects_list().size();
}

// There are two columns
// 1 - name of the grid
// 2 - type of grid
int GsTL_root_item::columnCount() const{
    return 2;
}


QVariant GsTL_root_item::data(int column) const{
    return QVariant();
}

int GsTL_root_item::row() const{
    return 0;
}


//  --------------------------------

//   GsTL_grid_item

//  --------------------------------


GsTL_grid_item::GsTL_grid_item():GsTL_item(0){}

GsTL_grid_item::GsTL_grid_item(GsTL_project* proj, Geostat_grid* grid, QString name, GsTL_item *parent)
    :GsTL_item(parent),proj_(proj), grid_(grid),gridName_(name)
{
}

GsTL_grid_item::~GsTL_grid_item(){}


GsTL_item *GsTL_grid_item::child(int row){
    std::list<std::string> props = grid_->property_list();

    std::list<std::string>::const_iterator it = props.begin();
    for( int i=0; i<row; i++, ++it){}
    return new GsTL_property_item( grid_->property(*it),this);
    //return new GsTL_grid_item(proj_, grid_, gridName_ ,this);
//    return new GsTL_root_item(proj_);
}

int GsTL_grid_item::childCount() const{
    std::list<std::string> props = grid_->property_list();
    return props.size();
}

// There are two columns
// 1 - name of the grid
// 2 - type of grid
int GsTL_grid_item::columnCount() const{
    return 2;
}


QVariant GsTL_grid_item::data(int column) const{
    if( column == 0 ) return gridName_;
    if( column == 1) return QString::fromStdString(grid_->classname());
    else return QVariant();
}

int GsTL_grid_item::row() const{
	std::list<std::string>::const_iterator it = proj_->objects_list().begin();
	for(int i=0; it != proj_->objects_list().end(); ++i, ++it){
		if(*it == gridName_.toStdString()) return i;
	}
	return 0;
}


//
//   GsTL_property_item
//

GsTL_property_item::GsTL_property_item():GsTL_item(0){}

GsTL_property_item::GsTL_property_item(Grid_continuous_property* prop, GsTL_item *parent)
    :GsTL_item(parent),prop_(prop)
{
    if(parent) {
        GsTL_grid_item* gridItem = dynamic_cast<GsTL_grid_item*>(parentItem_);
        if(gridItem) {
            gridName_ = gridItem->gridName();
        }
    }

}

GsTL_property_item::~GsTL_property_item(){}


GsTL_item *GsTL_property_item::child(int row){
    MetricDataManager* mDataRegistrar = this->gridDataManager();


    MetricDataCollection mData =
            mDataRegistrar->getMetricDataCollection(gridName_.toStdString(),prop_->name() );

    QString metricName = QString::fromStdString( mData.metric(row)->name());
    return new GsTL_metric_item( gridName_,QString::fromStdString(prop_->name()), metricName ,this);

}

int GsTL_property_item::childCount() const{


    MetricDataManager* mDataRegistrar = this->gridDataManager();
    MetricDataCollection mData =
            mDataRegistrar->getMetricDataCollection(gridName_.toStdString(),prop_->name());

    return mData.size();
}

// There are two columns
// 1 - name of the property
// 2 - type of property
int GsTL_property_item::columnCount() const{
    return 2;
}


QVariant GsTL_property_item::data(int column) const{
    if( column == 0 ) return QString::fromStdString(prop_->name());
    if( column == 1) {
        Grid_categorical_property* cprop = dynamic_cast<Grid_categorical_property*>(prop_);
        if(cprop) return QString("Categorical");
        else return QString("Continuous");
    }

    else return QVariant();
}

int GsTL_property_item::row() const{
    if(!parentItem_) return 0;

    GsTL_grid_item* gridItem = dynamic_cast<GsTL_grid_item*>(parentItem_);
    if(gridItem == 0) return 0;

    Geostat_grid* grid = gridItem->grid();
    std::list<std::string> props = grid->property_list();
    std::list<std::string>::const_iterator it = props.begin();
    for(int i=0; it != props.end(); ++i, ++it) {
        if( *it == prop_->name() ) {
            return i;
        }
    }
    return 0;
}





//
//        GsTL_metric_item
//


GsTL_metric_item::GsTL_metric_item():GsTL_item(0){}

GsTL_metric_item::GsTL_metric_item(QString gridName, QString propName, QString metricName, GsTL_item *parent)
    :GsTL_item(parent),gridName_(gridName),propName_(propName),metricName_(metricName)
{
    MetricDataManager* mDataManager = this->gridDataManager();

    metric_ = mDataManager->getMetricData(gridName_.toStdString(),propName_.toStdString(), metricName_.toStdString());

}

GsTL_metric_item::~GsTL_metric_item(){}


GsTL_item *GsTL_metric_item::child(int row){
    return 0;
}

int GsTL_metric_item::childCount() const{
    return 0;
}

// There are two columns
// 1 - name of the property
// 2 - type of property
int GsTL_metric_item::columnCount() const{
    return 2;
}


QVariant GsTL_metric_item::data(int column) const{
    if( column == 0 ) return metricName_;
    if( column == 1) return QString::fromStdString( metric_->algoName() );
    else return QVariant();
}

int GsTL_metric_item::row() const{

    MetricDataManager* mDataManager = this->gridDataManager();

    MetricDataCollection mData =
            mDataManager->getMetricDataCollection(gridName_.toStdString(),
                                                  propName_.toStdString());

    std::vector<MetricData*>::const_iterator it = mData.begin();
    for(int i=0 ; it != mData.end(); ++it, ++i ) {
        if( metricName_ == (*it)->name().c_str() )
            return i;
    }
    return 0;
}


