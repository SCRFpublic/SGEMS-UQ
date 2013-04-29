/*
 * Metric_algo_stats.h
 *
 *  Created on: Apr 21, 2011
 *      Author: aboucher
 */

#ifndef METRIC_ALGO_STATS_H_
#define METRIC_ALGO_STATS_H_

#include <metrics_action_common.h>
#include <Metric_algo.h>
#include <GsTLAppli/grid/grid_model/geostat_grid.h>
#include <GsTLAppli/grid/grid_model/grid_property.h>
#include <GsTLAppli/grid/grid_model/grid_region.h>

class METRICSACTIONS_DECL Metric_algo_stats : public Metric_algo {
public:
	Metric_algo_stats() {};
	virtual ~Metric_algo_stats() {};


  virtual bool initialize( const Parameters_handler* parameters,
		     Error_messages_handler* errors );

  /** Runs the algorithm.
   * @return 0 if the run was successful
   */
  virtual int execute( GsTL_project* proj=0 ) = 0;

  /** Tells the name of the algorithm
   */
  virtual std::string name() const =0;

protected :
  Geostat_grid* grid_;
  std::string grid_name_;
  std::vector<GsTLGridProperty*> props_;
  GsTLGridRegion* region_;

};


class Metric_algo_mean : public Metric_algo_stats {
public:
	Metric_algo_mean() : Metric_algo_stats(){}
	virtual ~Metric_algo_mean() {};

	static Named_interface* create_new_interface( std::string& );


  /** Runs the algorithm.
   * @return 0 if the run was successful
   */
  virtual int execute( GsTL_project* proj=0 );

  /** Tells the name of the algorithm
   */
  virtual std::string name() const { return "MeanData"; }

protected :


};


class Metric_algo_variance : public Metric_algo_stats {
public:
	Metric_algo_variance() : Metric_algo_stats(){}
	virtual ~Metric_algo_variance() {};

	static Named_interface* create_new_interface( std::string& );


  /** Runs the algorithm.
   * @return 0 if the run was successful
   */
  virtual int execute( GsTL_project* proj=0 );

  /** Tells the name of the algorithm
   */
  virtual std::string name() const { return "VarianceData"; }

protected :

};


#endif /* METRIC_ALGO_STATS_H_ */
