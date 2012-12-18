/*
 * MDSUncertaintySpaceInputFilter.h
 *
 *  Created on: Nov 23, 2011
 *      Author: lewisli
 */

#ifndef MDSUNCERTAINTYSPACEINPUTFILTER_H_
#define MDSUNCERTAINTYSPACEINPUTFILTER_H_

#include <metrics_action_common.h>
#include <filters/MDSUncertaintySpaceFilters.h>

class METRICSACTIONS_DECL MDSUncertaintySpaceInputFilter: public MDSUncertaintySpaceFilters
{
public:
	MDSUncertaintySpaceInputFilter();
	virtual ~MDSUncertaintySpaceInputFilter();

	virtual std::string classname() const { return "MDSUncertaintySpaceInputFilter"; }
	static std::string filtername() { return "FilterMDSInput"; }

	void exec();


};

#endif /* MDSUNCERTAINTYSPACEINPUTFILTER_H_ */
