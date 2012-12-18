/*
 * MDSUncertainitySpaceOutputFilter.h
 *
 *  Created on: Nov 23, 2011
 *      Author: lewisli
 */

#ifndef MDSUNCERTAINITYSPACEOUTPUTFILTER_H_
#define MDSUNCERTAINITYSPACEOUTPUTFILTER_H_

#include <metrics_action_common.h>
#include <filters/MDSUncertaintySpaceFilters.h>


class METRICSACTIONS_DECL MDSUncertaintySpaceOutputFilter : public MDSUncertaintySpaceFilters{
public:
	MDSUncertaintySpaceOutputFilter();
	virtual ~MDSUncertaintySpaceOutputFilter();

	virtual std::string classname() const { return "MDSUncertaintySpaceOutputFilter"; }
	static std::string filtername() { return "FilterMDSOutput"; }

	void exec();


};

#endif /* MDSUNCERTAINITYSPACEOUTPUTFILTER_H_ */
