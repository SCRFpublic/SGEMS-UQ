
#include <common.h>
//#include <MDS_gui_action.h>
#include "metricmanager/eda_response.h"
#include "eda_mds_space.h""
#include <appli/manager_repository.h>
#include <utils/gstl_messages.h>

extern "C"  METRICSGUI_DECL int plugin_init() {
  GsTLlog << "\n\n registering actions mds_action" << "\n";
  SmartPtr<Named_interface> ni =
    Root::instance()->interface( actions_manager );
  Manager* dir = dynamic_cast<Manager*>( ni.raw_ptr() );
  if( !dir ) {
    GsTLlog << "Directory " << actions_manager << " does not exist \n";
    return 1;
  }
 /*
  GsTLlog << "\n\n registering action MDS_gui" << "\n";
  dir->factory( "MDS_gui",
  		MDS_gui_action::create_new_interface );
      */
  ni = 	Root::instance()->interface( eda_charts_manager );
  dir = dynamic_cast<Manager*>( ni.raw_ptr() );
  if( !dir ) {
	  GsTLlog << "Directory " << eda_charts_manager << " does not exist \n";
	  return 1;
  }
  dir->factory( EDA_response_factory().name(), 
                EDA_response_factory::create_new_interface);

  dir->factory( EDA_mds_space_factory().name(), 
                EDA_mds_space_factory::create_new_interface);

  return 0;
}
