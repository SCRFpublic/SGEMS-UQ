/*
 * Metric_gbody_algo.h
 *
 *  Created on: Apr 18, 2012
 *      Author: aboucher
 */

#ifndef METRIC_ALGO_STATS_H_
#define METRIC_ALGO_STATS_H_

#include <metrics_action_common.h>
#include <Metric_algo.h>
#include <GsTLAppli/grid/grid_model/geostat_grid.h>
#include <GsTLAppli/grid/grid_model/grid_property.h>
#include <GsTLAppli/grid/grid_model/grid_region.h>

//#include "../Geobody/geobody_action.h"



class Metric_gbody_algo :
  public Metric_algo
{
public:
  Metric_gbody_algo(void);
  virtual ~Metric_gbody_algo(void);


  virtual bool initialize( const Parameters_handler* parameters,
		     Error_messages_handler* errors );

  /** Runs the algorithm.
   * @return 0 if the run was successful
   */
  virtual int execute( GsTL_project* proj=0 );

  /** Tells the name of the algorithm
   */
  virtual std::string name() const { return "geobody-stats"; }


private :

  std::string metric_base_name_;

  GsTLGridRegion* region_;
  Geostat_grid* grid_;

  std::vector<const GsTLGridProperty*> gbody_props_;
  std::vector<GsTLGridProperty*> props_;

//  Geobody_action gbody_algo_;

};

#endif