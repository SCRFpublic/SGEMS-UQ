/*
 * mds_mdi_area.cpp
 *
 *  Created on: Apr 29, 2011
 *      Author: aboucher
 */

#include "mds_mdi_area.h"
//#include "MdsObjectMdiSubWindow.h"

#include <metric_manager_repository.h>
#include <multi_dim_scaling_space.h>
#include "mdsuncertainitysubwindow.h"

#include <GsTL/utils/smartptr.h>
#include <utils/manager.h>
#include <appli/manager_repository.h>

#include <QDragEnterEvent>
#include <QDropEvent>



#include <utility>


MdsMdiArea::MdsMdiArea(QWidget* parent)
    : QMdiArea(parent)
{
    this->setAcceptDrops(true);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

MdsMdiArea::~MdsMdiArea() {
    // TODO Auto-generated destructor stub
}

// Function runs when we are dragging data into the visualization side
void MdsMdiArea::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->
            hasFormat("application/x-qabstractitemmodeldatalist")) {
        event->acceptProposedAction();
    }
}

void MdsMdiArea::dropEvent(QDropEvent *event)
{
    QByteArray encoded = event->mimeData()->
            data("application/x-qabstractitemmodeldatalist");
    QDataStream stream(&encoded, QIODevice::ReadOnly);

    QStringList objectNames;

    while (!stream.atEnd())
    {
        int row, col;
        QMap<int,  QVariant> roleDataMap;
        stream >> row >> col >> roleDataMap;

        qDebug()<<roleDataMap.size()<<"  "<<roleDataMap.value(0)<<"  "
               <<roleDataMap.value(1);
        objectNames.append(roleDataMap.value(0).toString());

        /* do something with the data */
    }
    for(int i=0; i<objectNames.size(); ++i)
    {
        this->addChart(objectNames.at(i));
    }

}

// This functions creates the actual chart and 3D view to the visualization area
void MdsMdiArea::addChart(QString mdsObjectName)
{

    // There is no point in creating the same visualizer for each MDS space
    // Instead we create a hash table that saves the names and respective point
    // -ers.

    if (!hashObjectWindows_.contains(mdsObjectName))
    {
        SmartPtr<Named_interface> ni =
                Root::instance()->interface( mds_manager+"/"+
                                             mdsObjectName.toStdString());

        MultiDimScalingSpace* mdsObject = dynamic_cast
                <MultiDimScalingSpace*>(ni.raw_ptr());

        if(mdsObject==0) return;

        // Create QWidget Type Uncertaininty SubWindow
        MDSUncertainitySubWindow *mdsSubView = new
                MDSUncertainitySubWindow(mdsObjectName, this);

        // Generate a SubWindow type object that can be added
        QMdiSubWindow *mdsSubViewer = new QMdiSubWindow(this);
        mdsSubViewer->setWidget(mdsSubView);
        this->addSubWindow(mdsSubViewer);

        mdsSubViewer->showMaximized();
    }
}

