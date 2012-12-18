#include "externalresponsefilter.h"
#include <filters/externalresponseinputfilter.h>

ExternalResponseFilter::ExternalResponseFilter()
{
}

Named_interface* ExternalResponseFilter::CreateExternalResponseFilter(
    std::string& s)
{

        if (s.compare(ExternalResponseInputFilter::filtername()) == 0)
                return new ExternalResponseInputFilter();
        else
                return 0;
}

void ExternalResponseFilter::exec()
{
        return;
}

bool ExternalResponseFilter::loadParameters(QDomDocument *parameters,
                                                std::string filename)
{
        this->paramXml_ = *parameters;
        this->filename = filename;
        return true;
}
