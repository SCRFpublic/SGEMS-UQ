#include "mdsspaceanalystframe.h"
#include "ui_mdsspaceanalystframe.h"

#include <metric_utilities.h>
#include <multi_dim_scaling_space.h>

MDSSpaceAnalystFrame::MDSSpaceAnalystFrame(GsTL_project *proj, QWidget *parent) :
    QFrame(parent), proj_ (proj), ui(new Ui::MDSSpaceAnalystFrame)
{
    // Load GUI
    ui->setupUi(this);

    this->setLayout(this->ui->gridLayout);

    // Intialize MDI area
    this->mdiArea_ = new MdsMdiArea(this);
    this->ui->viewerWidget->setWidget(this->mdiArea_);

    this->setMDSTreeView();

    SmartPtr<Named_interface> mdsMan= Root::instance()->interface(mds_manager);

    Manager* manager = dynamic_cast<Manager*>(mdsMan.raw_ptr());

    manager->factory("mds_io_action",
                     UncertainSpaceIOAction::create_new_interface);
    manager->factory("mds_action",MDS_action::create_new_interface);

    // Create an empty model that we will use as a placeholder
    this->emptyModel = new QStandardItemModel(this);


    // Connect signals for creating/deleting MDS Spaces
    connect(this->ui->createMDSSpaceButton,SIGNAL(clicked()),
            this,SLOT(createNewMDSSpace()));
    connect(this->ui->deleteMDSSpaceButton,SIGNAL(clicked()),
            this,SLOT(deleteMDSSpace()));
    connect(this->ui->loadMDSSpaceButton, SIGNAL(clicked()),
            this,SLOT(loadMDSSpace()));
    connect(this->ui->saveMDSSpaceButton, SIGNAL(clicked()),
            this,SLOT(saveMDSSpace()));
    connect(this->ui->mdsListTreeView,SIGNAL(clicked(QModelIndex)),
            this,SLOT(updatePropertyExplorer(QModelIndex)));
    connect(this->ui->mdsListTreeView,SIGNAL(clicked(QModelIndex)),
            this,SLOT(updateResponseExplorer(QModelIndex)));
}

MDSSpaceAnalystFrame::~MDSSpaceAnalystFrame()
{
    delete ui;
}

void MDSSpaceAnalystFrame::createNewMDSSpace()
{
    MDSSpaceCreatorDialog* mdsSpaceCreatorDialog =
            new MDSSpaceCreatorDialog(proj_,this);
    mdsSpaceCreatorDialog->show();
}

void MDSSpaceAnalystFrame::deleteMDSSpace()
{
    // Find the currently selected MDS Space
    QModelIndex indexToDelete = this->ui->mdsListTreeView->currentIndex();


    QString deleteSpaceName = this->proxy_model->data(indexToDelete,
                                                      Qt::DisplayRole).toString();

    deleteSpaceName.prepend("/");

    // Remove space from root manager
    Root::instance()->delete_interface(mds_manager +
                                       deleteSpaceName.toStdString());

    this->ui->propertiesTreeExplorer->setModel(this->emptyModel);
    this->ui->responsesTreeExplorer->setModel(this->emptyModel);
}

void MDSSpaceAnalystFrame::saveMDSSpace()
{
    // Get filename of XML file where we want to save XML file
    QString path = QFileDialog::getSaveFileName(this, "Select save location",
                                                QString("/home/lewisli/code-dev/sgems-plugin-dev/mds_gui/mdsspace"),
                                                tr("*.xml"));

    QModelIndex currentIndex = this->ui->mdsListTreeView->currentIndex();


    QString mdsObjectName = this->proxy_model->data(currentIndex,
                                                    Qt::DisplayRole).toString();


    SmartPtr<Named_interface> ni =
            Root::instance()->interface( mds_manager+"/"+
                                         mdsObjectName.toStdString());

    MultiDimScalingSpace* mdsObject_ = dynamic_cast<MultiDimScalingSpace*>
            (ni.raw_ptr());

    // Generate new XML Document
    QDomDocument *configurationParameter = new QDomDocument("MDSParameters");

    // Create Root Element
    QDomElement root = configurationParameter->createElement("MDS");
    configurationParameter->appendChild(root);

    // Write grid name: Here is a problem we can't handle multiple grid names
    QDomElement grid = configurationParameter->createElement("Grid");
    grid.setAttribute("name","grid");

    // Write grid elements
    for (int i = 0;
         i < mdsObject_->pointCount(); i++)
    {
        QDomElement prop = configurationParameter->createElement("Property");
        prop.setAttribute("name",QString(mdsObject_->getPropertyName(i).
                                         c_str()) );
        grid.appendChild(prop);
    }

    // Get metric names
    std::vector<std::string> metricName = mdsObject_->getMetricNames();
    // Write Metric Tag
    QDomElement metric = configurationParameter->createElement("MetricData");

    // Write metric
    for (int i = 0; i < metricName.size(); i++)
    {
        QDomElement met = configurationParameter->createElement("Metric");
        met.setAttribute("name",QString(metricName.at(i).c_str()));
        metric.appendChild(met);
    }

    // Write Kernel Type
    QDomElement kernelType = configurationParameter->createElement("Kernel");
    kernelType.setAttribute("type", QString("Euclidean"));

    // Write space name
    QDomElement spaceName = configurationParameter->createElement("Spacename");
    spaceName.setAttribute("name", mdsObjectName);

    // Write cluster information
    const std::vector< MDSUtil::clusterBlock> *kmeanResults =
            mdsObject_->getKmeans();

    QDomElement clusterResult=configurationParameter->createElement("Clusters");

    // For the first level of cluster result we will write the clusters kmeans
    for (unsigned int i = 0 ; i < kmeanResults->size(); i++)
    {
        QString numstring;
        numstring.append(QString("%1").arg(kmeanResults->at(i).numBlock));

        QDomElement clustersUsed =
                configurationParameter->createElement("ClustersUsed");
        clustersUsed.setAttribute("num",numstring);

        // Second level we iterate through each code block
        for (unsigned int j = 0; j < kmeanResults->at(i).numBlock; ++j)
        {

            QDomElement clustersID =
                    configurationParameter->createElement("ClusterID");
            numstring.clear();
            numstring.append(QString("%1").arg(j));
            clustersID.setAttribute("ID",numstring);

            // Third level we iterate through each cluster and put all the point
            unsigned int numPtsInCluster = kmeanResults->at(i).
                    block.at(j).numComponents;

            for (unsigned int k = 0; k < numPtsInCluster; ++k)
            {
                QDomElement pointID =
                        configurationParameter->createElement("Property");
                pointID.setAttribute("name", kmeanResults->at(i).block.at(j).
                                     components.at(k).first);
                pointID.setAttribute("num",kmeanResults->at(i).block.at(j).
                                     components.at(k).second);
                clustersID.appendChild(pointID);
            }

            clustersUsed.appendChild(clustersID);
        }
        clusterResult.appendChild(clustersUsed);
    }

    // Put under root
    root.appendChild(kernelType);
    root.appendChild(grid);
    root.appendChild(metric);
    root.appendChild(clusterResult);
    root.appendChild(spaceName);

    SmartPtr<Named_interface> ni1 =
            Root::instance()->new_interface("mds_io_action",
                                            mds_manager + "/temp");
    UncertainSpaceIOAction* newAction1 =
            dynamic_cast<UncertainSpaceIOAction*> (ni1.raw_ptr());

    Error_messages_handler temp1;
    string filename = path.toStdString();
    string testcase = "Write";

    newAction1->init(testcase,&*this->proj_,&temp1);
    newAction1->initFromDom(configurationParameter, filename);

    Root::instance()->delete_interface(mds_manager + "/temp");
}

void MDSSpaceAnalystFrame::loadMDSSpace()
{
    // Get filename of XML file where we have saved a particular XML file
    QString path = QFileDialog::getOpenFileName(this,"Select valid XML file",
                                                QString("/home/lewisli/code-dev/"),
                                                tr("*.xml"));

    SmartPtr<Named_interface> ni1 =
            Root::instance()->new_interface("mds_io_action",
                                            mds_manager + "/tempRead");
    UncertainSpaceIOAction* newAction1 =
            dynamic_cast<UncertainSpaceIOAction*> (ni1.raw_ptr());

    QDomDocument *configurationParameter = new QDomDocument("MDSParameters");
    Error_messages_handler temp1;
    string filename = path.toStdString();
    string testcase = "Read";

    newAction1->init(testcase,&*this->proj_,&temp1);
    newAction1->initFromDom(configurationParameter, filename);

    Root::instance()->delete_interface(mds_manager + "/tempRead");
}

void MDSSpaceAnalystFrame::setMDSTreeView()
{
    this->proxy_model = new Filter_root_proxy_model(this);
    this->ui->mdsListTreeView->setModel(proxy_model);
    //this->mdsBaseIndex = proxy_model->mds_root_index();
    this->mdsBaseIndex = proxy_model->manager_index(
                QString::fromStdString(mds_manager));
    this->ui->mdsListTreeView->setRootIndex(this->mdsBaseIndex);
    this->ui->mdsListTreeView->setItemsExpandable(false);

    // Allow for objects to be dragged from list view into MDI area
    this->ui->mdsListTreeView->setDragEnabled(true);
    this->ui->mdsListTreeView->setDragDropMode(QAbstractItemView::DragOnly);

}


void MDSSpaceAnalystFrame::updatePropertyExplorer(QModelIndex mdsSpaceIndex)
{
    // Create a sort filter
    QString spaceName = this->proxy_model->data(mdsSpaceIndex,
                                                Qt::DisplayRole).toString();

    this->ui->propertiesTreeExplorer->setModel(this->proxy_model);
    this->ui->propertiesTreeExplorer->setRootIndex(
                ModelUtilities::mds_metric_properties_root_index(
                    this->proxy_model, spaceName));

}

void MDSSpaceAnalystFrame::updateResponseExplorer(QModelIndex mdsSpaceIndex)
{
    // Create a sort filter
    QString spaceName = this->proxy_model->data(mdsSpaceIndex,
                                                Qt::DisplayRole).toString();

    this->ui->responsesTreeExplorer->setModel(this->proxy_model);
    this->ui->responsesTreeExplorer->setRootIndex(
                ModelUtilities::mds_metric_response_root_index(
                    this->proxy_model, spaceName));

}


