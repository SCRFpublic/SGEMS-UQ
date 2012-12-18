#include "MetricFilterCollection.h"

MetricFilterCollection::MetricFilterCollection(void)
{
	filters_.clear();
}

MetricFilterCollection::~MetricFilterCollection(void)
{
	vector<MetricFilter*>::iterator it;
	for (it = filters_.begin(); it != filters_.end(); it++) {
		delete (*it);
	} 
}

void MetricFilterCollection::addFilter(MetricFilter* f)
{
	filters_.push_back(f);
}