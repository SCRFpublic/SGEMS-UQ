#include "mds_gui.h"
#include "metric_explorer.h"
#include "data_analyst_from_metric.h"
//#include "data_analyst_from_property.h"
#include "mds_space_analyst.h"
#include "mdsspaceanalystframe.h"
#include "metricmanager/mdsmetricmanagerframe.h"

#include <ml_library_constants.h>
#include <MetricDataManager.h>
#include <metric_manager_repository.h>
#include <dommodel.h>
#include <metricdommodel.h>
#include <GsTL/utils/smartptr.h>
#include <GsTLAppli/utils/manager.h>
#include <GsTLAppli/appli/manager_repository.h>
#include <GsTLAppli/appli/project.h>

#include <QToolBar>
#include <QStackedWidget>
#include <QSignalMapper>
#include <QAction>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>

mds_gui::mds_gui(GsTL_project* proj, QWidget *parent)
    : proj_(proj),QMainWindow(parent)
{
    this->setMinimumSize(800,600);
    this->setWindowState(Qt::WindowMaximized);

    this->statusBar();
    QToolBar* toolbar = new QToolBar(this);
    QSize size;
    size.setHeight(80);
    size.setWidth(80);
    toolbar->setIconSize(size);
    this->addToolBar(Qt::LeftToolBarArea,toolbar);
    toolbar->setMovable(false);
    toolbar->setFloatable(false);


    //toolbar->addSpacing(2);
    toolbar->addSeparator();
    QAction* actionMetricCreate =
            toolbar->addAction(QIcon(":/icons/metricExplorer.png"), "View");
    actionMetricCreate->setStatusTip(tr("Manage Metric Data"));

    toolbar->addSeparator();
    QAction* actionMetricData =
            toolbar->addAction(QIcon(":/icons/metric2props.png"),
                               "Metric \nPerspective");
    actionMetricData->setStatusTip(tr("Explore Metrics->Property"));

    toolbar->addSeparator();
    QAction* actionGeoData =
            toolbar->addAction(QIcon(":/icons/props2metrics.png"),
                               "Property \nPerspective");
    actionGeoData->setStatusTip(tr("Explore Property->Metrics"));

    toolbar->addSeparator();
    QAction* actionGeoVizu =
            toolbar->addAction(
                QIcon(":/icons/MdsSpace.png"),"Uncertainty \nSpace");
    actionGeoVizu->
            setStatusTip(tr("Explore Uncertainty space with Multi-dimensional scaling Two"));

    toolbar->addSeparator();

    QAction* actionMetricManager =
            toolbar->addAction(
                QIcon(":/icons/props2metrics.png"),"Response \nPerspective");
    actionMetricManager->setStatusTip(tr("Explore Available Metrics/Responses"));
    toolbar->addSeparator();




    QSignalMapper *mSignal = new QSignalMapper(this);

    connect(actionMetricCreate, SIGNAL(triggered()),
            mSignal, SLOT(map()));
    connect(actionMetricData, SIGNAL(triggered()),
            mSignal, SLOT(map()));
    connect(actionGeoData, SIGNAL(triggered()),
            mSignal, SLOT(map()));
    connect(actionGeoVizu, SIGNAL(triggered()),
            mSignal, SLOT(map()));
    connect(actionMetricManager, SIGNAL(triggered()),
            mSignal, SLOT(map()));

    //    Associate an emitter
    mSignal->setMapping(actionMetricCreate, 0);
    mSignal->setMapping(actionMetricData, 1);
    mSignal->setMapping(actionGeoData, 2);
    mSignal->setMapping(actionGeoVizu, 3);
    mSignal->setMapping(actionMetricManager, 4);


    QStackedWidget* stackedWidget = new QStackedWidget(this);
    this->setCentralWidget(stackedWidget);

    this->buildModel();

    MetricExplorer *firstPageWidget = new MetricExplorer(proj_, this);
    firstPageWidget->setTopLevelMetricModel(topLevelMetricModel_);
    DataAnalystFromMetric *secondPageWidget =
            new DataAnalystFromMetric(proj_, this);
    secondPageWidget->setTopLevelMetricModel(topLevelMetricModel_);
    //  DataAnalystFromProperty *thirdPageWidget = new DataAnalystFromProperty(proj_, this);
    //MdsSpaceAnalyst *fourthPageWidget = new MdsSpaceAnalyst(proj_, this);
    MDSSpaceAnalystFrame *fourthPageWidget =
            new MDSSpaceAnalystFrame(proj_,this);

    MDSMetricManagerFrame *fifthPageWidget = new MDSMetricManagerFrame(proj_,this);


    stackedWidget->addWidget(firstPageWidget);
    stackedWidget->addWidget(secondPageWidget);
    //  stackedWidget->addWidget(thirdPageWidget);
    stackedWidget->addWidget(new QWidget(stackedWidget));
    stackedWidget->addWidget(fourthPageWidget);
    stackedWidget->addWidget(fifthPageWidget);

    connect(mSignal, SIGNAL(mapped(int)),
            stackedWidget, SLOT(setCurrentIndex(int)));
    connect(mSignal, SIGNAL(mapped(int)),
            this, SLOT(setMainWindowTitle(int)));


}

mds_gui::~mds_gui()
{

}

void mds_gui::buildModel(){
    SmartPtr<Named_interface> ni =
            Root::instance()->interface(
                metricData_manager + "/metricRegistrar");

    //MetricDataManager* mDataRegistrar = dynamic_cast<MetricDataManager*>(ni.raw_ptr());
    MetricDataManager* mDataRegistrar =
            (MetricDataManager*)(ni.raw_ptr());

    topLevelMetricModel_ = new MetricDomModel(
                mDataRegistrar->getMetricDomDocument(), this);

}

// Just to set the names of windows of the different views
void mds_gui::setMainWindowTitle(int pageNumber){
    if(pageNumber == 0) {
        this->setWindowTitle("Metric Data Explorer");
        this->setWindowIcon(QIcon(":/icons/metricExplorer.png"));
    }
    else if(pageNumber == 1) {
        this->setWindowTitle("Metric Perspective Analysis");
        this->setWindowIcon(QIcon(":/icons/metric2props.png"));
    }
    else if(pageNumber == 2) {
        this->setWindowTitle("Property Perspective Analysis");
        this->setWindowIcon(QIcon(":/icons/props2metrics.png"));
    }
    else if(pageNumber == 3) {
        this->setWindowTitle("Multi-Dimensional Scaling Analysis");
        this->setWindowIcon(QIcon(":/icons/MdsSpace.png"));
    }
}
