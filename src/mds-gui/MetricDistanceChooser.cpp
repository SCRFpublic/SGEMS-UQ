/*
 * MetricDistanceChooser.cpp
 *
 *  Created on: May 1, 2011
 *      Author: aboucher
 */

#include "MetricDistanceChooser.h"
#include <QHBoxLayout>

#include <MetricData.h>
#include <MetricDataManager.h>
#include <metric_manager_repository.h>

#include <GsTL/utils/smartptr.h>
#include <GsTLAppli/utils/manager.h>
#include <GsTLAppli/appli/manager_repository.h>

MetricDistanceChooser::MetricDistanceChooser(QString metricName, QWidget* parent) {
	// TODO Auto-generated constructor stub

	QHBoxLayout *layout = new QHBoxLayout(this);
	metricNameLabel_ = new QLabel(metricName, this);
	kernelChooser_ = new QComboBox(this);
	layout->addWidget(metricNameLabel_);
	layout->addWidget(kernelChooser_);

	this->setLayout(layout);


	this->populateKernelTypes();

}

MetricDistanceChooser::~MetricDistanceChooser() {
	// TODO Auto-generated destructor stub
}


void MetricDistanceChooser::populateKernelTypes(){

// Need to retrieve from the manager the filter of type "type"
	SmartPtr<Named_interface> ni =
	 Root::instance()->interface( kernel_manager);

	Manager* manager = dynamic_cast<Manager*>(ni.raw_ptr());

	Manager::type_iterator it = manager->begin();


	for( ; it != manager->end(); ++it ) {
		QString type((*it).c_str());
		kernelChooser_->addItem(type);
	}
 
}


QString MetricDistanceChooser::getSelectedKernel(){
 // return "";
	return kernelChooser_->currentText();
}
