/*
 * MDSUncertainitySpaceFilters.cpp
 *
 *  Created on: Nov 23, 2011
 *      Author: lewisli
 */

#include <filters/MDSUncertaintySpaceInputFilter.h>
#include <filters/MDSUncertaintySpaceOutputFilter.h>
#include <filters/MDSUncertaintySpaceFilters.h>


MDSUncertaintySpaceFilters::MDSUncertaintySpaceFilters() {
    // TODO Auto-generated constructor stub

}

MDSUncertaintySpaceFilters::~MDSUncertaintySpaceFilters()
{
    // TODO Auto-generated destructor stub
}

Named_interface* MDSUncertaintySpaceFilters::CreateMetricFilter(std::string& s)
{

    if (s.compare(MDSUncertaintySpaceInputFilter::filtername()) == 0)
        return new MDSUncertaintySpaceInputFilter();
    else if (s.compare(MDSUncertaintySpaceOutputFilter::filtername()) == 0)
        return new MDSUncertaintySpaceOutputFilter();
    else
        return 0;
}

void MDSUncertaintySpaceFilters::exec()
{
    return;
}

bool MDSUncertaintySpaceFilters::loadParameters(QDomDocument *parameters,
                                                std::string filename)
{
    this->paramXml_ = *parameters;
    this->filename = filename;
    return true;
}
