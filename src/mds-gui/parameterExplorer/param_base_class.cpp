#include "param_base_class.h"

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
    std::vector<GsTLGridProperty*> mdsProps = mdsObject_->getProperties();

    for (unsigned int i = 0; i < mdsProps.size(); ++i)
        propertyList_.push_back(QString::fromStdString(mdsProps.at(i)->name()));

    // Connect to SQL Database
    connectToDatabase();
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

QString param_base_class::queryDatabase(QString parameter, QString property)
{
    // Connect to parameters database
    QSqlQuery query(db_);

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

/**
 *  getParameterList(QString algoName)
 *  Function that returns all the parameters specified by the algorithm in algo
 *  name and puts into parameterList_
 *
 *  @param algoName QString - the name of the algorithm we are looking up
 *  @return bool
 *
 */
bool param_base_class::getParameterList(QString selectedProperty)
{
    QString usedAlgorithm =
            mdsObject_->readPropertyAlgorithm(selectedProperty);

    QStringList *completeParameterList =
            mdsObject_->getParameterNames(usedAlgorithm);

    displayParameterModel = new parameterModel();

    parameterList_.clear();

    for (int i = 0; i < completeParameterList->size(); ++i)
    {

        QString parameterValue = queryDatabase(completeParameterList->at(i),
                                               selectedProperty);

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

    return true;
}

bool param_base_class::isCastable(QString valueStr, float &valueFlt)
{
    bool ok;
    valueFlt = valueStr.toFloat(&ok);
    return ok;
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

}


