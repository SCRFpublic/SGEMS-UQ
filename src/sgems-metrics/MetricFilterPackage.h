#ifndef METRICFILTERPACKAGE_H_
#define METRICFILTERPACKAGE_H_

#include <metrics_action_common.h>
#include "MetricFilter.h"


class MetricData;

class METRICSACTIONS_DECL MetricFilterMean : 	public MetricFilter
{
public:

	MetricFilterMean(void) {}
	virtual ~MetricFilterMean(void) {}
	virtual MetricData* computeMetricData(Geostat_grid* input_grid, Grid_continuous_property* input_prop);


	virtual std::string classname() const { return "MetricFilterMean"; }
	static std::string filtername() { return "FilterMean"; }
};

class MetricFilterVariance :
	public MetricFilter
{
public:
	MetricFilterVariance(void) {}
	virtual ~MetricFilterVariance(void) {}
	virtual MetricData* computeMetricData(Geostat_grid* input_grid, Grid_continuous_property* input_prop);


	virtual std::string classname() const { return "MetricFilterVariance"; }
	static std::string filtername() { return "FilterVariance"; }
};

class MetricFilterValues :
	public MetricFilter
{
public:
	MetricFilterValues(void) {}
	virtual ~MetricFilterValues(void) {}
	virtual MetricData* computeMetricData(Geostat_grid* input_grid, Grid_continuous_property* input_prop);

	virtual std::string classname() const { return "MetricFilterValues"; }
	static std::string filtername() { return "PixelWiseValues"; }
};

#endif