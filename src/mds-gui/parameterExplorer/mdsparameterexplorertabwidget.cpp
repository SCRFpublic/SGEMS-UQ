#include "mdsparameterexplorertabwidget.h"
#include "ui_mdsparameterexplorertabwidget.h"

#include <parameterExplorer/mdsparametertablemodel.h>

#include <QDebug>
MDSParameterExplorerTabWidget::MDSParameterExplorerTabWidget(
    Chart_mdi_area* mdi_area,QString mdsObjectName, QWidget *parent) :
    QTabWidget(parent),mdi_area_(mdi_area), mdsSpaceName_(mdsObjectName),
    ui(new Ui::MDSParameterExplorerTabWidget)
{
    ui->setupUi(this);

    this->setTabPosition(QTabWidget::West);


    // Test

    displayWidget = new param_display_widget(this,mdi_area,mdsObjectName);
    searchWidget = new param_search_widget(this,mdi_area, mdsObjectName);
    sensitivityWidget = new param_sensitivity_widget(this,mdi_area,mdsObjectName);
    plotWidget = new param_plot_widget(this,mdi_area,mdsObjectName);
    clusterWidget = new param_cluster_widget(this,mdi_area,mdsObjectName);
    commonWidget = new param_commonModels_widget(this,mdi_area,mdsObjectName);
    commonParamWidget = new param_commonParam_widget(this,mdi_area,mdsObjectName);

    this->addTab(displayWidget,"Display");
    this->addTab(searchWidget,"Search");
    this->addTab(sensitivityWidget, "Sensitivity");
    this->addTab(plotWidget,"Plot");
    this->addTab(clusterWidget,"Clutering");
    this->addTab(commonWidget,"CommonModel");
    this->addTab(commonParamWidget,"CommonParamW");

}

void MDSParameterExplorerTabWidget::loadClusterComboBox()
{
    this->clusterWidget->updateClusterComboBox();
}

MDSParameterExplorerTabWidget::~MDSParameterExplorerTabWidget()
{
    delete ui;
}
