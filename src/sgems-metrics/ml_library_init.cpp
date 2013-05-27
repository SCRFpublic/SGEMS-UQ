//#include <utils/gstl_plugins.h>

#include "mds_action.h"
#include "kmeans_action.h"
#include "ml_library_constants.h"
#include "Metric_algo_stats.h"
#include "GenerateMetricsAction.h"
#include "MetricFilter.h"
#include "MetricFilterPackage.h"

#include "filters/MDSUncertaintySpaceFilters.h"
#include "filters/MDSUncertaintySpaceInputFilter.h"
#include "filters/MDSUncertaintySpaceOutputFilter.h"

#include "filters/externalresponseinputfilter.h"
#include "filters/externalresponsefilter.h"

#include "actions/UncertainSpaceIOAction.h"
#include "actions/externalresponseioaction.h"

#include <MetricData.h>
#include <MetricDataManager.h>

#include <GsTL/utils/smartptr.h>
#include <appli/manager_repository.h>


#if defined(_WIN32) || defined(WIN32)
#define METRIC_ALGO_LIB_DECL __declspec(dllexport)
#else
#define METRIC_ALGO_LIB_DECL
#endif


extern "C" METRIC_ALGO_LIB_DECL int plugin_init() {
    GsTLlog << "\n\n registering action mds_action" << "\n";
    SmartPtr<Named_interface> ni =
            Root::instance()->interface( actions_manager );
    Manager* dir = dynamic_cast<Manager*>( ni.raw_ptr() );
    if( !dir ) {
        GsTLlog << "Directory " << actions_manager << " does not exist \n";
        return 1;
    }

    dir->factory( "MDS",
                  MDS_action::create_new_interface );
    GsTLlog << "\n\n registering action kmeans_action" << "\n";
    dir->factory( "kmeans",
                  kmeans_action::create_new_interface );

    // Generate new UncertainSpaceIOaction
    dir ->factory ("MDSFileIO", UncertainSpaceIOAction::create_new_interface);

    // Generate new ExternalResponseIOAction
    dir->factory("ExternalResponseIO",ExternalResponseIOAction::create_new_interface);

    GsTLlog << "\n\n registering action GenerateMetrics" << "\n";

    dir->factory( "GenerateMetrics",
                  GenerateMetricsAction::create_new_interface );


    ni = Root::instance()->interface( geostatAlgo_manager );
    dir = dynamic_cast<Manager*>( ni.raw_ptr() );
    if( !dir ) {
        GsTLlog << "Directory " << geostatAlgo_manager <<
                   " does not exist \n";
        return 1;
    }

    GsTLlog << "\n\n registering algo Metric_algo_mean" << "\n";
    dir->factory( Metric_algo_mean().name(),
                  Metric_algo_mean::create_new_interface );
    GsTLlog << "\n\n registering algo Metric_algo_variance" << "\n";
    dir->factory( Metric_algo_variance().name(),
                  Metric_algo_variance::create_new_interface );

    // register MetricFilter manager
    SmartPtr<Named_interface> mdata_mfilter_ni =
            Root::instance()->new_interface("directory://metricFilter",
                                            metricFilter_manager);

    dir = dynamic_cast<Manager*> (mdata_mfilter_ni.raw_ptr());

    if (!dir)
    {
        GsTLlog << "could not create directory " << metricFilter_manager << "\n";
        return 1;
    }
    dir->factory( "metric_filter", MetricFilter::CreateMetricFilter );

    // register Metric Filter Packages
    Root::instance()->new_interface( "metric_filter://" + MetricFilterMean::filtername(),
                                     metricFilter_manager + "/" + MetricFilterMean::filtername());
    Root::instance()->new_interface( "metric_filter://" + MetricFilterVariance::filtername(),
                                     metricFilter_manager + "/" + MetricFilterVariance::filtername());
    Root::instance()->new_interface( "metric_filter://" + MetricFilterValues::filtername(),
                                     metricFilter_manager + "/" + MetricFilterValues::filtername());

    // register MDSFilter manager
    SmartPtr<Named_interface> MDSIOFilter_ni =
            Root::instance()->new_interface("directory://mdsIOFilter",
                                            MDSUncertFilter_manager);

    dir = dynamic_cast<Manager*> (MDSIOFilter_ni.raw_ptr());

    if (!dir)
    {
        GsTLlog << "could not create directory " << MDSUncertFilter_manager << "\n";
        return 1;
    }

    dir->factory( "MDS_IO_filter", MDSUncertaintySpaceFilters
                  ::CreateMetricFilter );

    // register Metric Filter Packages
    Root::instance()->new_interface( "MDS_IO_filter://"
                                     + MDSUncertaintySpaceOutputFilter
                                     ::filtername(),
                                     MDSUncertFilter_manager + "/" +
                                     MDSUncertaintySpaceOutputFilter
                                     ::filtername());
    // register Metric Filter Packages
    Root::instance()->new_interface( "MDS_IO_filter://" +
                                     MDSUncertaintySpaceInputFilter
                                     ::filtername(),
                                     MDSUncertFilter_manager + "/" +
                                     MDSUncertaintySpaceInputFilter
                                     ::filtername());

    // register External Responses Manager
    SmartPtr<Named_interface> externalResponseFilter_ni =
            Root::instance()->new_interface("directory://ExternalResponseFilter",
                                            externalResponseFilter_manager);

    dir = dynamic_cast<Manager*> (externalResponseFilter_ni.raw_ptr());

    if (!dir)
    {
       std::cerr << "Could not create directory " <<
                   externalResponseFilter_manager << "\n";
        return 1;
    }

    dir->factory( "ExternalResponseFilter",
                  ExternalResponseFilter::CreateExternalResponseFilter);

    Root::instance()->new_interface( "ExternalResponseFilter://" +
                                     ExternalResponseInputFilter
                                     ::filtername(),
                                     externalResponseFilter_manager +
                                     "/" + ExternalResponseInputFilter
                                     ::filtername());

    return 0;
}
