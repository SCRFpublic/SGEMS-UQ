#include "mds_action.h"
#include "MetricFilterPackage.h"
#include "MetricFilterCollection.h"
#include "ml_library_constants.h"

#include <mds.h> // mds algorithm
#include <MetricDataManager.h>
#include <metric_manager_repository.h>
#include <multi_dim_scaling_space.h>

#include <actions/defines.h>
#include <utils/gstl_messages.h>
#include <utils/string_manipulation.h>
#include <utils/error_messages_handler.h>
#include <utils/string_manipulation.h>
#include <grid/geostat_grid.h>
#include <grid/point_set.h>
#include <appli/manager_repository.h>
#include <appli/project.h>
#include <GsTL/utils/smartptr.h>

#include <geostat/utilities.h>
#include <iostream>
#include <sstream>


#include <Eigen/Dense>

using namespace Eigen;
using namespace std;
using Eigen::MatrixXf;

Named_interface* MDS_action::create_new_interface( std::string& ){
	return new MDS_action;
}

MDS_action::~MDS_action() {

}

//The difficulty here is that we may have property on different grid.
// The data will be passed through an xml file.

// filename
bool MDS_action::init(std::string& parameters, GsTL_project* proj,
		Error_messages_handler* errors) {


	std::vector< std::string > params =
                        String_Op::decompose_string( parameters,
                                                     Actions::separator,
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
		output_pset_name_ = "MDS_points";
	}


	SmartPtr<Named_interface> ni =
			Root::instance()->interface( metricData_manager + "/metricRegistrar");

	MetricDataManager* mDataRegistrar = dynamic_cast<MetricDataManager*>(ni.raw_ptr());

	//Get the XML definition of the filter
	QDomDocument doc("MDSAction");
	QFile file(params[0].c_str());
	if (!doc.setContent(&file))
	{
		file.close();
		errors->report( "The file "+params[0]+" is not a valid xml format file" );
		return false;
	}
	file.close();

	QDomElement root = doc.documentElement();

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


	ni = Root::instance()->new_interface( "multi_dim_scaling",mds_manager + "/" + output_pset_name_);

	MultiDimScalingSpace*  mdsObject = dynamic_cast<MultiDimScalingSpace*>(ni.raw_ptr());

	mdsObject->initialize(output_pset_name_,props_grid,mdata_names);


	return errors->empty();

}

bool MDS_action::initFromDom(QDomDocument parameters, GsTL_project* proj,
		Error_messages_handler* errors)
{
	std::vector<MultiDimScalingSpace::grid_prop_pairT> props_grid;

	SmartPtr<Named_interface> ni =
			Root::instance()->interface( metricData_manager + "/metricRegistrar");

	MetricDataManager* mDataRegistrar = dynamic_cast<MetricDataManager*>(ni.raw_ptr());

	// Get root element
	QDomElement root = parameters.documentElement();

	// Read kernel type
	QDomElement elemKernel = root.firstChildElement("Kernel");
	std::string kernel_type = elemKernel.attribute("type").toStdString();

	// Read space name
	QDomElement spaceName = root.firstChildElement("Spacename");
	std::string space_name = spaceName.attribute("name").toStdString();

	// Read metric data
	QDomElement elemMetricData = root.firstChildElement("MetricData");
	QDomElement elemMetric = elemMetricData.firstChildElement("Metric");
	std::vector<std::string> mdata_names; // vector of metric names
	for( ; !elemMetric.isNull(); elemMetric = elemMetric.nextSiblingElement("Metric")  ) {
		std::string metric_name = elemMetric.attribute("name").toStdString();
		mdata_names.push_back(metric_name);
	}

	// Read Grid
	QDomElement elem = root.firstChildElement("Grid");

	for( ; !elem.isNull(); elem = elem.nextSiblingElement("Grid")  )
	{
		std::string grid_name = elem.attribute("name").toStdString();
		QDomElement gridProps = elem.firstChildElement("Property");
		for (; !gridProps.isNull(); gridProps = gridProps.nextSiblingElement("Property"))
		{
            std::string property_name =
                    gridProps.attribute("name").toStdString();
            props_grid.push_back(MultiDimScalingSpace::grid_prop_pairT(
                                     grid_name,property_name));
			MetricDataCollection mdataCollection =
                    mDataRegistrar->getMetricDataCollection(grid_name,
                                                            property_name, mdata_names);

                        if(mdataCollection.empty()) {
                errors->report( "Some metric data for the property "+
                                property_name+ " on the grid "+grid_name+
                                " could not be found" );
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

	ni = Root::instance()->new_interface( "multi_dim_scaling",mds_manager + "/" + space_name);

	MultiDimScalingSpace*  mdsObject = dynamic_cast<MultiDimScalingSpace*>(ni.raw_ptr());

	mdsObject->initialize(space_name,props_grid,mdata_names);

	// Parse cluster information
	vector<MDSUtil::clusterBlock> kmeansResults;

	QDomElement cluster = root.firstChildElement("Clusters");
	QDomElement clusterUsed = cluster.firstChildElement("ClustersUsed");
	for (;!clusterUsed.isNull(); clusterUsed = clusterUsed.nextSiblingElement("ClustersUsed") )
	{
		// Top level of blocks
		MDSUtil::clusterBlock currentBlock;
		currentBlock.numBlock = clusterUsed.attribute("num").toInt();

		QDomElement clusterID = clusterUsed.firstChildElement("ClusterID");
		for (; !clusterID.isNull(); clusterID = clusterID.nextSiblingElement("ClusterID"))
		{
			std::string currentClustNo = clusterID.attribute("ID").toStdString();
			QDomElement property = clusterID.firstChildElement("Property");
			MDSUtil::cluster currentCluster;

			for (; !property.isNull(); property = property.nextSiblingElement("Property"))
			{
				pair<QString, int> currentProperty;
				currentProperty.first = property.attribute("name");
				currentProperty.second = property.attribute("num").toInt();
				currentCluster.components.push_back(currentProperty);
			}

			currentCluster.numComponents = currentCluster.components.size();

			currentBlock.block.push_back(currentCluster);
		}

		kmeansResults.push_back(currentBlock);
	}

	mdsObject->setKMeanResults(kmeansResults);


	return errors->empty();
}

bool MDS_action::exec() {



	// create upper triangular distance matrix
	MatrixXf distance(num_inputs_, num_inputs_);
	for (int i = 0; i < num_inputs_; i++) {
		for (int j = 0; j < num_inputs_; j++) {
			if (i < j)
				distance(i,j) = mdataCollections_[i].distance(mdataCollections_[j]);
			else
				distance(i,j) = 0.0f;
		}
	}

	float* output_points = new float[MDS_DIMENSIONS * num_inputs_];
	mds mds_algo;
	mds_algo.do_mds(distance, num_inputs_, output_points, MDS_DIMENSIONS);

	delete output_points;

	// update project
	//proj_->new_object( output_pset_name_ );

	return true;
}
