#ifndef MDSPARAMETEREXPLORERTABWIDGET_H
#define MDSPARAMETEREXPLORERTABWIDGET_H

#include <QTabWidget>
#include <common.h>

#include <metric_manager_repository.h>
#include <MetricData.h>
#include <MetricDataManager.h>
#include <GsTL/utils/smartptr.h>
#include <GsTLAppli/utils/manager.h>
#include <GsTLAppli/appli/manager_repository.h>
#include <GsTLAppli/gui/charts/chart_mdi_area.h>
#include <QDebug>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QFile>
#include <QDir>

#include <QHash>
#include <QDomDocument>
#include <QXmlDefaultHandler>
#include <iostream>

#include <multi_dim_scaling_space.h>
#include <QStringList>
#include <QStringListModel>
#include <QString>

#include <QItemSelectionModel>

#include <iostream>

class parameterNode;
class parameterModel;

class parameterPlotSubWindow;
class sharedParameterSubwindow;

namespace Ui {
class MDSParameterExplorerTabWidget;
}

class MDSParameterExplorerTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit MDSParameterExplorerTabWidget(Chart_mdi_area* mdi_area,
                                           QString mdsObjectName = QString(""),QWidget *parent=0);
    ~MDSParameterExplorerTabWidget();

    void loadClusterComboBox();

protected:
    MultiDimScalingSpace* mdsObject_;

    bool connectToDataBase();
    QSqlDatabase db;
    QStringList completeParameterNames;
    QString mdsSpaceName_;

    QStringList selectedClusterProperties;
    QStringList sharedParameters;
    QList<QString> generatingAlgorithms;

    QString queryDatabase(QString parameter, QString property);

    // This function returns names of parameters that are shared among a list
    // of properties
    QStringList getSharedParameters(QStringList properties);

private slots:
    void addProperty();
    void highlightProperty();
    void removeProperty();

    void findCommonParametersStart();
    void findCommonModelsStart();

    void displayParameters();
    void displayParametersCommon();

    void updateClusterList(QString numClusters);
    void updateClusterPropertyList(int row);

    void updateSharedParameters();
    void updatedParameterPlots();

    // This loads shared parameters on the sharedparam tab
    void updateSharedParamTab();
    void updateSharedParamPlot();

signals:
    void displayClusterChart(QString mdsObjName);
    void updateParameterPlots(std::vector<parameterPlotSubWindow*> subWindows);
    void updateSharedPlot(sharedParameterSubwindow* subWindow);
    void highlightCommonModels(QStringList &selectModels);

private:
  Chart_mdi_area* mdi_area_;
    QStringList completePropertiesList;
    QStringListModel *completePropertiesListModel;
    QStringList selectedPropertiesList;
    QStringListModel *selectedPropertiesListModel;
    Ui::MDSParameterExplorerTabWidget *ui;

    parameterModel *displayParameterModel;
    QSqlTableModel* model;

    // Given a list of models find all common parameters
    QStringList findCommonParameters(QStringList models);

    // Given a model and parameter(s), find all other models that have the
    // same parameter
    QStringList findCommonModels(QString model,
                                 QStringList parameterList);

    QStringList queryValues(QString model,
                            QStringList parameterList);

    // Generate castable properties
    bool isCastable(QString parameter, QString property);

    // Generate path - given a parameterNode, this function recursively
    // geneates the full path of the node
    void generatePaths(parameterNode *node, QStringList *results);

    // Generate parent path - helper function used by generatePaths to trace
    // path to parent
    void generateParentPath(parameterNode *node, QString *currentPath);

    parameterModel *sharedModel;

    parameterModel *sharedParameterTabModel;
};

#endif // MDSPARAMETEREXPLORERTABWIDGET_H
