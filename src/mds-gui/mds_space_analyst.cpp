/*
 * MdsSpaceAnalyst.cpp
 *
 *  Created on: Apr 29, 2011
 *      Author: aboucher
 *
 *  Edited on October 25th , 2011
 *      Author: lewisli
 */

#include "mds_space_analyst.h"
#include "MdsObjectMdiSubWindow.h"

#include <metric_manager_repository.h>
#include <MdsSpaceObjectModel.h>
#include <GsTL/utils/smartptr.h>
#include <GsTLAppli/utils/manager.h>
#include <GsTLAppli/appli/manager_repository.h>
#include <mdsspacecreatordialog.h>

#include <QVBoxLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QPushButton>

MdsSpaceAnalyst::MdsSpaceAnalyst(GsTL_project* proj, QWidget *parent)
    :QFrame(parent),proj_(proj)
{
    // Create instance of the base widget which is a VBoxLayout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    this->setLayout(mainLayout);

    QSplitter* mainSplitter = new QSplitter(Qt::Horizontal, this);
    this->layout()->addWidget(mainSplitter);

    // This is the View object that we use to display the string objects
    mdsObjectView_ = new QListView(this);
    mdsObjectView_->setDragEnabled(true);
    mdsObjectView_->setDragDropMode(QAbstractItemView::DragOnly);

    // This is the Model that we will use to store the names of the MDS objects
    model_ = new QStringListModel(this);
    model_->setHeaderData( 0, Qt::Horizontal, QString(tr("MDS Objects")));

    // Read model from root manager
    this->updateModel();
    mdsObjectView_->setModel(model_);

    QPushButton *create_mds_space_button = new QPushButton("&Create New MDS Space", this);

    QGroupBox * mdsBox = new QGroupBox(this);
    QVBoxLayout* mdsLayout = new QVBoxLayout(this);
    mdsLayout->addWidget(mdsObjectView_);
    mdsLayout->addWidget(create_mds_space_button);
    mdsBox->setLayout(mdsLayout);

    mainSplitter->addWidget(mdsBox);

    mdiArea_ = new MdsMdiArea(this);
    mainSplitter->addWidget(mdiArea_);

    connect(create_mds_space_button, SIGNAL(clicked()),
            this, SLOT( create_mds_space_button_clicked() ) );

}

MdsSpaceAnalyst::~MdsSpaceAnalyst()
{
    // TODO Auto-generated destructor stub
}


void MdsSpaceAnalyst::updateModel() {

    SmartPtr<Named_interface> ni =
            Root::instance()->interface( mds_manager);

    Manager* manager = dynamic_cast<Manager*>(ni.raw_ptr());

    Manager::interface_iterator it = manager->begin_interfaces();

    QStringList mdsObjectList;

    for( ; it != manager->end_interfaces(); ++it )
    {
        Manager* managerType = dynamic_cast<Manager*>((*it).raw_ptr());
        QString mdsObjectName( manager->name( (*it).raw_ptr() ).c_str() );
        mdsObjectList.append(mdsObjectName);
    }
    mdsObjectList.sort();

    model_->setStringList(mdsObjectList);

}

void MdsSpaceAnalyst::create_mds_space_button_clicked() {


    // MdsSpaceCreator* mdsSpaceCreator = new MdsSpaceCreator(proj_, this);
    //    if( mdsSpaceCreator->exec() == QDialog::Rejected ) return;
    //   mdiArea_->addSubWindow(mdsSpaceCreator);
    //	mdsSpaceCreator->show();
}

void MdsSpaceAnalyst::updateList()
{
    this->updateModel();
}
