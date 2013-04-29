/*
 * MetricDataListWidget.cpp
 *
 *  Created on: Apr 22, 2011
 *      Author: aboucher
 */

#include "MetricDataTable.h"

#include <metric_manager_repository.h>
#include <MetricDataManager.h>
#include <GsTL/utils/smartptr.h>
#include <GsTLAppli/utils/manager.h>
#include <GsTLAppli/appli/manager_repository.h>

#include <QVBoxLayout>


MetricDataTable::MetricDataTable(GsTL_project* proj, QWidget* parent) :
    QWidget(parent),proj_(proj)
{

    tableView_ = new QTableView(this);
    this->setMetricModel();

    QVBoxLayout* l = new QVBoxLayout(this);
    l->addWidget(tableView_);
    l->addStretch(1);

    //Forward signal
    connect( tableView_, SIGNAL(clicked(QModelIndex)),
             this, SIGNAL(clicked(QModelIndex)) );

}

MetricDataTable::~MetricDataTable()
{

}


void MetricDataTable::setMetricModel(){

    delete model_;

    SmartPtr<Named_interface> ni =
            Root::instance()->interface( metricData_manager + "/metricRegistrar");

    MetricDataManager* mDataRegistrar = (MetricDataManager*)(ni.raw_ptr());

    std::vector<std::string> metric_names = mDataRegistrar->get_metric_names();

    model_ = new QStandardItemModel(metric_names.size(), 2, this);
    model_->setHeaderData(0, Qt::Horizontal, tr("Name"));
    model_->setHeaderData(1, Qt::Horizontal, tr("Type"));
    for(unsigned int i=0; i<metric_names.size(); ++i ) {
        const MetricDataToProperties m =
                mDataRegistrar->get_properties_for_metric(metric_names[i]);

        //MetricDataToProperties m;
        model_->setData(model_->index(i,0, QModelIndex()),
                        QString::fromStdString(metric_names[i]) );
        model_->setData(model_->index(i,0, QModelIndex()),
                        QString::fromStdString(m.type()) );
    }

    tableView_->setModel(model_);

}
