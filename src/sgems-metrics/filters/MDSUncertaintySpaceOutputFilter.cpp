/*
 * MDSUncertainitySpaceOutputFilter.cpp
 *
 *  Created on: Nov 23, 2011
 *      Author: lewisli
 */

#include <filters/MDSUncertaintySpaceOutputFilter.h>


MDSUncertaintySpaceOutputFilter::MDSUncertaintySpaceOutputFilter()
{
	// TODO Auto-generated constructor stub

}

MDSUncertaintySpaceOutputFilter::~MDSUncertaintySpaceOutputFilter()
{
	// TODO Auto-generated destructor stub
}

void MDSUncertaintySpaceOutputFilter::exec()
{
	QString *fname = new QString(filename.c_str());
	QFile file(*fname);

	if (file.exists())
		file.remove();

	if( !file.open(QFile::WriteOnly) )
		std::cerr << "ERROR IN IO" << std::endl;

	QTextStream ts( &file );
	ts << this->paramXml_.toString();
	file.close();
}
