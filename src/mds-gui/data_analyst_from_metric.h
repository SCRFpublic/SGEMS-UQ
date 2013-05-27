/*
 * DataAnalystFromMetrics.h
 *
 *  Created on: Apr 26, 2011
 *      Author: aboucher
 */

#ifndef DATAANALYSTFROMMETRICS_H_
#define DATAANALYSTFROMMETRICS_H_

#include "chartmdiarea.h"
#include <common.h>
#include <appli/project.h>

#include <Qt>
#include <QFrame>
#include <QMdiArea>
#include <QTreeWidget>
#include <QTreeView>
#include <QListView>
#include <QComboBox>
#include <QStackedWidget>
#include <QMap>
#include <QString>

class METRICSGUI_DECL DataAnalystFromMetric : public QFrame
{

    Q_OBJECT

public:
    explicit DataAnalystFromMetric(GsTL_project* proj, QWidget *parent = 0);
    ~DataAnalystFromMetric();

    void setTopLevelMetricModel(QAbstractItemModel* model);

protected :

    GsTL_project* proj_;

    ChartDataFromMetricMdiArea *mdiArea_;
    QTreeView* metricView_;
};




#endif /* DATAANALYSTFROMMETRICS_H_ */
