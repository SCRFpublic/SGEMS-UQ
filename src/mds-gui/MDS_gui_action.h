/*
 * MDS_gui_action.h
 *
 *  Created on: Apr 20, 2011
 *      Author: aboucher
 */

#ifndef MDS_GUI_ACTION_H_
#define MDS_GUI_ACTION_H_

#include <common.h>
#include <utils/gstl_plugins.h>
#include <actions/common.h>
#include <actions/action.h>


class GsTL_project;

class METRICSGUI_DECL MDS_gui_action :  public Action
{
public:
	static Named_interface* create_new_interface( std::string& );

public:
	MDS_gui_action();
	virtual ~MDS_gui_action();

	virtual bool init( std::string& parameters, GsTL_project* proj,
										 Error_messages_handler* errors );
	virtual bool exec( );

protected :
	GsTL_project* proj_;



};



#endif /* MDS_GUI_ACTION_H_ */
