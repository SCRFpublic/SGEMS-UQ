#include "mdsparameterexplorertabwidget.h"
#include "ui_mdsparameterexplorertabwidget.h"

#include <parameterExplorer/mdsparametertablemodel.h>
#include <parameterExplorer/parameterplotsubwindow.h>
#include <parameterExplorer/sharedparametersubwindow.h>

#include "param_display_widget.h"
#include "param_search_widget.h"

#include <QDebug>
MDSParameterExplorerTabWidget::MDSParameterExplorerTabWidget(
    Chart_mdi_area* mdi_area,QString mdsObjectName, QWidget *parent) :
    QTabWidget(parent),mdi_area_(mdi_area), mdsSpaceName_(mdsObjectName),
    ui(new Ui::MDSParameterExplorerTabWidget)
{
    ui->setupUi(this);
    this->setTabPosition(QTabWidget::West);


    // Test
    param_display_widget *displayWidget;
    param_search_widget *searchWidget;

    displayWidget = new param_display_widget(this,mdi_area,mdsObjectName);
    searchWidget = new param_search_widget(this,mdi_area, mdsObjectName);

    this->addTab(displayWidget,"Display");
    this->addTab(searchWidget,"Search");



    SmartPtr<Named_interface> ni =
            Root::instance()->interface( mds_manager+"/"+
                                         mdsObjectName.toStdString());

    this->mdsObject_ = dynamic_cast<MultiDimScalingSpace*>(ni.raw_ptr());

    std::vector<GsTLGridProperty*> mdsProperties=
            this->mdsObject_->getProperties();

    for (int i = 0; i < mdsProperties.size(); ++i)
    {
        QString propName = QString::fromStdString(mdsProperties.at(i)->name());

        this->completePropertiesList.push_back(propName);
        this->ui->completePropertiesList->addItem(propName);
        this->ui->completePropertiesList1->addItem(propName);
        this->ui->completePropertiesList2->addItem(propName);
        this->ui->completePropertiesList4->addItem(propName);
    }

    ui->tab1->hide();
    ui->tab_2->hide();
    ui->tab_3->hide();
    ui->tab_4->hide();
    ui->tab->hide();

    // Load clusters
    loadClusterComboBox();

    // Connect + and - buttons
    connect(this->ui->addButton,SIGNAL(clicked()),
            this,SLOT(addProperty()));
    connect(this->ui->completePropertiesList,SIGNAL(itemSelectionChanged()),
            this,SLOT(highlightProperty()));
    connect(this->ui->completePropertiesList2,SIGNAL(itemSelectionChanged()),
            this,SLOT(displayParametersCommon()));
    connect(this->ui->minusButton,SIGNAL(clicked()),
            this,SLOT(removeProperty()));
    connect(this->ui->commonParamSearchButton,SIGNAL(clicked()),
            this,SLOT(findCommonParametersStart()));
    connect(this->ui->completePropertiesList1,SIGNAL(itemSelectionChanged()),
            this,SLOT(displayParameters()));

//    connect(this->ui->selectPropertyButton,SIGNAL(clicked()),
//            this,SLOT(displayParametersCommon()));

    connect(this->ui->findCommonModelButton,SIGNAL(clicked()),
            this,SLOT(findCommonModelsStart()));
    connect(this->ui->numClustersComboBox,SIGNAL(currentIndexChanged(QString)),
            this,SLOT(updateClusterList(QString)));
    connect(this->ui->clusterListWidget,SIGNAL(currentRowChanged(int)),
            this,SLOT(updateClusterPropertyList(int)));

    // Make connection such that everytime a selection of clusterListWidget is
    // made, we update the shared parameters
    connect(ui->propertiesListWidget,SIGNAL(itemSelectionChanged()),
            this,SLOT(updateSharedParameters()));

    connect(ui->completePropertiesList4,SIGNAL(itemSelectionChanged()),
            this,SLOT(updateSharedParamTab()));

    // Make connection between display, emitting signal to MDIarea, and updating
    // the data in parameterVisualizer
    connect(this->ui->displayPushButton,SIGNAL(clicked()),
            this,SLOT(updatedParameterPlots()));

    connect(this->ui->displaySharedParamButton,SIGNAL(clicked()),
            this,SLOT(updateSharedParamPlot()));

    // Set selectable
    this->ui->completePropertiesList->setSelectionMode(
                QAbstractItemView::MultiSelection);
    this->ui->selectedPropertiesList->setSelectionMode(
                QAbstractItemView::MultiSelection);
    this->ui->completePropertiesList1->setSelectionMode(
                QAbstractItemView::SingleSelection);
    this->ui->propertiesListWidget->setSelectionMode(
                QAbstractItemView::MultiSelection);
    this->ui->completePropertiesList4->setSelectionMode(
                QAbstractItemView::MultiSelection);

    // Connect to SQL Database
    this->connectToDataBase();

    //this->setupModel();
}

void MDSParameterExplorerTabWidget::loadClusterComboBox()
{
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

MDSParameterExplorerTabWidget::~MDSParameterExplorerTabWidget()
{
    delete ui;
}

void MDSParameterExplorerTabWidget::addProperty()
{
    this->ui->selectedPropertiesList->clear();
    this->selectedPropertiesList.clear();

    QList<QListWidgetItem*> selProp =
            this->ui->completePropertiesList->selectedItems();

    for (int i = 0; i < selProp.size(); i++)
    {
        this->ui->selectedPropertiesList->
                addItem(selProp.at(i)->data(Qt::DisplayRole).toString());
        this->selectedPropertiesList.
                append(selProp.at(i)->data(Qt::DisplayRole).toString());
    }

    this->ui->selectedPropertiesList->sortItems();
    emit(this->highlightCommonModels(selectedPropertiesList));

}

void MDSParameterExplorerTabWidget::findCommonParametersStart()
{
    QStringList ans = this->findCommonParameters(
                this->selectedPropertiesList);

    emit(this->highlightCommonModels(this->selectedPropertiesList));

    QStringList values = this->queryValues(this->selectedPropertiesList.at(0),
                                           ans);

    parameterModel *resultsModel = new parameterModel(this);

    for (int i = 0; i < ans.size(); i++)
    {
        resultsModel->addNode(ans.at(i),values.at(i),resultsModel->root());
    }

    resultsModel->expandAll();
    resultsModel->resizeColumnToContents(0);
    resultsModel->resizeColumnToContents(1);
    resultsModel->setHeaderHidden(true);
    this->ui->resultsScrollArea->setWidget(resultsModel);

}

void MDSParameterExplorerTabWidget::removeProperty()
{
    QList<QListWidgetItem*> selProp =
            this->ui->selectedPropertiesList->selectedItems();

    for (int i = 0; i < selProp.size(); i++)
    {
        QString delString = selProp.at(i)->data(Qt::DisplayRole).toString();

        int strIndex = this->selectedPropertiesList.indexOf(delString);
        if (strIndex != -1)
        {
            this->selectedPropertiesList.removeAt(strIndex);
        }
    }

    this->ui->selectedPropertiesList->clear();

    for (int i = 0; i < this->selectedPropertiesList.size(); i++)
    {
        this->ui->selectedPropertiesList->addItem(
                    this->selectedPropertiesList.at(i));
    }

}

void MDSParameterExplorerTabWidget::highlightProperty()
{
    QList<QListWidgetItem*> selProp =
            this->ui->completePropertiesList->selectedItems();

    QStringList selectedPropList;
    for (int i = 0; i < selProp.size(); i++)
    {
        selectedPropList.
                append(selProp.at(i)->data(Qt::DisplayRole).toString());
    }
    emit(this->highlightCommonModels(selectedPropList));


}

bool MDSParameterExplorerTabWidget::connectToDataBase()
{
    this->db = QSqlDatabase::addDatabase("QSQLITE");

    // NOTE: We store the database in the tmp folder; should work under both
    // windows and linux
    QString path(QDir::tempPath().append(QDir::separator()));
    path.append(mdsSpaceName_);
    path.append(".sqlite");
    path = QDir::toNativeSeparators(path);

    db.setDatabaseName(path);

    if (!db.open())
    {
        qDebug() << "Failed to open database";
        return false;
    }
    else
    {
        qDebug() << "Read Parameters SQL";
        return true;
    }

}

// Function to iterate through every parameter for all inputed models
// and return the common parameters
QStringList MDSParameterExplorerTabWidget::findCommonParameters(QStringList
                                                                models)
{
    // This is a list of common parameters the models have, (category wise),
    // not necessarily value wise
    QStringList commonParams = this->getSharedParameters(models);

    emit(this->highlightCommonModels(models));

    // Check each shared parameter and find common parameters
    QStringList identicalParameterNames;
    for (int i = 0; i < commonParams.size(); ++i)
    {
        // This is a QStringList that stores the values for the current
        // parameter for each model
        QStringList parameterValue;

        for (int j = 0; j< models.size(); ++j)
        {
            parameterValue.append(this->queryDatabase(commonParams.at(i),
                                                      models.at(j)));
        }

        // Check if every value in parameterValue is equal
        // TODO: the following is absolutely horrendous code;
        // write this properly
        bool allIdentical = true;
        for (int a = 0; a < parameterValue.size(); ++a)
        {
            for (int b = 0; b < parameterValue.size(); ++b)
            {
                allIdentical =
                        !parameterValue.at(a).compare(parameterValue.at(b));
                if (!allIdentical)
                    break;
            }
            if (!allIdentical)
                break;
        }

        if (allIdentical)
            identicalParameterNames.append(commonParams.at(i));
    }

    return identicalParameterNames;
}

QStringList MDSParameterExplorerTabWidget::findCommonModels(QString model,
                                                            QStringList
                                                            parameterList)
{
    // First find out the values for each parameterList for inputed model
    QString algorithmName = mdsObject_->readPropertyAlgorithm(model);
    QStringList values = this->queryValues(model,parameterList);

    QStringList modelList;
    modelList.append(model);

    emit(this->highlightCommonModels(modelList));

    // Now we search our table for models that match ALL given parameters
    QStringList commonModels;
    QSqlQuery query;

    QString queryStr;
    queryStr.append("SELECT parameters$Property_Name$value FROM ");
    queryStr.append(algorithmName);
    queryStr.append( " WHERE ");

    for (int i = 0; i < parameterList.size(); ++i)
    {
        queryStr.append(parameterList.at(i));
        queryStr.append(" = '");
        queryStr.append(values.at(i));
        queryStr.append("'");

        if (i+1<parameterList.size())
        {
            queryStr.append(" AND ");
        }
    }

    query.exec(queryStr);

    while (query.next())
        commonModels.append(query.value(0).toString());

    return commonModels;
}

QStringList MDSParameterExplorerTabWidget::queryValues(QString model,
                                                       QStringList parameterList)
{
    QStringList values;
    for (int i = 0; i < parameterList.size(); ++i)
    {
        QString value = this->queryDatabase(parameterList.at(i),
                                            model);
        values.append(value);
    }

    return values;
}

void MDSParameterExplorerTabWidget::displayParameters()
{

    QList<QListWidgetItem*> selProp =
            this->ui->completePropertiesList1->selectedItems();

    QString selectedProperty;

    // Make sure something is selected
    if (selProp.size()>0)
    {
        parameterModel *displayParameterModel = new parameterModel(this);

        selectedProperty = selProp.at(0)->data(Qt::DisplayRole).toString();

        // Find which algorithm to use
        QString usedAlgorithm =
                mdsObject_->readPropertyAlgorithm(selectedProperty);


        QStringList *completeParameterList =
                mdsObject_->getParameterNames(usedAlgorithm);

        QStringList selectedPropertyList;
        selectedPropertyList.append(selectedProperty);
        emit(this->highlightCommonModels(selectedPropertyList));


        for (int i = 0; i < completeParameterList->size(); ++i)
        {

            QString parameterValue = queryDatabase(completeParameterList->at(i),
                                                   selectedProperty);

            std::cout << "name: " << completeParameterList->at(i).toStdString() << " "
                      << parameterValue.toStdString() << std::endl;
            displayParameterModel->addNode(completeParameterList->at(i),
                                           parameterValue,
                                           displayParameterModel->root());

        }

        displayParameterModel->expandAll();

        // Resize columns to fit parameter names
        displayParameterModel->resizeColumnToContents(0);
        displayParameterModel->resizeColumnToContents(1);
        displayParameterModel->setHeaderHidden(true);

        this->ui->parametersTreeScrollArea->setWidget(displayParameterModel);
    }


}

void MDSParameterExplorerTabWidget::displayParametersCommon()
{

    QList<QListWidgetItem*> selProp =
            this->ui->completePropertiesList2->selectedItems();

    QString selectedProperty;

    // Make sure something is selected
    if (selProp.size() >0)
    {
        this->displayParameterModel = new parameterModel(this);

        selectedProperty = selProp.at(0)->data(Qt::DisplayRole).toString();

        QStringList selectedPropertyList;
        selectedPropertyList.push_back(selectedProperty);

        emit(this->highlightCommonModels(selectedPropertyList));


        QStringList completeParameterList =
                this->mdsObject_->completeParameterNames;
        for (int i = 0; i < completeParameterList.size(); ++i)
        {

            QString result =
                    this->queryDatabase(completeParameterList.at(i),
                                        selectedProperty);

            displayParameterModel->addNode(completeParameterList.at(i),
                                           result,
                                           displayParameterModel->root());
        }

        displayParameterModel->expandAll();
        displayParameterModel->resizeColumnToContents(0);
        displayParameterModel->resizeColumnToContents(1);
        displayParameterModel->setHeaderHidden(true);
        displayParameterModel->setSelectionMode(QAbstractItemView::MultiSelection);
        this->ui->parametersTreeScrollArea2->setWidget(displayParameterModel);
    }
}

void MDSParameterExplorerTabWidget::findCommonModelsStart()
{
    // Get selected models
    QList<QTreeWidgetItem *> selectedItems =
            this->displayParameterModel->selectedItems();

    // Parse selected models into stringlist with full path
    QStringList *fullPaths = new QStringList;
    for (int i = 0; i < selectedItems.size(); i++)
    {
        parameterNode* currentNode = static_cast<parameterNode*>
                (selectedItems.at(i));
        QStringList *results = new QStringList;

        this->generatePaths(currentNode,results);
        for (int j=0; j<results->size(); ++j)
            fullPaths->append(results->at(j));
    }

    // Remove duplicates
    fullPaths->removeDuplicates();

    // run findCommon models
    QString modelName = this->ui->completePropertiesList2->
            selectedItems().at(0)->data(Qt::DisplayRole).toString();

    QStringList commonModels = this->findCommonModels(
                modelName,*fullPaths);

    emit(this->highlightCommonModels(commonModels));

    QStringListModel *resultsModel = new QStringListModel(commonModels);
    this->ui->commonModelsResultsListView->setModel(resultsModel);
}

void MDSParameterExplorerTabWidget::generatePaths(parameterNode *node,
                                                  QStringList *results)
{
    QHash<QString, parameterNode*>::iterator i;
    if (node->children.size() != 0)
    {
        // Run generatePaths over the children nodes
        for (i = node->children.begin(); i!=node->children.end(); ++i)
        {
            this->generatePaths(i.value(),results);
        }
    }
    else
    {
        QString *currentPath = new QString(node->name());
        this->generateParentPath(node,currentPath);
        results->append(*currentPath);
    }

}

void MDSParameterExplorerTabWidget::generateParentPath(parameterNode *node,
                                                       QString *currentPath)
{
    if (node->parent!=0)
    {
        QString parentPath = node->parent->name();
        parentPath.append("$");
        currentPath->prepend(parentPath);
        this->generateParentPath(node->parent,currentPath);
    }

}

void MDSParameterExplorerTabWidget::updateClusterList(QString numClusters)
{
    int numberOfClusters = numClusters.toInt();

    ui->clusterListWidget->clear();

    for (int i = 0; i < numberOfClusters; ++i)
    {
        ui->clusterListWidget->addItem(QString("Cluster ").
                                       append(QString().setNum(i)));
    }

}

void MDSParameterExplorerTabWidget::updateClusterPropertyList(
    int row)
{
    int clusterID = row;
    int numCluster = ui->numClustersComboBox->currentText().toInt();


    std::vector<MDSUtil::clusterBlock> *kmeanClusters;
    kmeanClusters = mdsObject_->getKmeans();

    std::vector<MDSUtil::clusterBlock>::iterator it;
    ui->propertiesListWidget->clear();

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
                    ui->propertiesListWidget->addItem(currentCluster.
                                                      components.at(i).first);
                    selectedClusterNames.push_back(currentCluster.components.at(i).first);
                }
                emit(this->highlightCommonModels(selectedClusterNames));
                break;
            }
        }
    }
}

void MDSParameterExplorerTabWidget::updateSharedParameters()
{
    // Get list of algorithms
    QList<QListWidgetItem *> selectedProperties =
            ui->propertiesListWidget->selectedItems();

    // Generate a String list of selected Property Names
    generatingAlgorithms.clear();
    selectedClusterProperties.clear();


    sharedParameters.clear();

    for (unsigned int i = 0; i < selectedProperties.size(); ++i)
    {
        QString currentPropName =
                selectedProperties.at(i)->data(Qt::DisplayRole).toString();

        QString propAlgorithm = this->mdsObject_->
                readPropertyAlgorithm(currentPropName);

        selectedClusterProperties.append(currentPropName);
    }

    // We now have a list of all algorithms that have been used to generate
    // the selected properties. We need to iterate through and find which are
    // shared
    sharedParameters.clear();
    sharedParameters = this->getSharedParameters(selectedClusterProperties);

    parameterModel *sharedParamModel = new parameterModel(this);
    sharedModel = sharedParamModel;
    // Update Shared Parameter Window
    for (unsigned int i = 0; i < sharedParameters.size(); ++i)
    {
        sharedParamModel->addNode(sharedParameters.at(i), QString(),
                                  sharedParamModel->root());
    }
    emit(this->highlightCommonModels(sharedParameters));
    sharedParamModel->expandAll();
    sharedParamModel->setColumnHidden(1,true);
    sharedParamModel->resizeColumnToContents(0);
    sharedParamModel->resizeColumnToContents(1);
    sharedParamModel->show();
    ui->sharedParameterScrollArea->setWidget(sharedParamModel);
}

void MDSParameterExplorerTabWidget::updatedParameterPlots()
{
    // We need to pass list of parameters and properties
    QList<QListWidgetItem *> selectedProperties =
            ui->propertiesListWidget->selectedItems();
    QList<QTreeWidgetItem *> selectedParameters =
            sharedModel->selectedItems();

    // Parse selected models into stringlist with full path
    QStringList fullPaths,propertyList;
    for (int i = 0; i < selectedParameters.size(); i++)
    {
        parameterNode* currentNode =
                static_cast<parameterNode*>(selectedParameters.at(i));

        QStringList results;

        this->generatePaths(currentNode,&results);
        for (int j=0; j<results.size(); ++j)
            fullPaths.append(results.at(j));
    }

    // Remove duplicates
    fullPaths.removeDuplicates();

    // Generate properties list
    for (unsigned int i = 0; i < selectedProperties.size(); ++i)
    {
        propertyList.append(selectedProperties.at(i)->
                            data(Qt::DisplayRole).toString());
    }

    std::vector<parameterPlotSubWindow*> subWindows;

    // Generate a new parameter subwindow for each plot
    for (unsigned int i = 0; i < fullPaths.size(); ++i)
    {
        if (this->isCastable(fullPaths.at(i),propertyList.at(0)))
        {

            parameterPlotSubWindow* subWindow = new parameterPlotSubWindow
                    (this,mdsSpaceName_,propertyList,fullPaths.at(i));
//            subWindow->setGeometry(0,0,480,320);
//            subWindow->setMaximumSize(480,320);
//            subWindow->setMinimumSize(480,320);
            subWindows.push_back(subWindow);
            mdi_area_->addSubWindow(subWindow);
            subWindow->show();
        }
    }

    //emit this->updateParameterPlots(subWindows);
}

void MDSParameterExplorerTabWidget::updateSharedParamPlot()
{
    // We need to pass list of parameters and properties
    QList<QListWidgetItem *> selectedProperties =
            ui->completePropertiesList4->selectedItems();
    QList<QTreeWidgetItem *> selectedParameters =
            sharedParameterTabModel->selectedItems();

    // Parse selected models into stringlist with full path
    QStringList fullPaths,propertyList;
    for (int i = 0; i < selectedParameters.size(); i++)
    {
        parameterNode* currentNode =
                static_cast<parameterNode*>(selectedParameters.at(i));

        QStringList results;

        this->generatePaths(currentNode,&results);
        for (int j=0; j<results.size(); ++j)
            fullPaths.append(results.at(j));
    }

    // Remove duplicates
    fullPaths.removeDuplicates();

    // Generate properties list
    for (unsigned int i = 0; i < selectedProperties.size(); ++i)
    {
        propertyList.append(selectedProperties.at(i)->
                            data(Qt::DisplayRole).toString());
    }

    std::vector<sharedParameterSubwindow*> subWindows;

    // Generate a new parameter subwindow for each plot
    for (unsigned int i = 0; i < fullPaths.size(); ++i)
    {


        sharedParameterSubwindow* subWindow = new sharedParameterSubwindow
                (this,mdsSpaceName_,propertyList,fullPaths.at(i));
   //     subWindow->setGeometry(0,0,480,320);
   //     subWindow->setMaximumSize(480,320);
   //     subWindow->setMinimumSize(480,320);
        subWindows.push_back(subWindow);
        mdi_area_->addSubWindow(subWindow);
        subWindow->show();
        //emit this->updateSharedPlot(subWindow);

    }

    //emit this->updateParameterPlots(subWindows);
}
bool MDSParameterExplorerTabWidget::isCastable(QString parameter,
                                               QString property)
{
    QString parameterValue = queryDatabase(parameter,property);

    bool ok;
    parameterValue.toFloat(&ok);
    return ok;
}

QString MDSParameterExplorerTabWidget::queryDatabase(QString parameter,
                                                     QString property)
{
    // Connect to our parameters database
    QSqlQuery query(db);

    // Find out which algorithm we are using (thus which table)
    QString algoName_ = mdsObject_->
            readPropertyAlgorithm(property);

    // Make sure we have read an algorithm
    if (algoName_ != "")
    {
        QString queryStr;
        queryStr.append("SELECT ");
        queryStr.append(parameter);
        queryStr.append(" FROM ");
        queryStr.append(algoName_);
        queryStr.append(" WHERE parameters$Property_Name$value = '");
        queryStr.append(property);
        queryStr.append("'");

        query.exec(queryStr);

        QString parameterValue;

        while (query.next())
        {
            parameterValue = query.value(0).toString();
        }

        return parameterValue;

    }
    else
    {
        std::cerr << "Warning: Property "
                  << property.toStdString()
                  << " does not appear to have an algorithm selected"
                  << std::endl;

        // If algorithm read fails, we return an empty string
        return QString();
    }
}


QStringList MDSParameterExplorerTabWidget::getSharedParameters(
    QStringList properties)
{
    // First thing we do generate a stringlist of algorithms that was used to
    // generate the inputed properties
    QStringList usedAlgorithms;
    for (unsigned int i = 0; i < properties.size(); ++i)
    {
        usedAlgorithms.append(
                    mdsObject_->readPropertyAlgorithm(properties.at(i)));
    }
    usedAlgorithms.removeDuplicates();

    // We now check through the list of usedAlgorithms to generate a string
    // list of common parameters

    // Read baseline parameter list
    if (usedAlgorithms.size() > 0)
    {
        QStringList *baselineParameters =
                mdsObject_->getParameterNames(usedAlgorithms.at(0));

        // List to store common parameters
        QStringList commonParameters;

        assert(baselineParameters != 0);

        for (int i = 0; i < baselineParameters->size(); ++i)
        {
            bool commonParam = true;

            for (int j = 0; j < usedAlgorithms.size(); ++j)
            {
                QStringList *checkParam =
                        mdsObject_->getParameterNames(
                            usedAlgorithms.at(j));

                assert(checkParam !=0);

                if (!checkParam->contains(baselineParameters->at(i)))
                {
                    commonParam = false;

                    // If any of the properties don't have this parameter
                    // then there is no point to continue checking the
                    // others
                    break;
                }
            }

            if (commonParam)
                commonParameters.push_back(baselineParameters->at(i));
        }

        return commonParameters;
    }
    else
        return QStringList();
}

void MDSParameterExplorerTabWidget::updateSharedParamTab()
{
    // Get list of algorithms
    QList<QListWidgetItem *> selectedProperties =
            ui->completePropertiesList4->selectedItems();

    // Generate a String list of selected Property Names
    QStringList usedAlgo;
    QStringList sharedParam;
    QStringList selectedPropSParamTab;

    for (unsigned int i = 0; i < selectedProperties.size(); ++i)
    {
        QString currentPropName =
                selectedProperties.at(i)->data(Qt::DisplayRole).toString();

        QString propAlgorithm = this->mdsObject_->
                readPropertyAlgorithm(currentPropName);

        selectedPropSParamTab.append(currentPropName);
    }

    emit(this->highlightCommonModels(selectedPropSParamTab));

    // We now have a list of all algorithms that have been used to generate
    // the selected properties. We need to iterate through and find which are
    // shared
    sharedParam = this->getSharedParameters(selectedPropSParamTab);
    parameterModel* sharedParamTabModel = new parameterModel(this);
    sharedParameterTabModel = sharedParamTabModel;
    // Update Shared Parameter Window
    for (unsigned int i = 0; i < sharedParam.size(); ++i)
    {
        sharedParamTabModel->addNode(sharedParam.at(i), QString(),
                                     sharedParamTabModel->root());
    }
    sharedParamTabModel->expandAll();
    sharedParamTabModel->setColumnHidden(1,true);
    sharedParamTabModel->resizeColumnToContents(0);
    sharedParamTabModel->resizeColumnToContents(1);
    sharedParamTabModel->show();
    ui->sharedParamScrollArea->setWidget(sharedParamTabModel);
}
