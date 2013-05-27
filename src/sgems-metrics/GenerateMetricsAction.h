#ifndef GENERATE_METRICS_ACTION_H_
#define GENERATE_METRICS_ACTION_H_


#include <actions/action.h> 
//#include <actions/unary_action.h> 
#include <grid/grid_region_temp_selector.h>
#include <grid/grid_property.h>
#include <grid/rgrid.h>
#include <metrics_action_common.h>
#include <MetricFilterCollection.h>
#include <GenericKernels.h>

class GsTL_project; 

class METRICSACTIONS_DECL GenerateMetricsAction :  public Action
{
public: 
  static Named_interface* create_new_interface( std::string& ); 

public:
  GenerateMetricsAction() :  input_props_(0), num_inputs_(0) { }
  virtual ~GenerateMetricsAction();

  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec( void );

protected :
	std::vector<Grid_continuous_property*> input_props_;
	std::string grid_name_;
	std::string metric_name_;
	Geostat_grid* grid_;
  GsTL_project* proj_;
	int num_inputs_;
	MetricFilterCollection filters_;
};

#endif
