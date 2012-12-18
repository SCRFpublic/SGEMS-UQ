/*
 * DataAnalystFromMetrics.cpp
 *
 *  Created on: Apr 26, 2011
 *      Author: aboucher
 */

#include "data_analyst_from_metric.h"
#include "vtkmdisubwindow.h"
#include "data_from_metric_chart_mdi_subwindow.h"

#include <QVBoxLayout>
#include <QSplitter>

DataAnalystFromMetric::DataAnalystFromMetric(GsTL_project* proj, QWidget *parent)
 :QFrame(parent),proj_(proj)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    this->setLayout(mainLayout);
    QSplitter* mainSplitter = new QSplitter(Qt::Horizontal, this);
    this->layout()->addWidget(mainSplitter);

    metricView_ = new QTreeView(this);
    metricView_->setDragEnabled(true);
    metricView_->setDragDropMode(QAbstractItemView::DragOnly);
	metricView_->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mainSplitter->addWidget(metricView_);

    mdiArea_ = new ChartDataFromMetricMdiArea(this);
    mainSplitter->addWidget(mdiArea_);
}


DataAnalystFromMetric::~DataAnalystFromMetric(){}

void DataAnalystFromMetric::setTopLevelMetricModel(QAbstractItemModel* model){
	metricView_->setModel(model);

}
