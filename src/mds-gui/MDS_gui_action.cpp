/*
 * MDS_gui_action.cpp
 *
 *  Created on: Apr 20, 2011
 *      Author: aboucher
 */

#include "MDS_gui_action.h"
#include "mds_gui.h"

#include <GsTL/utils/smartptr.h>
#include <appli/manager_repository.h>
#include <appli/project.h>

Named_interface* MDS_gui_action::create_new_interface( std::string& ){
    return new MDS_gui_action;
}


MDS_gui_action::MDS_gui_action():proj_(0) {
    // TODO Auto-generated constructor stub
}

MDS_gui_action::~MDS_gui_action() {
    // TODO Auto-generated destructor stub
}

bool MDS_gui_action::exec() {
    mds_gui* g = new mds_gui(proj_,0);
    g->show();

    return true;
}

bool MDS_gui_action::init(std::string& parameters, GsTL_project* proj,
                          Error_messages_handler* errors){
    proj_ = proj;
    return true;
}

/*
#if defined(_WIN32) || defined(WIN32)
 #define MDS_GUI_LIB_DECL __declspec(dllexport)
#else
 #define MDS_GUI_LIB_DECL
#endif

extern "C"  MDS_GUI_LIB_DECL int plugin_init() {
  GsTLlog << "\n\n registering actions mds_action" << "\n";
  SmartPtr<Named_interface> ni =
    Root::instance()->interface( actions_manager );
  Manager* dir = dynamic_cast<Manager*>( ni.raw_ptr() );
  if( !dir ) {
    GsTLlog << "Directory " << actions_manager << " does not exist \n";
    return 1;
  }

  GsTLlog << "\n\n registering action MDS_gui" << "\n";
  dir->factory( "MDS_gui",
    MDS_gui_action::create_new_interface );


}
*/
