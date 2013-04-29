/*
 * MdsSpaceCreator.cpp
 *
 *  Created on: Apr 30, 2011
 *      Author: aboucher
 */

#include "MdsSpaceCreator.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

MdsSpaceCreator::MdsSpaceCreator(GsTL_project* proj, QWidget* parent)
	: QDialog(parent), proj_(proj)
{
	QHBoxLayout* mainLayout = new QHBoxLayout(this);

	QGroupBox *propBox = new QGroupBox(this);
	QVBoxLayout* propLayout = new QVBoxLayout(this);

        gridSelector_ = new GridSelectorBasic(this, "Grid", proj_);
	mPropSelector_ = new OrderedPropertySelector( this, "Properties" );

        this->metricSpaceName_ = new QTextEdit(this);
        QVBoxLayout *spaceNameLayout = new QVBoxLayout(this);
        spaceNameLayout->addWidget(new QLabel("Enter Metric Space Name:"));
        spaceNameLayout->addWidget(this->metricSpaceName_);
        QGroupBox *spaceNameBox = new QGroupBox(this);
        spaceNameBox->setLayout(spaceNameLayout);
        mainLayout->addWidget(spaceNameBox);

	propLayout->addWidget(new QLabel("Select Properties", this));
	propLayout->addWidget(gridSelector_);
	propLayout->addWidget(mPropSelector_);
	propLayout->insertStretch(-1);
	propBox->setLayout(propLayout);

	mainLayout->addWidget(propBox);
	mainLayout->insertSpacing(-1,1);



	QGroupBox *metricBox = new QGroupBox(this);
	QVBoxLayout* metricLayout = new QVBoxLayout(this);
	metricSelector_ = new MultipleMetricDataSelector(this);
	metricLayout->addWidget(new QLabel("Select Metrics", this));
	metricLayout->addWidget(metricSelector_);
	metricLayout->insertStretch(-1);
	metricBox->setLayout(metricLayout);
	mainLayout->addWidget(metricBox);



	kernelBox_ = new QGroupBox(this);
	QVBoxLayout* distanceLayout = new QVBoxLayout(this);
	distanceLayout->addWidget(new QLabel("Select Distance", this));
	kernelBox_->setLayout(distanceLayout);
	mainLayout->addWidget(kernelBox_);


	connect(gridSelector_, SIGNAL(activated(const QString&)),
			mPropSelector_, SLOT(show_properties(const QString&)) );

	connect( gridSelector_, SIGNAL(activated(const QString&)),
			metricSelector_, SLOT(setSelectedGrid(const QString&)) );

	connect( mPropSelector_, SIGNAL(forwardSelectedProperties(const QStringList&)),
			metricSelector_, SLOT(setMetricsList(const QStringList&)) );

	connect(metricSelector_, SIGNAL(selectionApproved(const QStringList& )),
			this, SLOT(showMetricKernelChooser(const QStringList&)));

}

MdsSpaceCreator::~MdsSpaceCreator() {
	// TODO Auto-generated destructor stub
}

/*
void MdsSpaceCreator::getSelectedPropertyList(){
	QStringList selectedProperties;
	QList<QListWidgetItem*>  items = mPropSelector_->selectedItems();
	for(int i=0; i<items.count(); ++i) {
		selectedProperties.append(items.at(i)->text());
	}
	emit(selectedPropertyList(selectedProperties));
}
*/
void MdsSpaceCreator::showMetricKernelChooser(const QStringList& metricNames){

	// Metrics to be removed from the list
	QList<QString> keys = kernelHash_.keys();
	for(int i=0; i<keys.size(); ++i) {
		if( metricNames.contains(keys.at(i))  ) continue;
		MetricDistanceChooser* kernelToBeRemoved = kernelHash_.value(keys.at(i));
		kernelBox_->layout()->removeWidget(kernelToBeRemoved);
		kernelHash_.remove(keys.at(i));
		delete kernelToBeRemoved;
	}

	for(int i=0; i<metricNames.size(); ++i) {
		if( kernelHash_.contains(metricNames.at(i)) ) continue;
		MetricDistanceChooser* kernelChooser = new MetricDistanceChooser(metricNames.at(i), this);
		kernelBox_->layout()->addWidget(kernelChooser);
		kernelHash_.insert(metricNames.at(i), kernelChooser);
	}
}
