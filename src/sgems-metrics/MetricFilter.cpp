#include "MetricFilter.h"
#include <MetricData.h>
#include "MetricFilterPackage.h"
#include "ml_library_constants.h"

#include <GsTLAppli/appli/manager_repository.h>

MetricFilter::MetricFilter(void)
{
}

MetricFilter::~MetricFilter(void)
{
}


Named_interface* MetricFilter::CreateMetricFilter(std::string& s)
{
	// create appropriate class
	if (s.compare(MetricFilterMean::filtername()) == 0)
		return new MetricFilterMean();
	else if (s.compare(MetricFilterVariance::filtername()) == 0)
		return new MetricFilterVariance();
	else if (s.compare(MetricFilterValues::filtername()) == 0)
		return new MetricFilterValues();
	else 
		return 0;
}

Named_interface* MetricFilter::CreateMetricFilterFromFileName(std::string& filename){
	QDomDocument doc("MetricFilter");
	qDebug() << " filename : " << QString(filename.c_str());
	QFile file(filename.c_str());
	if (!doc.setContent(&file))
	{
		file.close();
		return false;
	}
	file.close();

	QDomElement root = doc.documentElement();
	return CreateMetricFilterFromQDom(root);

}
MetricFilter* MetricFilter::CreateMetricFilterFromQDom(const QDomElement& root){
	if( !root.hasAttribute("type")  ) return 0;
	if( !root.hasAttribute("name")  ) return 0;
	std::string type = root.attribute("type").toStdString();
	std::string name = root.attribute("name").toStdString();

	// Need to retrieve from the manager the filter of type "type"
  SmartPtr<Named_interface> ni =  
	 Root::instance()->interface( metricFilter_manager + "/" + type);  

	if (!ni.raw_ptr())
		return 0;
	
	MetricFilter* filter = dynamic_cast<MetricFilter*>(ni.raw_ptr());
	if (!filter)
		return 0;

	// found metric filter
	filter->initialize(root);
	return filter;

}

bool MetricFilter::initialize(std::string filename)
{
	QDomDocument doc("MetricFilter");
	qDebug() << " filename : " << QString(filename.c_str());
	QFile file(filename.c_str());
	if (!doc.setContent(&file))
	{
		file.close();
		return false;
	}
	file.close();

	QDomElement root = doc.documentElement();
	return initialize(root);
}

bool MetricFilter::initialize(const QDomElement& root) {
	paramXml_ = root;
	return true;
}

MetricData* MetricFilter::computeMetricData(Geostat_grid* input_grid, GsTLGridProperty* input_prop) {
	return 0;
}
