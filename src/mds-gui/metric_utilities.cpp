// A. Boucher

#include <metric_utilities.h>
#include <metric_manager_repository.h>


namespace ModelUtilities {
QModelIndex mds_metric_root_index(Filter_root_proxy_model* model,
                                  QString mds_space_name )
{
    QModelIndex mds_root = model->manager_index(
                QString::fromStdString(mds_manager));

    int nrow = model->rowCount(mds_root);
    for (unsigned int i = 0; i < nrow; ++i)
    {
        QModelIndex index = model->index(i,0,mds_root);
        if (index.data() == mds_space_name)
            return index;

    }

    return QModelIndex();
}

QModelIndex mds_metric_properties_root_index(Filter_root_proxy_model* model,
                                             QString mds_space_name )
{

    QModelIndex mdsSpaceIndex = mds_metric_root_index(model, mds_space_name);

    int nrow = model->rowCount(mdsSpaceIndex);
    for (unsigned int i = 0; i < nrow; ++i)
    {
        QModelIndex index = model->index(i,0,mdsSpaceIndex);
        if (index.data() == "MetricProperties")
            return index;

    }

    return QModelIndex();

}

QModelIndex mds_metric_response_root_index(Filter_root_proxy_model* model,
                                           QString mds_space_name )
{

    QModelIndex mdsSpaceIndex = mds_metric_root_index(model, mds_space_name);

    int nrow = model->rowCount(mdsSpaceIndex);
    for (unsigned int i = 0; i < nrow; ++i)
    {
        QModelIndex index = model->index(i,0,mdsSpaceIndex);
        if (index.data() == "MetricResponses")
            return index;

    }

    return QModelIndex();

}

QModelIndex response_root_index(Filter_root_proxy_model* model)
{
    QModelIndex metric_root = model->manager_index(
                QString::fromStdString(metricData_manager));

    int nrow = model->rowCount(metric_root);
    for (unsigned int i = 0; i < nrow; ++i)
    {
        QModelIndex index = model->index(i,0,metric_root);
        if (index.data() == "Metrics")
            return index;

    }

    return QModelIndex();
}

QModelIndex response_property_root_index(Filter_root_proxy_model* model,
                                         QString response_name)
{
    QModelIndex metricRootIndex = response_root_index(model);

    int nrow = model->rowCount(metricRootIndex);
    for (unsigned int i = 0; i < nrow; ++i)
    {
        QModelIndex index = model->index(i,0,metricRootIndex);
        if (index.data() == response_name)
            return index;

    }

    return QModelIndex();
}


}
