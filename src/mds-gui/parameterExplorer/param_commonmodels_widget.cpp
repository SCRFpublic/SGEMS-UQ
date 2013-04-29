#include "param_commonmodels_widget.h"
#include "ui_param_commonmodels_widget.h"
#include "mdsparametertablemodel.h"

param_commonModels_widget::param_commonModels_widget(QWidget *parent,
                                                     Chart_mdi_area *mdi,
                                                     QString mdsSpaceName) :
    QWidget(parent), param_base_class(mdi,mdsSpaceName),
    ui(new Ui::param_commonModels_widget)
{
    ui->setupUi(this);

    realizationsWidget.addItems(propertyList_);
    ui->realizationsScrollArea->setWidget(&realizationsWidget);
    realizationsWidget.setSelectionMode(QAbstractItemView::SingleSelection);
    connect(&realizationsWidget,SIGNAL(itemSelectionChanged()),
            this,SLOT(updateParameterList()));
    connect(ui->findCommonModelButton,SIGNAL(clicked()),this,
            SLOT(search()));
}

param_commonModels_widget::~param_commonModels_widget()
{
    delete ui;
}

void param_commonModels_widget::updateParameterList()
{
    QList<QListWidgetItem*> selProp =
            realizationsWidget.selectedItems();

    // Make sure something is selected
    if (selProp.size()>0)
    {
        getParameterList(selProp.at(0)->data(Qt::DisplayRole).toString());
        this->ui->parametersTreeScrollArea->setWidget(displayParameterModel);
    }
}

void param_commonModels_widget::search()
{
    // Get selected models
    QList<QTreeWidgetItem *> selectedItems =
            this->displayParameterModel->selectedItems();

    // Parse selected models into stringlist with full path
    QStringList *fullPaths = new QStringList;
    for (int i = 0; i < selectedItems.size(); i++)
    {
        fullPaths->append(flattenPath(static_cast<parameterNode*>
                                      (selectedItems.at(i))));
    }

    // Remove duplicates
    fullPaths->removeDuplicates();

    // run findCommon models
    QString modelName = this->realizationsWidget.selectedItems().at(0)->
            data(Qt::DisplayRole).toString();

    QStringList commonModels = this->findCommonModels(
                modelName,*fullPaths);

    QStringListModel *resultsModel = new QStringListModel(commonModels);
    this->ui->commonModelsResultsListView->setModel(resultsModel);


}

QStringList param_commonModels_widget::findCommonModels(QString &model,
                                                        QStringList &parameterList)
{
    // First find out the values for each parameterList for inputed model
    QString algorithmName = mdsObject_->readPropertyAlgorithm(model);
    QStringList values = this->queryValues(parameterList,model);

    QStringList modelList;
    modelList.append(model);

    //emit(this->highlightCommonModels(modelList));

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

bool param_commonModels_widget::update()
{

}
