#include "param_commonparam_widget.h"
#include "ui_param_commonparam_widget.h"
#include "mdsparametertablemodel.h"

param_commonParam_widget::param_commonParam_widget(QWidget *parent,
                                                   Chart_mdi_area *mdi,
                                                   QString mdsSpaceName) :
    QWidget(parent), param_base_class(mdi,mdsSpaceName),
    ui(new Ui::param_commonParam_widget)
{
    ui->setupUi(this);

    realizationsWidget.addItems(propertyList_);
    ui->realizationsScrollArea->setWidget(&realizationsWidget);

    connect(ui->commonParamSearchButton,SIGNAL(clicked()),
            this,SLOT(findCommonParameters()));

}

param_commonParam_widget::~param_commonParam_widget()
{
    delete ui;
}

bool param_commonParam_widget::update()
{

}

void param_commonParam_widget::search()
{

}

void param_commonParam_widget::findCommonParameters()
{

    // This is a list of common parameters the models have, (category wise),
    // not necessarily value wise
    QModelIndexList selectedModels =
            realizationsWidget.selectionModel()->selectedIndexes();

    QStringList selectedRealizations;
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

        selectedRealizations.append(realizationName);
    }

    QStringList commonParams = findSharedParameters(selectedRealizations);

    // Check each shared parameter and find common parameters
    QStringList identicalParameterNames;
    for (int i = 0; i < commonParams.size(); ++i)
    {
        // This is a QStringList that stores the values for the current
        // parameter for each model
        QStringList parameterValue;

        for (int j = 0; j< selectedRealizations.size(); ++j)
        {
            parameterValue.append(this->queryDatabase(commonParams.at(i),
                                                      selectedRealizations.at(j)));
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


    QStringList values = this->queryValues(identicalParameterNames,
                                           selectedRealizations.at(0));

    parameterModel *resultsModel = new parameterModel(this);

    for (int i = 0; i < identicalParameterNames.size(); i++)
    {
        resultsModel->addNode(identicalParameterNames.at(i),values.at(i),resultsModel->root());
    }

    resultsModel->expandAll();
    resultsModel->resizeColumnToContents(0);
    resultsModel->resizeColumnToContents(1);
    resultsModel->setHeaderHidden(true);
    this->ui->resultsScrollArea->setWidget(resultsModel);
}
