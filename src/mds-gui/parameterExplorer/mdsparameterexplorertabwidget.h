#ifndef MDSPARAMETEREXPLORERTABWIDGET_H
#define MDSPARAMETEREXPLORERTABWIDGET_H

#include <QTabWidget>
#include <common.h>

#include <metric_manager_repository.h>
#include <MetricData.h>
#include <MetricDataManager.h>
#include <GsTL/utils/smartptr.h>
#include <utils/manager.h>
#include <appli/manager_repository.h>
#include <charts/chart_mdi_area.h>
#include <QDebug>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QFile>
#include <QDir>

#include <QHash>
#include <QDomDocument>
#include <QXmlDefaultHandler>
#include <iostream>

#include <multi_dim_scaling_space.h>
#include <QStringList>
#include <QStringListModel>
#include <QString>

#include <QItemSelectionModel>

#include <iostream>

#include "param_display_widget.h"
#include "param_search_widget.h"
#include "param_sensitivity_widget.h"
#include "param_plot_widget.h"
#include "param_cluster_widget.h"
#include "param_commonmodels_widget.h"
#include "param_commonparam_widget.h"

class parameterNode;
class parameterModel;

namespace Ui {
class MDSParameterExplorerTabWidget;
}

class MDSParameterExplorerTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit MDSParameterExplorerTabWidget(Chart_mdi_area* mdi_area,
                                           QString mdsObjectName = QString(""),QWidget *parent=0);
    ~MDSParameterExplorerTabWidget();

    void loadClusterComboBox();

protected:
    MultiDimScalingSpace* mdsObject_;

    QString mdsSpaceName_;


    param_display_widget *displayWidget;
    param_search_widget *searchWidget;
    param_sensitivity_widget *sensitivityWidget;
    param_plot_widget *plotWidget;
    param_cluster_widget *clusterWidget;
    param_commonModels_widget *commonWidget;
    param_commonParam_widget *commonParamWidget;


private slots:


signals:

        void highlightCommonModels(QStringList &selectModels);

private:
    Chart_mdi_area* mdi_area_;

    Ui::MDSParameterExplorerTabWidget *ui;

};

#endif // MDSPARAMETEREXPLORERTABWIDGET_H
