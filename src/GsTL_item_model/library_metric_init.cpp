/*
 * library_init.cpp
 *
 *  Created on: Apr 23, 2011
 *      Author: aboucher
 */


#include "metric_manager_repository.h"
#include "MetricData.h"
#include "MetricDataManager.h"
#include "multi_dim_scaling_space.h"
#include "GenericKernels.h"
#include <Metric_data_io_filter.h>
#include <mds_space_filters.h>
#include <Metric_data_actions.h>
#include <GsTL/utils/smartptr.h>
#include <GsTLAppli/appli/manager_repository.h>
#include <GsTLAppli/filters/save_project_objects.h>


#if defined(_WIN32) || defined(WIN32)
#define ITEM_MODEL_LIB_DECL __declspec(dllexport)
#else
#define ITEM_MODEL_LIB_DECL
#endif


extern "C" ITEM_MODEL_LIB_DECL int plugin_init() {
    // register MetricData manager


    GsTLlog << "\n\n registering data metric library" << "\n";

    // register kernel manager
    SmartPtr<Named_interface> mdata_manager_ni =
            Root::instance()->new_interface("directory://metric_data", metricData_manager);

    Manager* dir = dynamic_cast<Manager*>( mdata_manager_ni.raw_ptr() );

    if (!dir)
    {
        GsTLlog << "could not create directory " << metricData_manager << "\n";
        return 1;
    }

    GsTLlog << "\n\n registering Scalar data metric" << "\n";
    dir->factory( MetricScalarData().dataFormat(),
                  MetricScalarData::create_new_interface );
    GsTLlog << "\n\n registering vector data" << "\n";
    dir->factory( MetricVectorData().dataFormat(),
                  MetricVectorData::create_new_interface );
    GsTLlog << "\n\n registering time series data" << "\n";
    dir->factory( MetricTimeSeriesData().dataFormat(),
                  MetricTimeSeriesData::create_new_interface );
    dir->factory( MetricContinuousDistributionData().dataFormat(),
                  MetricContinuousDistributionData::create_new_interface );

    //	dir->factory( "metric_data", MetricData::create_new_interface );

    // register Metric data Registrar
    dir->factory( "metric_data_manager", MetricDataManager::create_new_interface );
    SmartPtr<Named_interface> metric_registrar_ni =
            Root::instance()->new_interface("metric_data_manager://metricRegistrar",
                                            metricData_manager + "/metricRegistrar");
    if (!metric_registrar_ni.raw_ptr()) {
        GsTLlog << "could not create metric registrar\n";
        return 1;
    }


    GsTLlog << "\n\n registering kernel library" << "\n";
    // register kernel manager
    SmartPtr<Named_interface> kernel_manager_ni =
            Root::instance()->new_interface("directory://kernel", kernel_manager);

    dir = dynamic_cast<Manager*>( kernel_manager_ni.raw_ptr() );

    if (!dir)
    {
        GsTLlog << "could not create directory " << kernel_manager << "\n";
        return 1;
    }

    GsTLlog << "\n\n registering kernel Euclidean_kernel" << "\n";
    dir->factory( Euclidean_kernel().name(),
                  Euclidean_kernel::create_new_interface );
    GsTLlog << "\n\n registering Manhattan_kernel" << "\n";
    dir->factory( Manhattan_kernel().name(),
                  Manhattan_kernel::create_new_interface );
    GsTLlog << "\n\n registering kernel Gaussian_kernel" << "\n";
    dir->factory( Gaussian_kernel().name(),
                  Gaussian_kernel::create_new_interface );


    GsTLlog << "\n\n registering MDS Space" << "\n";
    // register mds manager
    SmartPtr<Named_interface> mds_manager_ni =
            Root::instance()->new_interface("directory", mds_manager);

    dir = dynamic_cast<Manager*>( mds_manager_ni.raw_ptr() );

    if (!dir)
    {
        GsTLlog << "could not create directory " << mds_manager << "\n";
        return 1;
    }

    GsTLlog << "\n\n registering MDS Space" << "\n";
    dir->factory( "multi_dim_scaling",
                  MultiDimScalingSpace::create_new_interface );



    // register the filter to load/save the metric data

    SmartPtr<Named_interface> outfilter_ni =
            Root::instance()->interface( outfilters_manager );
    dir = dynamic_cast<Manager*>( outfilter_ni.raw_ptr() );
    appli_assert( dir );

    dir->factory( Metric_data_output_filter().filter_name(), Metric_data_output_filter::create_new_interface );
    dir->factory( Metric_data_project_output_filter().filter_name(), Metric_data_project_output_filter::create_new_interface );
    dir->factory( Mds_space_output_filter().filter_name(), Mds_space_output_filter::create_new_interface );

    SmartPtr<Named_interface> infilter_ni =
            Root::instance()->interface( topLevelInputFilters_manager );
    dir = dynamic_cast<Manager*>( infilter_ni.raw_ptr() );
    appli_assert( dir );

    dir->factory( Metric_data_input_filter().filter_name(), Metric_data_input_filter::create_new_interface );
    dir->factory( Metric_data_project_input_filter().filter_name(), Metric_data_project_input_filter::create_new_interface );
    dir->factory( Mds_space_input_filter().filter_name(), Mds_space_input_filter::create_new_interface );

    // register the action to load/save the metric data
    SmartPtr<Named_interface> action_ni =
            Root::instance()->interface( actions_manager );
    dir = dynamic_cast<Manager*>( action_ni.raw_ptr() );
    appli_assert( dir );
    dir->factory( "LoadMetricData", Load_data_metric_action::create_new_interface );
    dir->factory( "SaveMetricData", Save_data_metric_action::create_new_interface );
    dir->factory( "LoadMetricSpace", Load_mds_space_action::create_new_interface );
    dir->factory( "SaveMetricSpace", Save_mds_space_action::create_new_interface );


    //Regsiter the Metric_data_project_output_filter for the project saving
    SmartPtr<Named_interface> project_filter_ni =
            Root::instance()->interface(project_filters_manager+"/"+"project_filters");
    Save_project_objects* project_filter = dynamic_cast<Save_project_objects*>(project_filter_ni.raw_ptr());

    project_filter->add_directory(metricData_manager,
                                  std::make_pair(Metric_data_project_output_filter().filter_name(),outfilters_manager+"/")) ;

    return 0;

}
