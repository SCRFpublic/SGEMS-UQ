//

#ifndef _METRICDATA_UTILITIES_H
#define _METRICDATA_UTILITIES_H

#include <QModelIndex>
#include <GsTLAppli/extra/qtplugins/project_proxy_model.h>
#include "common.h"
#include <QString>

namespace ModelUtilities {
  QModelIndex METRICSGUI_DECL mds_metric_root_index(Filter_root_proxy_model* model, QString mds_space_name );

  QModelIndex METRICSGUI_DECL mds_metric_response_root_index(Filter_root_proxy_model* model, QString mds_space_name );

  QModelIndex METRICSGUI_DECL mds_metric_properties_root_index(Filter_root_proxy_model* model, QString mds_space_name );

  QModelIndex METRICSGUI_DECL response_root_index(Filter_root_proxy_model* model);

  QModelIndex METRICSGUI_DECL response_property_root_index(Filter_root_proxy_model* model, QString response_name);

};

#endif
