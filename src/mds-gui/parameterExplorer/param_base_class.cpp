#include "param_base_class.h"
#include "param_plot_window.h"
#include "mdsparametertablemodel.h"

param_base_class::param_base_class(Chart_mdi_area *mdi,
                                   QString mdsSpaceName):
    mdiParent_(mdi), mdsSpaceName_(mdsSpaceName)
{

    // Obtain pointer to MDS Object
    SmartPtr<Named_interface> ni =
            Root::instance()->interface( mds_manager+"/"+
                                         mdsSpaceName_.toStdString());

    mdsObject_ = dynamic_cast<MultiDimScalingSpace*>(ni.raw_ptr());

    // Get complete list of properties from MDS Object
    std::vector<Grid_continuous_property*> mdsProps = mdsObject_->getProperties();

    for (unsigned int i = 0; i < mdsProps.size(); ++i)
        propertyList_.push_back(QString::fromStdString(mdsProps.at(i)->name()));

    // Connect to SQL Database
    connectToDatabase();

    commonParametersModel = new parameterModel();

    realizationsWidget.setSelectionMode(QAbstractItemView::MultiSelection);
}

bool param_base_class::connectToDatabase()
{
    db_ = QSqlDatabase::addDatabase("QSQLITE");

    QString path(QDir::tempPath().append(QDir::separator()));
    path.append(mdsSpaceName_);
    path.append(".sqlite");
    path = QDir::toNativeSeparators(path);

    db_.setDatabaseName(path);

    if (!db_.open())
    {
        qDebug() << "Failed to open database";
        return false;
    }
    else
    {
        return true;
    }
}

/**
 *  queryDatabase(QString &parameter, QString &realization)
 *  Function that queries the database for the given parameter and realization
 *  and returns a string of the parameter value.
 *
 *  @param parameter QString - the name of the parameter we are looking up
 *  @param realization QString - the name of the realization we are looking up
 *  @return QString
 *
 */
QString param_base_class::queryDatabase(const QString &parameter,
                                        const QString &realization)
{
    // Connect to parameters database
    QSqlQuery query(db_);

    // Find out which algorithm we are using (thus which table)
    QString algoName_ = mdsObject_->
            readPropertyAlgorithm(realization);

    // Make sure we have read an algorithm
    if (algoName_ != "")
    {
        QString queryStr;
        queryStr.append("SELECT ");
        queryStr.append(parameter);
        queryStr.append(" FROM ");
        queryStr.append(algoName_);
        queryStr.append(" WHERE parameters$Property_Name$value = '");
        queryStr.append(realization);
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
                  << realization.toStdString()
                  << " does not appear to have an algorithm selected"
                  << std::endl;

        // If algorithm read fails, we return an empty string
        return QString();
    }
}
/**
 *  queryValues(QStringList &parameters, QString &realization)
 *  Function that queries the database for the given parameters and realization
 *  and returns a stringlist of the parameter values.
 *
 *  @param parameter QStringList - the names of the parameters we are looking up
 *  @param realization QString - the name of the realization we are looking up
 *  @return QStringList
 *
 */
QStringList param_base_class::queryValues(const QStringList &parameters,
                                          const QString &realization)
{
    QStringList values;

    for (int i = 0; i < parameters.size(); ++i)
    {
        QString value = queryDatabase(parameters.at(i),realization);
        values.append(value);
    }

    return values;
}

bool param_base_class::isCastable(const QString &valueStr, float &valueFlt)
{
    bool ok;
    valueFlt = valueStr.toFloat(&ok);
    return ok;
}

/**
 *  getParameterList(QString algoName)
 *  Function that returns all the parameters specified by the algorithm in algo
 *  name and puts into parameterList_
 *
 *  @param algoName QString - the name of the algorithm we are looking up
 *  @return bool
 *
 */
void param_base_class::getParameterList(const QString &realization)
{
    QString usedAlgorithm =
            mdsObject_->readPropertyAlgorithm(realization);

    QStringList *completeParameterList =
            mdsObject_->getParameterNames(usedAlgorithm);

    displayParameterModel = new parameterModel();

    parameterList_.clear();

    for (int i = 0; i < completeParameterList->size(); ++i)
    {

        QString parameterValue = queryDatabase(completeParameterList->at(i),
                                               realization);

        displayParameterModel->addNode(completeParameterList->at(i),
                                       parameterValue,
                                       displayParameterModel->root());

        // Generate a geoStatParam object
        geoStatParam currentParam;
        currentParam.str = parameterValue;
        currentParam.type = isCastable(currentParam.str,currentParam.val);

        parameterList_.insert(completeParameterList->at(i), currentParam);
    }

    displayParameterModel->expandAll();

    // Resize columns to fit parameter names
    displayParameterModel->resizeColumnToContents(0);
    displayParameterModel->resizeColumnToContents(1);
    displayParameterModel->setHeaderHidden(true);
}


/**
 *  flattenPath(parameterNode *inputNode)
 *  Function that the full path of the node by flattening all its parent nodes
 *
 *  @param inputNode parameterNode - node whose path we want to generate
 *  @return QString
 *
 */
QString param_base_class::flattenPath(parameterNode *inputNode)
{
    QHash<QString, parameterNode*>::iterator i;

    QString *currentPath = new QString(inputNode->name());

    traversePath(inputNode,currentPath);

    return *currentPath;
}

/**
 *  traversePath(parameterNode *inputNode,QString *currentPath)
 *  Recursive function that prepends parent node names to currentPath
 *
 *  @param inputNode parameterNode - node whose path we want to generate
 *  @param currentPath QString* - the path to prepend to
 *  @return void
 *
 */
void param_base_class::traversePath(parameterNode *node, QString *currentPath)
{
    if (node->parent!=0)
    {
        QString parentPath = node->parent->name();
        parentPath.append("$");
        currentPath->prepend(parentPath);
        traversePath(node->parent,currentPath);
    }
}

bool param_base_class::update()
{
  return true;
}

/**
 *  findSharedParameters(QStringList &realizations)
 *  Function that given a set of realizations, finds all parameters that are
 *  common to all of them.
 *  @param QStringList &realizations - list of realizations
 *  @return QStringList of parameters
 *
 */
QStringList param_base_class::findSharedParameters(const QStringList
                                                   &realizations)
{
    // Determine which algorithms were used to generate the inputed realizations
    QStringList generatingAlgos;
    for (unsigned int i = 0; i < realizations.size(); ++i)
        generatingAlgos.append(
                    mdsObject_->readPropertyAlgorithm(realizations.at(i)));
    generatingAlgos.removeDuplicates();

    // We iterate through generatingAlgos to generate stringlist of shared
    // parameters
    if (generatingAlgos.size() > 0)
    {
        QStringList *baselineParameters =
                mdsObject_->getParameterNames(generatingAlgos.at(0));

        assert(baselineParameters != 0);

        // List to store common parameters
        QStringList sharedParameters;

        for (int i = 0; i < baselineParameters->size(); ++i)
        {
            bool sharedParam = true;

            for (int j = 1; j < generatingAlgos.size(); ++j)
            {
                QStringList *checkParam =
                        mdsObject_->getParameterNames(
                            generatingAlgos.at(j));

                assert(checkParam !=0);

                if (!checkParam->contains(baselineParameters->at(i)))
                {
                    sharedParam = false;

                    // If any of the properties don't have this parameter
                    // then there is no point to continue checking the
                    // others
                    break;
                }
            }

            if (sharedParam)
                sharedParameters.push_back(baselineParameters->at(i));
        }

        return sharedParameters;
    }
    else
        return QStringList();
}


parameterModel *param_base_class::findCommonParameters()
{
    // Generate list of selected realization names
    QStringList selectedRealNames;

    for (unsigned int i = 0; i < realizationsWidget.selectedItems().size(); ++i)
    {
        QString realName =
                realizationsWidget.selectedItems().at(i)->
                data(Qt::DisplayRole).toString();

        selectedRealNames.append(realName);
    }

    // Find the shared parameter names
    QStringList sharedParameters = findSharedParameters(selectedRealNames);

    parameterModel *sharedParameterModel = new parameterModel();
    commonParametersModel = sharedParameterModel;

    for (unsigned int i = 0; i < sharedParameters.size(); ++i)
    {
        sharedParameterModel->addNode(sharedParameters.at(i),QString(),
                                      sharedParameterModel->root());
    }

    sharedParameterModel->setColumnHidden(1,true);
    sharedParameterModel->resizeColumnToContents(0);
    sharedParameterModel->resizeColumnToContents(1);
    sharedParameterModel->show();

    return sharedParameterModel;
}


void param_base_class::createParameterPlotWindow()
{
    QModelIndexList selectedModels =
            realizationsWidget.selectionModel()->selectedIndexes();

    QList<QTreeWidgetItem *> selectedParameter =
            commonParametersModel->selectedItems();

    QString currentSearchParam_;
    if (selectedParameter.size() == 0)
        return;
    else
    {
        parameterNode* currentNode = static_cast<parameterNode*>
                (selectedParameter.at(0));
        currentSearchParam_ = flattenPath(currentNode);
    }

    plotParameters_.clear(); plotValues_.clear();

    std::vector<int> cont;

    for (unsigned int i = 0; i < selectedModels.size(); ++i)
    {
        cont.push_back(selectedModels.at(i).row());
    }
    std::sort(cont.begin(),cont.end());
    cont.erase(std::unique(cont.begin(), cont.end()), cont.end());

    for (unsigned int i = 0; i < cont.size(); ++i)
    {
        QListWidgetItem *selectedParamItem =
                realizationsWidget.item(cont.at(i));
        QString realizationName = selectedParamItem->
                data(Qt::DisplayRole).toString();

        plotParameters_.append(realizationName);

        QString valueStr = queryDatabase(currentSearchParam_,realizationName);
        float value;

        if (isCastable(valueStr,value))
            plotValues_.push_back(value);
        else
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText("The selected parameter is non-numeric. Cannot plot");
            msgBox.exec();

            return;
        }

    }

    param_plot_window * subWindow = new param_plot_window();
    subWindow->plot(plotParameters_,plotValues_,currentSearchParam_);
    mdiParent_->addSubWindow(subWindow);
    subWindow->show();

}




