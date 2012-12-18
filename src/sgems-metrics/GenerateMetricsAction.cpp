#include "GenerateMetricsAction.h"


#include <MetricFilterPackage.h>

#include <MetricData.h>
#include <MetricDataManager.h>
#include <ml_library_constants.h>

#include <metric_manager_repository.h>

#include <GsTLAppli/actions/defines.h>
#include <GsTLAppli/utils/gstl_messages.h>
#include <GsTLAppli/utils/string_manipulation.h>
#include <GsTLAppli/utils/error_messages_handler.h>
#include <GsTLAppli/utils/string_manipulation.h>
#include <GsTLAppli/grid/grid_model/geostat_grid.h>
#include <GsTLAppli/appli/manager_repository.h>
#include <GsTLAppli/appli/project.h>
#include <GsTL/utils/smartptr.h>

#include <GsTLAppli/geostat/utilities.h>
#include <iostream>
#include <sstream>


using namespace std;

Named_interface* GenerateMetricsAction::create_new_interface( std::string& ){
    return new GenerateMetricsAction;
}

GenerateMetricsAction::~GenerateMetricsAction() {

}

// creates new metrics with name (inputname + "_" + output_postfix)
// Grid::MetricXmlFileName::input1::input2::... :: input_n
bool GenerateMetricsAction::init(std::string& parameters, GsTL_project* proj,
                                 Error_messages_handler* errors)
{


    std::vector< std::string > params =
            String_Op::decompose_string( parameters, Actions::separator,
                                         Actions::unique );

    if( params.size() < 3 ) {
        errors->report( "some parameters are missing" );
        return false;
    }

    proj_ = proj;
    grid_name_ = params[0];
    if(grid_name_.empty()) {
        errors->report( "No grid selected" );
        return false;
    }

    std::string xml_filename;
    xml_filename = params[1];
    if(xml_filename.empty()) {
        errors->report( "No metric filename specified" );
        return false;
    }


    num_inputs_ = params.size()-2;

    // read inputs
    if (num_inputs_ <= 0) {
        errors->report( "Invalid number of inputs." );
        return false;
    }

    if (params.size() != (2+num_inputs_)) {
        errors->report( "Invalid number of inputs." );
        return false;
    }


    // Get the simulation grid from the grid manager

    if( !grid_name_.empty() ) {
	//	Geostat_grid* tempgrid;
        bool ok = geostat_utils::create( grid_, grid_name_,
					 "Grid_Name", errors );
        //		rgrid_ = dynamic_cast<RGrid*>(tempgrid);
        //		if(rgrid_==NULL) errors->report("Grid must be a regular grid" );
        if(!errors->empty()) return false;
    }
    else
        return false;


    //Get the XML definition of the filter
    QDomDocument doc("MetricData");
    qDebug() << " filename : " << QString(xml_filename.c_str());
    QFile file(xml_filename.c_str());
    if (!doc.setContent(&file))
    {
        file.close();
        errors->report( "The file "+xml_filename+" is not a valid xml format file" );
        return false;
    }
    file.close();

    QDomElement root = doc.documentElement();
    QDomElement elem = root.firstChildElement("Filter");
    for( ; !elem.isNull(); elem = elem.nextSiblingElement("Filter")  ) {
        MetricFilter* filter = MetricFilter::CreateMetricFilterFromQDom(elem);
        if( filter == 0 ) {
            errors->report( "Error loading filter" );
            return false;
        }

        filters_.addFilter(filter);
    }

    // verify and save list of input props
    for (int i = 2; i < num_inputs_ + 2; i++) {
        GsTLGridProperty* prop = grid_->select_property( params[i] );

        if (!prop) {
            errors->report("Invalid grid property " + params[i] );
            return false;
        }
        input_props_.push_back( prop  );
    }

    return errors->empty();

}

bool GenerateMetricsAction::exec() {
    SmartPtr<Named_interface> ni =
            Root::instance()->interface( metricData_manager + "/metricRegistrar");

    MetricDataManager* mDataRegistrar = dynamic_cast<MetricDataManager*>(ni.raw_ptr());

    for (int i = 0; i < input_props_.size(); i++) {

        std::vector<MetricFilter*>::iterator it_metric = filters_.begin();
        for( ; it_metric!= filters_.end() ; ++it_metric)
        {
            MetricData* metric = (*it_metric)->computeMetricData(grid_, input_props_[i]);

            mDataRegistrar->registerMetricData(grid_name_,input_props_[i]->name(), metric );
        }

    }

    return true;
}
