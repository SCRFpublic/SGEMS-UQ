#include "MetricFilterPackage.h"
#include <GsTLAppli/grid/grid_model/gval_iterator.h>

// preset filter packages

// return mean
/*
void filter_mean::create_vector(Geostat_grid* input_grid, GsTLGridProperty* input_prop, vector<float>& output_vector) {
 Geostat_grid::iterator it = input_grid->begin();

 float sum = 0.0f;
 int n = 0;

 int node_id;
 for (; it != input_grid->end(); ++it) {
  node_id = it->node_id();
  if (input_prop->is_informed(node_id)) {
   sum += input_prop->get_value(node_id);
   n++;
  }
 }
 float mean = sum / (float)n;
 output_vector.push_back(mean);
}
*/

MetricData* MetricFilterMean::computeMetricData(Geostat_grid* input_grid,
                                                GsTLGridProperty* input_prop)
{
    float sum = 0.0f;
    int n = 0;

    GsTLGridProperty::iterator it = input_prop->begin();

    for ( ; it != input_prop->end(); ++it) {
        sum += *it;
        n++;
    }

    return new MetricScalarData(input_prop,paramXml_, sum / (float)n);
}


MetricData* MetricFilterVariance::computeMetricData(Geostat_grid* input_grid,
                                                    GsTLGridProperty* input_prop)
{

    float sum = 0.0f;
    float sum2 = 0.0f; // sum of each element squared
    int n = 0;

    GsTLGridProperty::iterator it = input_prop->begin();

    for ( ; it != input_prop->end(); ++it)
    {
        float elt = *it;
        sum += elt;
        sum2 += elt * elt;
        n++;
    }

    float ex = sum / (float)n;
    float ex2 = sum2 / (float) n;
    float variance = ex2 - ex * ex; // Var[X] = E[X^2] - E[X]^2]

    return new MetricScalarData(input_prop,paramXml_, variance);
}

// return vector of values
MetricData* MetricFilterValues::computeMetricData(Geostat_grid* input_grid,
                                                  GsTLGridProperty* input_prop)
{

    return new MetricPropertyData(input_prop,paramXml_, input_prop);

}

