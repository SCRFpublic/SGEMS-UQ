/*
 * MultipleMdsSpaceObjectSelector.cpp
 *
 *  Created on: Apr 30, 2011
 *      Author: aboucher
 */

#include "MultipleMetricDataSelector.h"

#include <MetricData.h>
#include <MetricDataManager.h>
#include <metric_manager_repository.h>

#include <GsTL/utils/smartptr.h>
#include <GsTLAppli/utils/manager.h>
#include <GsTLAppli/appli/manager_repository.h>

#include <QAbstractItemView>
#include <QListWidgetItem>
#include <QVBoxLayout>

#include <algorithm>

MultipleMetricDataSelector::MultipleMetricDataSelector(QWidget* parent)
    : QGroupBox(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    list_ = new QListWidget(this);
    list_->setSelectionMode(QAbstractItemView::ExtendedSelection);
    selectButton_ = new QPushButton("Select", this);
    layout->addWidget(list_);
    layout->addWidget(selectButton_);
    this->setLayout(layout);

    connect(selectButton_,SIGNAL(clicked()), this ,
            SLOT(getSelectedMetricsList()));


}

MultipleMetricDataSelector::~MultipleMetricDataSelector() {
    // TODO Auto-generated destructor stub
}


void MultipleMetricDataSelector::setMetricsList(const QStringList& propSelection){
    QStringList propNames_ = propSelection;
    list_->clear();
    metricNames_.clear();

    if(gridName_.isEmpty()) return;
    if(propNames_.isEmpty()) return;

    SmartPtr<Named_interface> ni =
            Root::instance()->interface( metricData_manager + "/metricRegistrar");


    MetricDataManager* mRegistrar =
            dynamic_cast<MetricDataManager*>(ni.raw_ptr());

    // Get all the possible metrics
    std::vector<std::string> allMetrics = mRegistrar->get_metric_names();
    std::set<std::string> commonMetrics(allMetrics.begin(), allMetrics.end());

    std::string gridName_str = gridName_.toStdString();

    for(int i = 0; i < propNames_.size(); ++i)
    {
        // Check Why mdata is decreasing in size
        MetricDataCollection mdata = mRegistrar->getMetricDataCollection(
                    gridName_str, propNames_.at(i).toStdString());

        std::set<std::string> propMetrics;
        std::vector<MetricData*>::iterator it = mdata.begin();
        for( ; it!=mdata.end() ; ++it)
        {
            propMetrics.insert((*it)->name());
        }

        std::set<std::string> commonMetricsCopy(commonMetrics.begin(),
                                                commonMetrics.end());
        commonMetrics.clear();

        std::set_intersection(propMetrics.begin(), propMetrics.end(),
                              commonMetricsCopy.begin(), commonMetricsCopy.end(),
                              std::inserter(commonMetrics,commonMetrics.begin()));

        if(  commonMetrics.empty() ) return;

    }
    std::set<std::string>::iterator it =  commonMetrics.begin();
    for( ; it != commonMetrics.end(); ++it )
    {
        metricNames_.append( QString::fromStdString(*it) );
    }

    list_->addItems(metricNames_);

}

void MultipleMetricDataSelector::getSelectedMetricsList(){
    QStringList selection;
    for( unsigned int i = 0; i < list_->count() ; i++ ) {
        QListWidgetItem * it = list_->item(i);
        if( it->isSelected( ) && !it->text().isEmpty() ) {
            selection.append(it->text());
        }
    }
    emit selectionApproved(selection);
}

void MultipleMetricDataSelector::setSelectedGrid(const QString& gridName){
    if( gridName_ == gridName ) return;
    gridName_ = gridName;
    list_->clear();

}

