/*
 * Metric_algo.h
 *
 *  Created on: Apr 21, 2011
 *      Author: aboucher
 */

#ifndef METRIC_ALGO_H_
#define METRIC_ALGO_H_

#include <metrics_action_common.h>
#include <geostat/geostat_algo.h>
#include <geostat/common.h>
#include <utils/named_interface.h>

#include <QtXml/QtXml>

class Error_messages_handler;
class Parameters_handler;
class GsTL_project;


class METRICSACTIONS_DECL Metric_algo: public Geostat_algo {
public:
	Metric_algo(){}
	virtual ~Metric_algo(){}

  virtual bool initialize( const Parameters_handler* parameters,
		     Error_messages_handler* errors ) = 0;

  /** Runs the algorithm.
   * @return 0 if the run was successful
   */
  virtual int execute( GsTL_project* proj=0 ) = 0;

  /** Tells the name of the algorithm
   */
  virtual std::string name() const { return ""; }

protected :
  QDomElement paramXml_;

};

#endif /* METRIC_ALGO_H_ */
