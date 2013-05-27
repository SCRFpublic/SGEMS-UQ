#include "ui_param_sensitivity_widget.h"
#include "param_sensitivity_widget.h"
#include "param_plot_window.h"

param_sensitivity_widget::param_sensitivity_widget(QWidget *parent,
                                                   Chart_mdi_area *mdi,
                                                   QString mdsSpaceName) :
    QWidget(parent),param_base_class(mdi,mdsSpaceName),
    ui(new Ui::param_sensitivity_widget)
{
    ui->setupUi(this);

    // Load list of available realizations
    for (unsigned int i =0; i < propertyList_.size(); ++i)
        ui->realizationsListWidget->addItem(propertyList_.at(i));


    ui->realizationsListWidget->setSelectionMode(
                QAbstractItemView::MultiSelection);

    connect(ui->realizationsListWidget,SIGNAL(itemSelectionChanged()),
            this,SLOT(updateParameterList()));
    connect(ui->addPushButton,SIGNAL(clicked()),
            this,SLOT(addSelectedParameter()));
    connect(ui->plotPushButton,SIGNAL(clicked()),
            this,SLOT(plot()));

    updateResponseList();
}

param_sensitivity_widget::~param_sensitivity_widget()
{
    delete ui;
}

void param_sensitivity_widget::updateParameterList()
{
    // Get list of selected realizations
    QList<QListWidgetItem *> selectedRealizations =
            ui->realizationsListWidget->selectedItems();

    // Generate list of selected realization names
    QStringList selectedRealNames;

    for (unsigned int i = 0; i < selectedRealizations.size(); ++i)
    {
        QString realName =
                selectedRealizations.at(i)->data(Qt::DisplayRole).toString();

        selectedRealNames.append(realName);
    }

    // Find the shared parameter names
    QStringList sharedParameters = findSharedParameters(selectedRealNames);

    // Generate a model to store these parameters
    parameterModel *sharedParameterModel = new parameterModel(this);
    sharedModel = sharedParameterModel;

    for (unsigned int i = 0; i < sharedParameters.size(); ++i)
    {
        sharedParameterModel->addNode(sharedParameters.at(i),QString(),
                                      sharedParameterModel->root());
    }

    //sharedParameterModel->expandAll();
    sharedParameterModel->setColumnHidden(1,true);
    sharedParameterModel->resizeColumnToContents(0);
    sharedParameterModel->resizeColumnToContents(1);
    sharedParameterModel->show();

    ui->parameterScrollArea->setWidget(sharedParameterModel);
}

void param_sensitivity_widget::addSelectedParameter()
{
    // Find list of selected parameters and realizations
    QList<QListWidgetItem *> selectedRealizations =
            ui->realizationsListWidget->selectedItems();
    QList<QTreeWidgetItem *> selectedParameters =
            sharedModel->selectedItems();

    // Parse selected models into string list full path
    QStringList fullPaths, realizationList;

    for (int i = 0; i < selectedParameters.size(); ++i)
    {
        parameterNode* currentNode =
                static_cast<parameterNode*>(selectedParameters.at(i));

        QString result;

        result = flattenPath(currentNode);
        fullPaths.append(result);
    }

    // Remove duplicates
    fullPaths.removeDuplicates();

    for (unsigned int i = 0; i < fullPaths.size(); ++i)
    {
        QString fullPath = fullPaths.at(i);
        fullPath.replace("$","/");
        ui->selectedParamListWidget->addItem(fullPath);
    }
}

bool param_sensitivity_widget::update()
{
  return true;
}

void param_sensitivity_widget::plot()
{
    // Retrieve number of realizations
    QList<QListWidgetItem *> selectedRealizations =
            ui->realizationsListWidget->selectedItems();

    // Generate list of selected realization names
    QStringList selectedRealNames;

    for (unsigned int i = 0; i < selectedRealizations.size(); ++i)
    {
        QString realName =
                selectedRealizations.at(i)->data(Qt::DisplayRole).toString();

        selectedRealNames.append(realName);
    }

    int numRealizations = selectedRealNames.size();

    // Generate list of selected parameters
    int numParameters = ui->selectedParamListWidget->count();

    // Convert selected parameters in selectedParamListWidget to QStringList
    QStringList selectedParameterNames;
    for (unsigned int i = 0; i < numParameters; ++i)
    {
        QString paramName = ui->selectedParamListWidget->item(i)->text();
        paramName.replace("/","$");
        selectedParameterNames.push_back(paramName);
    }


    // Generate name of selected response
    QModelIndexList list = ui->responseTreeView->
            selectionModel()->selectedIndexes();

    QString responseName = ui->responseTreeView->model()->
            data(list.at(0)).toString();

    VectorXf responseVector = readResponseValues(selectedRealNames,
                                                 responseName);
    MatrixXf parameterMatrix = readParameterValues(selectedRealNames,
                                                   selectedParameterNames);

    // Compute the sensitivity coefficients
    VectorXf senVector = computeSensitivity(parameterMatrix,responseVector);

    QStringList sortedParamNames = sortSensitivity(senVector,
                                                   selectedParameterNames);

    // Clean up parameter names
    QStringList cleanParam = cleanParameterNames(sortedParamNames);


    param_plot_window * subWindow = new param_plot_window(this);
    subWindow->plot(cleanParam,senVector);
    mdiParent_->addSubWindow(subWindow);
    subWindow->show();

}

QStringList param_sensitivity_widget::cleanParameterNames(
        QStringList &parameterNames)
{
    QStringList cleanedParam;
    for (unsigned int i = 0; i < parameterNames.size(); i++)
    {
        QString param = parameterNames.at(i);

        param = param.replace("parameters$","");
        param = param.replace("$nonParamCdf$","");
        param = param.replace("Histogram$","");
        param = param.replace("$nonParamCdf_primary$","");
        param = param.replace("std","var");
        param = param.replace("facie","facies");
        param = param.replace("$","-");
        param = param.replace("permeability","perm");
        param = param.replace("porosity","poro");
        cleanedParam.push_back(param);
    }

    return cleanedParam;

}

VectorXf param_sensitivity_widget::computeSensitivity(
        MatrixXf &parameterMatrix, VectorXf &responseVector)
{
    MatrixXf Ctemp = parameterMatrix.transpose()*parameterMatrix;
    MatrixXf C;
    C = Ctemp.inverse();

    VectorXf b = C*parameterMatrix.transpose()*responseVector;

    VectorXf Y_hat = parameterMatrix*b;

    int p = b.rows();

    VectorXf sigma2Vec = responseVector-Y_hat;

    float sigma2 = sigma2Vec.squaredNorm();
    sigma2= sigma2/(parameterMatrix.rows() - p);

    Ctemp = C*sigma2;

    MatrixXf denominator = Ctemp.diagonal();

    // Do element-wise division
    VectorXf t = b;
    for (int i = 0; i < b.rows(); i++)
    {
        t(i) = abs(b(i)/sqrt(denominator(i)));
    }

    return t;
}

VectorXf param_sensitivity_widget::readResponseValues(
        QStringList realizations,QString response)
{

    // Sort the realizations stringlist
    realizations.sort();


    SmartPtr<Named_interface> ni =
            Root::instance()->interface( metricData_manager +
                                         "/metricRegistrar");

    MetricDataManager* mRegistrar =
            dynamic_cast<MetricDataManager*>(ni.raw_ptr());

    // Grab the grid name
    std::string gridName = mdsObject_->getGridName(0);

    // Obtain pointer to metricToProperties map
    MetricDataToProperties  propFromMetricRegistrar =
            mRegistrar->get_properties_for_metric(response.toStdString());

    // Determine what kind of metricData we are looking at
    std::string response_type = propFromMetricRegistrar.
            data_type().toStdString();

    // Response values
    VectorXf responseValue(realizations.size());

    if (response_type == "MetricTimeSeriesData")
    {

        for (unsigned int i = 0; i < realizations.size(); ++i )
        {
            MetricData* mdata = mRegistrar->getMetricData(gridName,
                                                          realizations.at(i).
                                                          toStdString(),
                                                          response.toStdString());

            // Should be a safe cast as this function is only called for data that
            // has form "TimeSeries"
            MetricTimeSeriesData *mDataTS =
                    static_cast<MetricTimeSeriesData*>(mdata);

            // Get number of data points in this response for particular property
            // We assume that the data/time have same number of points
            int numDataPoints = mDataTS->data().size();

            responseValue[i] = (mDataTS->data().at(numDataPoints -1));
        }
    }

    return responseValue;
}

MatrixXf param_sensitivity_widget::readParameterValues(QStringList realizations,
                                                       QStringList parameters)
{
    realizations.sort();
    parameters.sort();

    MatrixXf real_params_val(realizations.size(), parameters.size()+1);

    for (unsigned int i = 0; i < realizations.size(); ++i)
    {
        QString realizationName = realizations.at(i);
        real_params_val(i,0) = 1;

        for (unsigned j = 0; j < parameters.size(); ++j)
        {
            QString parameterName = parameters.at(j);
            QString parameterValueStr = queryDatabase(parameterName,
                                                      realizationName);

            float paramValueNum;

            if (!isCastable(parameterValueStr,paramValueNum))
                paramValueNum = 0.0f;

            // Write to matrix
            real_params_val(i,j+1) = paramValueNum;
        }
    }

    return real_params_val;
}

void param_sensitivity_widget::updateResponseList()
{

    this->proxy_model = new Filter_root_proxy_model(this);
    ui->responseTreeView->setModel(proxy_model);
    this->responseBaseIndex =
            ModelUtilities::response_root_index(this->proxy_model);
    ui->responseTreeView->setRootIndex(this->responseBaseIndex);
    ui->responseTreeView->setItemsExpandable(false);
}

QStringList param_sensitivity_widget::sortSensitivity(VectorXf &senVal,
                                                      QStringList &paramNames)
{
    // We need to sort senVal and use the corresponding indices to sort
    // paramNames

    // Step 1: Convert senVal to vector of pairs
    std::vector<std::pair<float,int> > sortedSenVals;
    std::pair<float,int> senValuePair;
    for (unsigned int i = 1; i < senVal.rows(); ++i)
    {
        senValuePair.first = senVal(i);
        senValuePair.second = i-1; // We are ignoring first element
        sortedSenVals.push_back(senValuePair);
    }

    // Step 2: Sort this vector
    std::sort(sortedSenVals.begin(), sortedSenVals.end());

    QStringList sortedParamNames;

    // Step 3: Rewrite the senVal and paramNames
    for (unsigned int i = 0; i < sortedSenVals.size(); ++i)
    {
        sortedParamNames.push_back(paramNames.at(sortedSenVals.at(i).second));
        senVal[i] = sortedSenVals.at(i).first;
    }

    return sortedParamNames;

}

