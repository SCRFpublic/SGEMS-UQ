#ifndef MDSMETRICMANAGERFRAME_H
#define MDSMETRICMANAGERFRAME_H

// GsTL Dependencies
#include <GsTL/utils/smartptr.h>
#include <common.h>
// GsTLAppli Dependencies
#include <GsTLAppli/utils/manager.h>
#include <GsTLAppli/appli/manager_repository.h>
#include <GsTLAppli/appli/root_model.h>
#include <GsTLAppli/appli/project.h>
#include <GsTLAppli/extra/qtplugins/project_proxy_model.h>
#include <GsTLAppli/extra/qtplugins/property_proxy_model.h>

#include <QFrame>
#include <QStandardItemModel>

#include "metricplotsmdiarea.h"

#include <actions/externalresponseioaction.h>

namespace Ui {
class MDSMetricManagerFrame;
}

class METRICSGUI_DECL MDSMetricManagerFrame : public QFrame
{
    Q_OBJECT

public:
    explicit MDSMetricManagerFrame(GsTL_project *proj,QWidget *parent = 0);
    ~MDSMetricManagerFrame();

public slots:
    void setResponseTreeView();
    void updatePropertyExplorer(QModelIndex responseIndex);
    void updateGridSelection(QModelIndex gridIndex);
    void loadResponse();

protected:
    Filter_root_proxy_model* proxy_model;
    Filter_root_proxy_model* grid_proxy_model;
    QStandardItemModel  *emptyModel;


    void setGridTreeView();

    QModelIndex responseBaseIndex;

    MetricPlotsMdiArea *metricPlotsMdi;

    GsTL_project *proj_;

private:
    Ui::MDSMetricManagerFrame *ui;
};

#endif // MDSMETRICMANAGERFRAME_H
