#include "mdsmetricmanagerframe.h"
#include "ui_mdsmetricmanagerframe.h"

#include <metric_utilities.h>

MDSMetricManagerFrame::MDSMetricManagerFrame(GsTL_project *proj,
                                             QWidget *parent) :
    QFrame(parent), proj_ (proj),
    ui(new Ui::MDSMetricManagerFrame)
{
    ui->setupUi(this);

    // Set up Tree-Views
    this->setGridTreeView();
    this->setResponseTreeView();

    // FIX THIS HARD-CODE
    this->metricPlotsMdi = new MetricPlotsMdiArea(this,QString("grid2"));
    this->ui->scrollArea->setWidget(this->metricPlotsMdi);

    SmartPtr<Named_interface> metricMan=
            Root::instance()->interface(metricData_manager);

    Manager* manager = dynamic_cast<Manager*>(metricMan.raw_ptr());

    manager->factory("response_io_action",
                     ExternalResponseIOAction::create_new_interface);

    connect(this->ui->responseListTreeView,SIGNAL(clicked(QModelIndex)),
            this,SLOT(updatePropertyExplorer(QModelIndex)));
    connect(this->ui->responseListTreeView,SIGNAL(activated(QModelIndex)),
            this,SLOT(updatePropertyExplorer(QModelIndex)));
    connect(this->ui->responseListTreeView,SIGNAL(pressed(QModelIndex)),
            this,SLOT(updatePropertyExplorer(QModelIndex)));
    connect(this->ui->loadResponseButton,SIGNAL(clicked()),
            this,SLOT(loadResponse()));
    connect(this->ui->gridListTreeView,SIGNAL(clicked(QModelIndex)),
            this,SLOT(updateGridSelection(QModelIndex)));

  //  connect(this->ui->gridListTreeView,SIGNAL(clicked(QModelIndex)),
  //          metricPlotsMdi,SLOT(changeSelectedGrid(QModelIndex)))
}

MDSMetricManagerFrame::~MDSMetricManagerFrame()
{
    delete ui;
}

void MDSMetricManagerFrame::setGridTreeView()
{
    grid_proxy_model = new Filter_root_proxy_model(this);
    ui->gridListTreeView->setModel(grid_proxy_model);
    ui->gridListTreeView->setRootIndex(grid_proxy_model->project_root_index());
    ui->gridListTreeView->setItemsExpandable(false);
}

void MDSMetricManagerFrame::updateGridSelection(QModelIndex gridIndex)
{
    // Get name of grid
    qDebug() << grid_proxy_model->data(
                    gridIndex,Qt::DisplayRole).
                toString();
    metricPlotsMdi->changeSelectedGrid(grid_proxy_model->data(
                                           gridIndex,Qt::DisplayRole).
                                           toString());
}

void MDSMetricManagerFrame::setResponseTreeView()
{
    this->proxy_model = new Filter_root_proxy_model(this);
    this->ui->responseListTreeView->setModel(proxy_model);
    this->responseBaseIndex =
            ModelUtilities::response_root_index(this->proxy_model);
    this->ui->responseListTreeView->setRootIndex(this->responseBaseIndex);
    this->ui->responseListTreeView->setItemsExpandable(false);

    // Allow for objects to be dragged from list view into MDI area
    this->ui->responseListTreeView->setDragEnabled(true);
    this->ui->responseListTreeView->setDragDropMode(QAbstractItemView::DragOnly);

    // Allow for individual models to be dragged in as well
    this->ui->propertyListTreeView->setDragEnabled(true);
    this->ui->propertyListTreeView->setDragDropMode(QAbstractItemView::DragOnly);
    this->ui->propertyListTreeView->setSelectionMode(QAbstractItemView::MultiSelection);
}

void MDSMetricManagerFrame::updatePropertyExplorer(QModelIndex responseIndex)
{
    // Create a sort filter
    QString responseName = this->proxy_model->data(responseIndex,
                                                Qt::DisplayRole).toString();

    this->metricPlotsMdi->changedSelectedMetric(responseName);

    this->ui->propertyListTreeView->setModel(this->proxy_model);
    this->ui->propertyListTreeView->setRootIndex(
                 ModelUtilities::response_property_root_index(this->proxy_model,responseName));
}

void MDSMetricManagerFrame::loadResponse()
{
    SmartPtr<Named_interface> ni1 =
            Root::instance()->new_interface("response_io_action",
                                            metricData_manager + "/tempRead");
    ExternalResponseIOAction* newAction =
            dynamic_cast<ExternalResponseIOAction*> (ni1.raw_ptr());

    QDomDocument *configurationParameter = new QDomDocument("MDSParameters");
    Error_messages_handler temp1;


    QString path = QFileDialog::getOpenFileName(this,"Select valid XML file",
                                                QString("/home/lewisli/code-dev/"),
                                                tr("*.xml"));
    string testcase = "Read";

    string filename = path.toStdString();

    newAction->init(testcase,&*this->proj_,&temp1);
    newAction->initFromDom(configurationParameter, filename);

    Root::instance()->delete_interface(metricData_manager + "/tempRead");

}
