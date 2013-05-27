#ifndef METRICFILTER_H_
#define METRICFILTER_H_

#include <metrics_action_common.h>
#include <utils/named_interface.h>
#include <grid/geostat_grid.h>
#include <grid/grid_property.h> 

#include <MetricData.h>

#include <vector>
#include <map>
#include <QtXml/QtXml>

using namespace std;

class METRICSACTIONS_DECL MetricFilter : 	public Named_interface
{
public:
	MetricFilter(void);
	~MetricFilter(void);

	// i/o
	static Named_interface* CreateMetricFilter(std::string&);
	static	MetricFilter* CreateMetricFilterFromQDom(const QDomElement& root);
	static Named_interface* CreateMetricFilterFromFileName(std::string&);

	// init
	bool initialize(std::string filename);
	virtual bool initialize(const QDomElement& root);

	// create vector
	virtual MetricData* computeMetricData(Geostat_grid* input_grid, Grid_continuous_property* input_prop);
	//virtual void create_vector(Geostat_grid* input_grid, Grid_continuous_property* input_prop, vector<float>& output_vector);

	virtual std::string classname() const { return "MetricFilter"; }


protected :
	QDomElement paramXml_;
};

#endif
