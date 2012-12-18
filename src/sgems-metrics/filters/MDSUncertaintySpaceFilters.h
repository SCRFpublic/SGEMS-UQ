/*
 * MDSUncertainitySpaceFilters.h
 *
 *  Created on: Nov 23, 2011
 *      Author: lewisli
 */

#ifndef MDSUNCERTAINITYSPACEFILTERS_H_
#define MDSUNCERTAINITYSPACEFILTERS_H_

#include <metrics_action_common.h>
#include <iostream>
#include <GsTLAppli/utils/named_interface.h>
#include <GsTLAppli/grid/grid_model/geostat_grid.h>
#include <GsTLAppli/grid/grid_model/grid_property.h>

#include <GsTLAppli/actions/defines.h>
#include <GsTLAppli/utils/gstl_messages.h>
#include <GsTLAppli/utils/string_manipulation.h>
#include <GsTLAppli/utils/error_messages_handler.h>
#include <GsTLAppli/utils/string_manipulation.h>
#include <GsTLAppli/grid/grid_model/geostat_grid.h>
#include <GsTLAppli/grid/grid_model/point_set.h>
#include <GsTLAppli/appli/manager_repository.h>
#include <GsTLAppli/appli/project.h>
#include <GsTL/utils/smartptr.h>

#include <ml_library_constants.h>

#include <mds_action.h>

#include <QtXml>

class METRICSACTIONS_DECL MDSUncertaintySpaceFilters : public Named_interface{
public:
	MDSUncertaintySpaceFilters();
	virtual ~MDSUncertaintySpaceFilters();

	// i/o
	static Named_interface* CreateMetricFilter(std::string&);

	//bool initialize(std::string filename);
	//virtual bool initialize(const QDomElement& root);

	bool loadParameters(QDomDocument *parameters, std::string filename);

	virtual void exec();
	virtual std::string classname() const { return "MDSUncertaintySpaceFilters"; }

	protected :
	QDomDocument paramXml_;
	std::string filename;
	GsTL_project* proj_;

};

#endif /* MDSUNCERTAINITYSPACEFILTERS_H_ */
