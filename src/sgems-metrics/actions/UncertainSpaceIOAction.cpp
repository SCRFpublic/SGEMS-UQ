/*
 * UncertainSpaceIOAction.cpp
 *
 *  Created on: Nov 23, 2011
 *      Author: lewisli
 */

#include <actions/UncertainSpaceIOAction.h>
using namespace std;

#include <iostream>
UncertainSpaceIOAction::UncertainSpaceIOAction() {
	// TODO Auto-generated constructor stub

}

UncertainSpaceIOAction::~UncertainSpaceIOAction() {
	// TODO Auto-generated destructor stub
}


Named_interface* UncertainSpaceIOAction::create_new_interface( std::string&){

	return new UncertainSpaceIOAction;

}

bool UncertainSpaceIOAction::init(std::string& parameters, GsTL_project* proj,
                         Error_messages_handler* errors) {

	// Parse parameter to see if we are writing or reading
	if (parameters == "Write")
	{
		this->mode = WRITE;
		cout << "Using Write Filter" << endl;
		return true;
	}
	else if (parameters == "Read")
	{
		this->mode = READ;
		cout << "Using Read Filter" << endl;
		return true;
	}
	else
	{
		cout << "Mode Not Recongized" << endl;
		return false;
	}
}

bool UncertainSpaceIOAction::exec() {
	cout << "UncertainSpaceIOAction::exec" << endl;
  return true;
}

bool UncertainSpaceIOAction::initFromDom(QDomDocument *parameters, std::string &filename)
{

	if (this->mode == READ)
	{
		 SmartPtr<Named_interface> ni =
			    Root::instance()->interface(MDSUncertFilter_manager + "/" + MDSUncertaintySpaceInputFilter::filtername() );
		 MDSUncertaintySpaceInputFilter* filter = dynamic_cast<MDSUncertaintySpaceInputFilter*>( ni.raw_ptr() );
		 filter->loadParameters(parameters, filename);
		 filter->exec();
	}

	else if (this->mode == WRITE)
	{
		 SmartPtr<Named_interface> ni =
			    Root::instance()->interface(MDSUncertFilter_manager + "/" + MDSUncertaintySpaceOutputFilter::filtername() );
		 MDSUncertaintySpaceOutputFilter* filter = dynamic_cast<MDSUncertaintySpaceOutputFilter*>( ni.raw_ptr() );
		 filter->loadParameters(parameters, filename);
		 filter->exec();
	}

  return true;

	//  cout << MDSUncertFilter_manager + "/" + MDSUncertaintySpaceInputFilter::filtername() << endl;

	 //  dynamic cast doesn't seem to work here, instead use static cast

//
	//  filter->loadParameters(parameters, filename);

}

