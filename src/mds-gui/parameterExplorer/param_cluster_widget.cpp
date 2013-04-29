#include "param_cluster_widget.h"
#include "mdsparametertablemodel.h"
#include "ui_param_cluster_widget.h"
#include <mdsutil.h>
#include <vector>

param_cluster_widget::param_cluster_widget(QWidget *parent, Chart_mdi_area *mdi,
                                           QString mdsSpaceName) :
          QWidget(parent), param_base_class(mdi,mdsSpaceName),
    ui(new Ui::param_cluster_widget)
{
    ui->setupUi(this);
    connect(this->ui->numClustersComboBox,SIGNAL(currentIndexChanged(QString)),
            this,SLOT(updateClusterList(QString)));
    connect(this->ui->clusterListWidget,SIGNAL(currentRowChanged(int)),
            this,SLOT(updateClusterPropertyList(int)));
    connect(&realizationsWidget,SIGNAL(itemSelectionChanged()),
            this,SLOT(updateClusterParameters()));
    connect(ui->displayPushButton,SIGNAL(clicked()),
            this,SLOT(plotParameterValues()));

    ui->realizationScrollArea->setWidget(&this->realizationsWidget);
}

param_cluster_widget::~param_cluster_widget()
{
    delete ui;
}

void param_cluster_widget::updateClusterList(QString numClusters)
{
    int numberOfClusters = numClusters.toInt();

    ui->clusterListWidget->clear();

    for (int i = 0; i < numberOfClusters; ++i)
    {
        ui->clusterListWidget->addItem(QString("Cluster ").
                                       append(QString().setNum(i)));
    }

}

void param_cluster_widget::updateClusterPropertyList(int row)
{
    int clusterID = row;
    int numCluster = ui->numClustersComboBox->currentText().toInt();

    std::vector<MDSUtil::clusterBlock> *kmeanClusters;
    kmeanClusters = mdsObject_->getKmeans();

    std::vector<MDSUtil::clusterBlock>::iterator it;
    realizationsWidget.clear();

    // TODO: Switch the cluster saving system to a Map or Hash table rather than
    // vector.
    for (it = kmeanClusters->begin(); it != kmeanClusters->end(); it++)
    {
        // Find block with cluster
        if (it->numBlock == numCluster)
        {
            std::vector<MDSUtil::cluster> currentClusters = it->block;
            if (clusterID < currentClusters.size())
            {
                MDSUtil::cluster currentCluster = currentClusters[clusterID];

                QStringList selectedClusterNames;
                for (unsigned int i = 0; i < currentCluster.numComponents; ++i)
                {
                    realizationsWidget.addItem(currentCluster.
                                                      components.at(i).first);
                    selectedClusterNames.push_back(currentCluster.components.at(i).first);
                }
          //      emit(this->highlightCommonModels(selectedClusterNames));
                break;
            }
        }
    }
}

void param_cluster_widget::updateClusterComboBox()
{
    // Load cluster combo box
    std::vector<MDSUtil::clusterBlock> *kmeanClusters;
    kmeanClusters = mdsObject_->getKmeans();

    std::vector<MDSUtil::clusterBlock>::iterator it;

    ui->numClustersComboBox->clear();
    for (it = kmeanClusters->begin(); it != kmeanClusters->end(); it++)
    {
        // If it does not already exist, add to combobox
        ui->numClustersComboBox->addItem(QString().setNum(it->numBlock));
    }
}

void param_cluster_widget::updateClusterParameters()
{
    ui->commonParametersScrollArea->setWidget(
                findCommonParameters());
}

void param_cluster_widget::plotParameterValues()
{
createParameterPlotWindow();
}

bool param_cluster_widget::update()
{

}

