#ifndef EXTERNALRESPONSEINPUTFILTER_H
#define EXTERNALRESPONSEINPUTFILTER_H

#include "filters/externalresponsefilter.h"
#include "MetricData.h"

class ExternalResponseInputFilter : public ExternalResponseFilter
{
public:
    ExternalResponseInputFilter();

    //virtual ~MDSUncertaintySpaceInputFilter();

    virtual std::string classname() const {
        return "ExternalResponseInputFilter"; }
    static std::string filtername() { return "FilterExternalResponseInput"; }

    void exec();
};

#endif // EXTERNALRESPONSEINPUTFILTER_H
