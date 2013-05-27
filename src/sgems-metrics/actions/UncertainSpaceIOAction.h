/*
 * UncertainSpaceIOAction.h
 *
 *  Created on: Nov 23, 2011
 *      Author: lewisli
 */

#ifndef UNCERTAINSPACEIOACTION_H_
#define UNCERTAINSPACEIOACTION_H_

#include <metrics_action_common.h>
#include <MetricData.h>
#include <MetricDataManager.h>
#include <ml_library_constants.h>

#include <metric_manager_repository.h>

#include <actions/common.h>
#include <actions/action.h>
#include <actions/unary_action.h>

#include <actions/defines.h>
#include <utils/gstl_messages.h>
#include <utils/string_manipulation.h>
#include <utils/error_messages_handler.h>
#include <utils/string_manipulation.h>
#include <grid/geostat_grid.h>
#include <appli/manager_repository.h>
#include <appli/project.h>
#include <GsTL/utils/smartptr.h>

#include <QtXml>
#include <filters/MDSUncertaintySpaceInputFilter.h>
#include <filters/MDSUncertaintySpaceOutputFilter.h>

#define READ 0
#define WRITE 1

class METRICSACTIONS_DECL UncertainSpaceIOAction : public Action{
public:
	UncertainSpaceIOAction();
	virtual ~UncertainSpaceIOAction();

public:
  static Named_interface* create_new_interface( std::string& );

public:
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors );
  virtual bool initFromDom(QDomDocument *parameters, std::string &filename);

  virtual bool exec( void );

protected :
  //QDomDocument *configurationParameters;
  bool mode;

};

#endif /* UNCERTAINSPACEIOACTION_H_ */
