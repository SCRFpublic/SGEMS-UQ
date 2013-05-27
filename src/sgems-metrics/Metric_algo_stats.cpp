/*
 * Metric_algo_stats.cpp
 *
 *  Created on: Apr 21, 2011
 *      Author: aboucher
 */

#include "Metric_algo_stats.h"
#include <MetricData.h>
#include "ml_library_constants.h"

#include <MetricDataManager.h>
#include <metric_manager_repository.h>

#include <utils/manager.h>
#include <appli/manager_repository.h>
#include <appli/project.h>
#include <utils/error_messages_handler.h>
#include <utils/string_manipulation.h>
#include <grid/grid_region_temp_selector.h>
#include <geostat/utilities.h>
#include <geostat/parameters_handler_impl.h>



bool Metric_algo_stats::initialize( const Parameters_handler* parameters,
	     Error_messages_handler* errors ){
	grid_name_ = parameters->value("GridName.value");
	if(grid_name_.empty()) {
		errors->report("GridName","No grid were selected");
		return false;
	}

	bool ok = geostat_utils::create(grid_,grid_name_,"GridName",errors);
	if( !ok ) {
		errors->report("GridName","The grid " +grid_name_+" does not exist" );
		return false;
	}


	std::string region = parameters->value("GridName.region");
	region_ = grid_->region(region);


  std::string prop_str = parameters->value( "PropertyNames.value" );
  if(prop_str.empty()) {
    errors->report("PropertyNames","No property specified");
    return false;
  }
	std::vector< std::string > prop_name =
            String_Op::decompose_string(prop_str, ";" );

	for(std::vector< std::string >::iterator it_str = prop_name.begin();
    it_str != prop_name.end(); it_str++ ) {
      Grid_continuous_property* prop = grid_->property( *it_str );
      errors->report(!prop, "PropertyNames","No property exist with that name");
      props_.push_back(prop);
  }
	const Parameters_handler_xml* xml_parameters = dynamic_cast<const Parameters_handler_xml*>(parameters);

	paramXml_ = xml_parameters->xmlDomElement("");
    if(paramXml_.isNull())
    {

	}

	return errors->empty();

}

Named_interface* Metric_algo_mean::create_new_interface( std::string& ){
	return new Metric_algo_mean;
}

int Metric_algo_mean::execute( GsTL_project* proj ){
	SmartPtr<Named_interface> ni =
				Root::instance()->interface( metricData_manager + "/metricRegistrar");

	MetricDataManager* mDataRegistrar = dynamic_cast<MetricDataManager*>(ni.raw_ptr());

	for(int i=0; i< props_.size(); ++i) {
		Temporary_propRegion_Selector regionProp(region_,props_[i]);
		float sum = 0.0f;
		int n = 0;

		Grid_continuous_property::iterator it = props_[i]->begin();

		for ( ; it != props_[i]->end(); ++it) {
			sum += *it;
			n++;
		}
		MetricData* metric = new MetricScalarData(props_[i],paramXml_, sum / (float)n);
		mDataRegistrar->registerMetricData(grid_name_,props_[i]->name(),metric);
	}

	return 0;

}


Named_interface* Metric_algo_variance::create_new_interface( std::string& ){
	return new Metric_algo_variance;
}

int Metric_algo_variance::execute( GsTL_project* proj ){
	SmartPtr<Named_interface> ni =
				Root::instance()->interface( metricData_manager + "/metricRegistrar");

	MetricDataManager* mDataRegistrar = dynamic_cast<MetricDataManager*>(ni.raw_ptr());

	for(int i=0; i< props_.size(); ++i) {
		Temporary_propRegion_Selector regionProp(region_,props_[i]);
		float sum = 0.0f;
		float sum2 = 0.0f;
		int n = 0;

		Grid_continuous_property::iterator it = props_[i]->begin();

		for ( ; it != props_[i]->end(); ++it) {
			float v = *it;
			sum += v;
			sum2 += v * v;
			n++;
		}
		float ex = sum / (float)n;
		float ex2 = sum2 / (float) n;
		float variance = ex2 - ex * ex;  // Var[X] = E[X^2] - E[X]^2
		MetricData* metric = new MetricScalarData(props_[i],paramXml_, variance );
		mDataRegistrar->registerMetricData(grid_name_,props_[i]->name(),metric);
	}
	return 0;
}






