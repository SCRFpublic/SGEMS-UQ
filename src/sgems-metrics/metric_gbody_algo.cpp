#include "metric_gbody_algo.h"

#include <MetricData.h>
#include "ml_library_constants.h"

#include <MetricDataManager.h>
#include <metric_manager_repository.h>

#include <GsTLAppli/utils/manager.h>
#include <GsTLAppli/appli/manager_repository.h>
#include <GsTLAppli/appli/project.h>
#include <GsTLAppli/utils/error_messages_handler.h>
#include <GsTLAppli/utils/string_manipulation.h>
#include <GsTLAppli/grid/grid_model/grid_region_temp_selector.h>
#include <GsTLAppli/geostat/utilities.h>
#include <GsTLAppli/geostat/parameters_handler_impl.h>


#include <algorithm>


Metric_gbody_algo::Metric_gbody_algo(void)
{
}


Metric_gbody_algo::~Metric_gbody_algo(void)
{
}


bool Metric_gbody_algo::initialize( const Parameters_handler* parameters,
		     Error_messages_handler* errors ){

	std::string grid_name = parameters->value("GridName.value");
	if(grid_name.empty()) {
		errors->report("GridName","No grid were selected");
		return false;
	}

	bool ok = geostat_utils::create(grid_,grid_name,"GridName",errors);
	if( !ok ) {
		errors->report("GridName","The grid " +grid_name+" does not exist" );
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
      GsTLGridProperty* prop = grid_->property( *it_str );
      errors->report(!prop, "PropertyNames","No property exist with that name");
      props_.push_back(prop);
  }   


  prop_str = parameters->value( "GeobodyPropertyNames.value" );
  if(prop_str.empty()) {
    errors->report("GeobodyPropertyNames","No property specified");
    return false;
  }
	std::vector< std::string > gbody_prop_name =
            String_Op::decompose_string(prop_str, ";" );

	for(std::vector< std::string >::iterator it_str = gbody_prop_name.begin();
    it_str != gbody_prop_name.end(); it_str++ ) {
      GsTLGridProperty* prop = grid_->property( *it_str );
      errors->report(!prop, "GeobodyPropertyNames","No property exist with that name");
      gbody_props_.push_back(prop);
  } 

  metric_base_name_ = parameters->value( "metric_base_name.value" );

  return true;

}


int Metric_gbody_algo::execute( GsTL_project* proj ){

  std::vector<const GsTLGridProperty*>::const_iterator it_gbody  = gbody_props_.begin();
  std::vector<GsTLGridProperty*>::iterator it_prop  = props_.begin();

  for( ; it_prop!=props_.end(); ++it_prop, it_gbody++) {

    //Get the number of geobody
    //Get the size distribution of geobody
    // Get statistic from geobody

    GsTLGridProperty::const_iterator it = (*it_gbody)->begin();
    int ngbody = -99;
    for( ; it != (*it_gbody)->end(); ++it) {
      if(  *it > ngbody ) ngbody = *it;
    }

    std::vector<int> gbody_distribution(ngbody+1,0);

    for(it = (*it_gbody)->begin(); it != (*it_gbody)->end(); ++it ) {
      gbody_distribution[static_cast<int>(*it)]++;
    }

    std::sort(gbody_distribution.begin(), gbody_distribution.end());

    float mean = std::accumulate(gbody_distribution.begin(), gbody_distribution.end(), 0.0)/gbody_distribution.size();
    float median = gbody_distribution[static_cast<int>(gbody_distribution.size()/2)];
    float max_size = gbody_distribution.back();


  }

  return 0;
}