#ifndef METRICPLOTSMDIAREA_H
#define METRICPLOTSMDIAREA_H

#include <QMdiArea>
#include <common.h>
#include <QDragEnterEvent>
#include <QDropEvent>

#include <GsTLAppli/extra/qtplugins/project_proxy_model.h>
#include <GsTLAppli/extra/qtplugins/property_proxy_model.h>


#include <metric_manager_repository.h>
#include <MetricData.h>
#include <MetricDataManager.h>
#include <GsTL/utils/smartptr.h>
#include <GsTLAppli/utils/manager.h>
#include <GsTLAppli/appli/manager_repository.h>
#include <metricmanager/mdsmetricplotsubwindow.h>


class METRICSGUI_DECL MetricPlotsMdiArea : public QMdiArea
{
    Q_OBJECT
public:
    explicit MetricPlotsMdiArea(QWidget *parent = 0,
                                QString gridName = QString(""));
    virtual ~MetricPlotsMdiArea();

signals:

public slots:
    void dragEnterEvent(QDragEnterEvent *event);
    void changedSelectedMetric(QString name);
    void changeSelectedGrid(QString name);
    void dropEvent(QDropEvent *event);
    void addCharts(QStringList propertiesToBeAdded, QString gridName);

protected:
    int metricDataType;
    QString selectedMetricName;
    QString selectedGridName;
    Filter_root_proxy_model* proxy_model;
};

#endif // METRICPLOTSMDIAREA_H
