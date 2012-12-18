#include "kmeans_action.h"
#include "MetricFilterPackage.h"
#include "MetricFilterCollection.h"
#include "ml_library_constants.h"

#include "kmeans.h"
#include <MetricDataManager.h>
#include <metric_manager_repository.h>
#include <multi_dim_scaling_space.h>

#include <GsTLAppli/actions/defines.h>
#include <GsTLAppli/utils/gstl_messages.h>
#include <GsTLAppli/utils/string_manipulation.h>
#include <GsTLAppli/utils/error_messages_handler.h>
#include <GsTLAppli/utils/string_manipulation.h>
#include <GsTLAppli/grid/grid_model/geostat_grid.h>
#include <GsTLAppli/grid/grid_model/point_set.h>
#include <GsTLAppli/appli/manager_repository.h>
#include <GsTLAppli/appli/project.h>
#include <GsTL/utils/smartptr.h>

#include <GsTLAppli/geostat/utilities.h>
#include <iostream>
#include <sstream>


#include <eigen3/Eigen/Dense>

using namespace Eigen;
using namespace std;
using Eigen::MatrixXf;

Named_interface* kmeans_action::create_new_interface( std::string& ){
  return new kmeans_action;
}

kmeans_action::~kmeans_action() {

}

//The difficulty here is that we may have property on different grid.
// The data will be passed through an xml file.

// filename
bool kmeans_action::init(std::string& parameters, GsTL_project* proj,
                         Error_messages_handler* errors) {

  std::vector< std::string > params = 
    String_Op::decompose_string( parameters, Actions::separator,
                      				   Actions::unique );

  if( params.size() != 2 ) {
    errors->report( "Need the name of the output and filename to an xml file" );
    return false;
  }

  std::vector<MultiDimScalingSpace::grid_prop_pairT> props_grid;

  proj_ = proj;

  output_pset_name_ = params[1];
  if(output_pset_name_.empty()) {
	  errors->report( "No output point set name specified" );
	  output_pset_name_ = "kmeans_pset";
  }


	SmartPtr<Named_interface> ni =
				Root::instance()->interface( metricData_manager + "/metricRegistrar");

	MetricDataManager* mDataRegistrar = dynamic_cast<MetricDataManager*>(ni.raw_ptr());

	// xml params
	QDomDocument doc("kmeansAction");
	qDebug() << " filename : " << QString(params[0].c_str());
	QFile file(params[0].c_str());
	if (!doc.setContent(&file))
	{
		file.close();
		errors->report( "The file "+params[0]+" is not a valid xml format file" );
		return false;
	}
	file.close();

	QDomElement root = doc.documentElement();

	QDomElement elemConfig = root.firstChildElement("Configuration");
	num_clusters_ = elemConfig.attribute("numberOfClusters").toInt();

	QDomElement elemKernel = root.firstChildElement("Kernel");
	std::string kernel_type = elemKernel.attribute("type").toStdString();

	QDomElement elemMetricData = root.firstChildElement("MetricData");
	QDomElement elemMetric = elemMetricData.firstChildElement("Metric");
	std::vector<std::string> mdata_names; // vector of metric names
	for( ; !elemMetric.isNull(); elemMetric = elemMetric.nextSiblingElement("Metric")  ) {
		std::string metric_name = elemMetric.attribute("name").toStdString();
		mdata_names.push_back(metric_name);
	}

	QDomElement elem = root.firstChildElement("Grid");
	for( ; !elem.isNull(); elem = elem.nextSiblingElement("Grid")  ) {
		std::string grid_name = elem.attribute("name").toStdString();
		QDomElement gridProps = elem.firstChildElement("Property");
		for (; !gridProps.isNull(); gridProps = gridProps.nextSiblingElement("Property")) {
			std::string property_name = gridProps.attribute("name").toStdString();
			props_grid.push_back(MultiDimScalingSpace::grid_prop_pairT(grid_name,property_name));
			MetricDataCollection mdataCollection =
					mDataRegistrar->getMetricDataCollection(grid_name, property_name, mdata_names);
			if(mdataCollection.empty()) {
				errors->report( "Some metric data for the property "+property_name+ " on the grid "+grid_name+" could not be found" );
				return false;
			}
			mdataCollections_.push_back(mdataCollection);
		}
	}
	num_inputs_ = mdataCollections_.size();

	// set kernels

	  SmartPtr<Named_interface> kernel_ni =
	    Root::instance()->new_interface( kernel_type, kernel_manager + "/" );

	if (!kernel_ni.raw_ptr()) {
		errors->report( "Kernel "+kernel_type+" could not be found" );
		return false;
	}
	Distance_kernel* kernel = dynamic_cast<Distance_kernel*>(kernel_ni.raw_ptr());
	if (!kernel) {
		errors->report( "Kernel "+kernel_type+" has an invalid kernel type for operating on the given data" );
		return false;
	}
	for (int i =0; i < num_inputs_; i++) {
		mdataCollections_[i].setKernel(kernel->clone());
	}

	// TODO: create kmeans object?

	/*ni = Root::instance()->new_interface( "multi_dim_scaling", mds_manager + "/test_mds");

	MultiDimScalingSpace*  mdsObject = dynamic_cast<MultiDimScalingSpace*>(ni.raw_ptr());

	mdsObject->initialize("test_mds", props_grid,mdata_names);*/


  return errors->empty();

}

bool kmeans_action::exec() {

	kmeans kmeans_algo;
	int* result_clusters = new int[num_inputs_];

	kmeans_algo.execute(&mdataCollections_.front(), result_clusters, num_inputs_, num_clusters_);

	// TODO: save as kmeans object in manager

	delete[] result_clusters;

	// add points to point set
	/*
	std::vector< Point_set::location_type > point_locations;
	for (int i = 0; i < num_inputs_; i++) {
		point_locations.push_back(Point_set::location_type(output_points[i*3+0], output_points[i*3+1], output_points[i*3+2]));
	}
	int point_set_size = point_locations.size();
 
	// check if the grid already exist
	SmartPtr<Named_interface> ni =
		Root::instance()->interface( gridModels_manager + "/" + output_pset_name_ );

	if( ni.raw_ptr() != 0 ) {
		GsTLcerr << "object " << output_pset_name_ << " already exists\n" << gstlIO::end;
		return 0;
	}

	std::string size_str = String_Op::to_string( point_set_size );
	ni = Root::instance()->new_interface( "point_set://" + size_str, 
					gridModels_manager + "/" + output_pset_name_ );
	Point_set* pset = dynamic_cast<Point_set*>( ni.raw_ptr() );
	appli_assert( pset != 0 );

	pset->point_locations( point_locations );
	
	
	// assign properties
	GsTLGridProperty* prop_name = pset->add_property( "name" );
	for( int i=0; i < point_set_size; i++ ) {
		prop_name->set_value( i, i ); // replace with input_props_[i]->name()
	}

	delete output_points;

	// update project
  proj_->new_object( output_pset_name_ );*/

	return true;
}
