/*
 * MDSUncertaintySpaceInputFilter.cpp
 *
 *  Created on: Nov 23, 2011
 *      Author: lewisli
 */

#include <filters/MDSUncertaintySpaceInputFilter.h>

MDSUncertaintySpaceInputFilter::MDSUncertaintySpaceInputFilter() {
	// TODO Auto-generated constructor stub

}

MDSUncertaintySpaceInputFilter::~MDSUncertaintySpaceInputFilter() {
	// TODO Auto-generated destructor stub
}

void MDSUncertaintySpaceInputFilter::exec()
{
	QFile openFile(QString::fromStdString(this->filename));

	if (!openFile.open(QIODevice::ReadOnly))
	{
		return;

	}
	if (!this->paramXml_.setContent(&openFile))
	{
		openFile.close();
		return;
	}
	openFile.close();

	std::string mds_manager = "/mds";

	SmartPtr<Named_interface> ni =
			Root::instance()->new_interface("mds_action",mds_manager + "/temp");
	MDS_action* newAction = dynamic_cast<MDS_action*> (ni.raw_ptr());
	Error_messages_handler temp;

	newAction->initFromDom(this->paramXml_,&*this->proj_,&temp);

	Root::instance()->delete_interface(mds_manager + "/temp");
}
