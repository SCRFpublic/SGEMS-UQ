#include "Metric_data_actions.h"

#include <Metric_data_io_filter.h>
#include <mds_space_filters.h>
#include <metric_manager_repository.h>
#include <MetricDataManager.h>
#include <multi_dim_scaling_space.h>

#include <actions/defines.h>
#include <utils/string_manipulation.h>
#include <appli/manager_repository.h>


Named_interface* Load_data_metric_action::create_new_interface(std::string&){
  return new Load_data_metric_action;
}

bool Load_data_metric_action::init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ){

  std::vector< std::string > params = 
    String_Op::decompose_string( parameters, Actions::separator,
                      				   Actions::unique );

  if( params.empty() )  {
    errors->report( "No file specified" ); 
    return false;
  }
  filename_ = params[0];

  Metric_data_input_filter in_filter;

  bool ok = in_filter.can_handle(filename_);

  if(!ok) {
    errors->report( "Error with the file formatting of "+filename_ ); 
    return false;
  }

  return true;

}


bool Load_data_metric_action::exec(){
  Metric_data_input_filter in_filter;

  Named_interface* ni = in_filter.read(filename_);

  return ni!=0;
}


/*
  ------------------------------------------------------------
*/


Named_interface* Save_data_metric_action::create_new_interface(std::string&){
  return new Save_data_metric_action;
}

bool Save_data_metric_action::init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ){

  std::vector< std::string > params = 
    String_Op::decompose_string( parameters, Actions::separator,
                      				   Actions::unique );

  if( params.empty()  )  {
    errors->report( "Need at least one parameter (filename)" ); 
    return false;
  }
  filename_ = params[0];

  return true;

}


bool Save_data_metric_action::exec(){

  SmartPtr<Named_interface> ni = 
    Root::instance()->interface( metricData_manager + "/metricRegistrar");

  MetricDataManager* md_manager = dynamic_cast<MetricDataManager*>(ni.raw_ptr());
  
  if(md_manager==0) return false;

  Metric_data_output_filter out_filter;

  std::string errors;
  return out_filter.write(filename_,md_manager,&errors);

}


/*
  ---------------------------------------
*/

Named_interface* Load_mds_space_action::create_new_interface(std::string&){
  return new Load_mds_space_action;
}

bool Load_mds_space_action::init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ){

  std::vector< std::string > params = 
    String_Op::decompose_string( parameters, Actions::separator,
                      				   Actions::unique );

  if( params.empty() )  {
    errors->report( "No file specified" ); 
    return false;
  }
  filename_ = params[0];

  Mds_space_input_filter in_filter;

  bool ok = in_filter.can_handle(filename_);

  if(!ok) {
    errors->report( "Error with the file formatting of "+filename_ ); 
    return false;
  }

  return true;

}


bool Load_mds_space_action::exec(){
  Mds_space_input_filter in_filter;

  Named_interface* ni = in_filter.read(filename_);

  return ni!=0;
}



/*
  ------------------------------------------------------------
*/


Named_interface* Save_mds_space_action::create_new_interface(std::string&){
  return new Save_mds_space_action;
}

bool Save_mds_space_action::init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ){

  std::vector< std::string > params = 
    String_Op::decompose_string( parameters, Actions::separator,
                      				   Actions::unique );

  if( params.size() != 2  )  {
    errors->report( "Need two parameters (filename and name of the mds_space)" ); 
    return false;
  }
  filename_ = params[0];
  mds_space_name_ = params[0];

  SmartPtr<Named_interface> ni = 
    Root::instance()->interface( mdsSpace_manager + "/"+mds_space_name_);

  mds_space_ = dynamic_cast<MultiDimScalingSpace*>(ni.raw_ptr());

  if(mds_space_ == 0) return false;

  return true;

}


bool Save_mds_space_action::exec(){

  Mds_space_output_filter out_filter;

  std::string errors;
  return out_filter.write(filename_,mds_space_,&errors);

}
