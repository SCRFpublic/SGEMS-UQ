#ifndef KMEANS_ACTION_H_
#define KMEANS_ACTION_H_

#include <metrics_action_common.h>
#include <GsTLAppli/actions/common.h>
#include <GsTLAppli/actions/action.h> 
#include <GsTLAppli/actions/unary_action.h> 
#include <GsTLAppli/grid/grid_model/grid_region_temp_selector.h>
#include <GsTLAppli/grid/grid_model/grid_property.h>
#include <GsTLAppli/grid/grid_model/rgrid.h>

#include <MetricData.h>

class GsTL_project; 

using namespace std;

class METRICSACTIONS_DECL kmeans_action :  public Unary_action
{
public: 
  static Named_interface* create_new_interface( std::string& ); 

public:
  kmeans_action() : Unary_action("kmeans"), metric_names_(0), num_inputs_(0) { }
  virtual ~kmeans_action();

  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec( void );

protected :
	std::string output_pset_name_;
	vector<string> metric_names_;
    GsTL_project* proj_;
	int num_inputs_;
	int num_clusters_;
	std::vector<MetricDataCollection> mdataCollections_;
	  
};
#endif
