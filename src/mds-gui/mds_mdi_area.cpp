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
#include <GsTLAppli/utils/manager.h>
#include <GsTLAppli/appli/manager_repository.h>

#include <QDragEnterEvent>
#include <QDropEvent>

#include <parameterExplorer/parameterplotsubwindow.h>
#include <parameterExplorer/sharedparametersubwindow.h>

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
        std::cerr << "Generating viewer for " << mdsObjectName.toStdString() << std::endl;
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

        //mdsSubViewer->show();
        mdsSubViewer->showMaximized();
        // mdsSubViewer->setGeometry(0,0,1200,800);

        //        connect(mdsSubView->parameterExploreModule,
        //                SIGNAL(updateParameterPlots(
        //                           std::vector<parameterPlotSubWindow*>)),
        //                this,
        //                SLOT(addParameterSubViewer(
        //                         std::vector<parameterPlotSubWindow*>)));

        //        connect(mdsSubView->parameterExploreModule,
        //                SIGNAL(updateSharedPlot(sharedParameterSubwindow*)),
        //                this,
        //                SLOT(addSharedParamSubViewer(sharedParameterSubwindow*)));


    }
}


void MdsMdiArea::addParameterSubViewer(std::vector<parameterPlotSubWindow*>
                                       subWindows)
{

    for (unsigned int i = 0; i < subWindows.size(); ++i)
    {
        QMdiSubWindow *viewer = new QMdiSubWindow(this);
        viewer->setWidget(subWindows.at(i));
        this->addSubWindow(viewer);
        viewer->show();
        //        viewer->setWindowTitle(subWindows.at(i)->getParameterName());
        //        viewer->setGeometry(0,0,480,320);
        //        viewer->setMaximumSize(480,320);
        //        viewer->setMinimumSize(480,320);

    }


}

void MdsMdiArea::addSharedParamSubViewer(sharedParameterSubwindow *subWindow)
{
    QMdiSubWindow *viewer = new QMdiSubWindow(this);
    viewer->setWidget(subWindow);
    this->addSubWindow(viewer);
    viewer->show();
    //     viewer->setGeometry(0,0,400,400);
    //     viewer->setMaximumSize(400,400);
    //     viewer->setMinimumSize(400,400);

}

// Add Parameter Chart to MDI Window

void MdsMdiArea::showParametersChart(QString mdsObjectName)
{
    //    // Get sub window that is displaying parameter chart

    //    MDSUncertainitySubWindow *mdsSubView =
    //            hashObjectWindows_[mdsObjectName].first;

    //    //    QMdiSubWindow *clusterParameterViewer =
    //    //            hashObjectWindows_[mdsObjectName].second;

    //    //QMdiSubWindow *viewer = new QMdiSubWindow(this);

    //    parameterPlotSubWindow *test1 = new parameterPlotSubWindow(this);
    //    parameterPlotSubWindow *test2 = new parameterPlotSubWindow(this);
    //    parameterPlotSubWindow *test3 = new parameterPlotSubWindow(this);
    //    parameterPlotSubWindow *test4 = new parameterPlotSubWindow(this);


    //    std::vector<parameterPlotSubWindow*> testVector;

    //    testVector.push_back(test1);
    //    testVector.push_back(test2);
    //    testVector.push_back(test3);
    //    testVector.push_back(test4);

    //    this->addParameterSubViewer(testVector);

    //  clusterParameterViewer->setWidget(test);

    // Todo: Make sure cluster parameter wasn't closed
    //    clusterParameterViewer->setWidget(mdsSubView->getClusterAddress());
    //    clusterParameterViewer->setGeometry(0,0,640,640);
    //    clusterParameterViewer->show();
}
