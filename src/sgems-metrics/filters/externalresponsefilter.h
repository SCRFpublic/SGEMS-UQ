#ifndef EXTERNALRESPONSEFILTER_H
#define EXTERNALRESPONSEFILTER_H

#include <metrics_action_common.h>
#include <iostream>
#include <utils/named_interface.h>
#include <grid/geostat_grid.h>
#include <grid/grid_property.h>

#include <actions/defines.h>
#include <utils/gstl_messages.h>
#include <utils/string_manipulation.h>
#include <utils/error_messages_handler.h>
#include <utils/string_manipulation.h>
#include <grid/geostat_grid.h>
#include <grid/point_set.h>
#include <appli/manager_repository.h>
#include <appli/project.h>
#include <GsTL/utils/smartptr.h>

#include <ml_library_constants.h>

#include <metric_manager_repository.h>
#include <MetricDataManager.h>

#include <mds_action.h>

#include <QtXml>
class ExternalResponseFilter : public Named_interface
{
public:
    ExternalResponseFilter();

    // i/o
    static Named_interface* CreateExternalResponseFilter(std::string&);

    bool loadParameters(QDomDocument *parameters, std::string filename);

    virtual void exec();

    virtual std::string classname() const { return "ExternalResponseFilters"; }

    protected :
    QDomDocument paramXml_;
    std::string filename;
    GsTL_project* proj_;
};

#endif // EXTERNALRESPONSEFILTER_H
