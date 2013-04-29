#ifndef MDS_ACTION_H_
#define MDS_ACTION_H_

#include <metrics_action_common.h>
#include <GsTLAppli/actions/action.h> 
#include <GsTLAppli/actions/unary_action.h> 
#include <GsTLAppli/grid/grid_model/grid_region_temp_selector.h>
#include <GsTLAppli/grid/grid_model/grid_property.h>
#include <GsTLAppli/grid/grid_model/rgrid.h>

#include "mdsutil.h"

#include <QtXml>

#include <MetricData.h>

#define MDS_DIMENSIONS 3

class GsTL_project; 

using namespace std;

class METRICSACTIONS_DECL MDS_action :  public Unary_action
{
public: 
  static Named_interface* create_new_interface( std::string& ); 

public:
  MDS_action() : Unary_action("MDS"), metric_names_(0), num_inputs_(0) { }
  virtual ~MDS_action();

  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 

  virtual bool initFromDom(QDomDocument parameters, GsTL_project* proj,
          Error_messages_handler* errors);
  virtual bool exec( void );

protected :
	std::string output_pset_name_;
	vector<string> metric_names_;
    GsTL_project* proj_;
	int num_inputs_;
	std::vector<MetricDataCollection> mdataCollections_;
	  
};
#endif
