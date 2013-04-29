#include "param_search_widget.h"
#include "ui_param_search_widget.h"

#include "mdsparametertablemodel.h"

#include "param_plot_window.h"
#include <QStandardItemModel>
#include <QItemSelectionModel>

param_search_widget::param_search_widget(QWidget *parent,Chart_mdi_area *mdi,
                                         QString mdsSpaceName) :
    QWidget(parent), param_base_class(mdi,mdsSpaceName),
    ui(new Ui::param_search_widget)
{
    ui->setupUi(this);

    // Add all properties to comboBoxProperty
    for (unsigned int i = 0; i < propertyList_.size(); ++i)
    {
        ui->comboBoxProperty->addItem(propertyList_.at(i));
    }


    // Connection between selecting a property and displaying its parameters
    connect(ui->comboBoxProperty,SIGNAL(currentIndexChanged(QString)),
            this,SLOT(updateParameterList(QString)));

    connect(ui->pushButtonSearch,SIGNAL(clicked()), this,
            SLOT(search()));

    connect(ui->pushButtonPlot, SIGNAL(clicked()),this,SLOT(plot()));

    updateParameterList(ui->comboBoxProperty->currentText());

}

param_search_widget::~param_search_widget()
{
    delete ui;
}
void param_search_widget::updateParameterList(QString selectProp)
{
    getParameterList(selectProp);
    ui->scrollAreaParam->setWidget(displayParameterModel);
    displayParameterModel->hideColumn(1);

    // Connection between selecting a parameter and displaying its values
    connect(this->displayParameterModel,SIGNAL(itemSelectionChanged()),
            this,SLOT(updateSelectedParameter()));
}

void param_search_widget::updateSelectedParameter()
{
    // Grab list of selected items in the displayParameterModel
    QList<QTreeWidgetItem *> selectedItems =
            displayParameterModel->selectedItems();

    // We will use only the first element
    if (selectedItems.size() > 0)
    {
        parameterNode* currentNode = static_cast<parameterNode*>
                (selectedItems.at(0));

        currentSearchParam_ = flattenPath(currentNode);

        // Only want to display last 2
        QString truncPath;

        if (parameterList_.contains(currentSearchParam_))
        {
            geoStatParam param = parameterList_[currentSearchParam_];
            ui->lineEditParam->setText(currentSearchParam_);

            QString dataTypeStr;

            switch(param.type)
            {
            case -1:
                dataTypeStr = QString("String");
                break;
            case 0:
                dataTypeStr = QString("Float");
                break;
            case 1:
                dataTypeStr = QString("Integer");
                break;
            case 2:
                dataTypeStr = QString("Double");
                break;
            case 3:
                dataTypeStr = QString("Unknown");
                break;
            default:
                dataTypeStr = QString("String");

            }

            ui->lineEditType->setText(dataTypeStr);

            QString castable = (param.type > 0) ?
                        (QString("True")) : (QString("False"));

            ui->lineEditCastable->setText(castable);
            ui->lineEditValue->setText(param.str);

            if (param.type > 0)
            {
                ui->lineEditSearchValue->setDisabled(false);
                ui->lineEditSearchValue->setText(param.str);
            }
            else
                ui->lineEditSearchValue->setDisabled(true);
        }
        else
        {
            ui->lineEditParam->setText(currentSearchParam_);
            ui->lineEditType->setText(QString("Category"));
            ui->lineEditCastable->setText("False");
            ui->lineEditValue->setText("N/A");
            ui->lineEditSearchValue->setDisabled(true);

        }
    }
}

bool param_search_widget::update()
{
    // Find search type
    // Hard code for now just use +/=

    // Map QString to boolean operator
    // Generate a vector with all properties values
    std::vector<float> parameterValues;

    for (unsigned int i = 0; i < propertyList_.size(); ++i)
        parameterValues.push_back(queryDatabase(currentSearchParam_,
                                                propertyList_.at(i)).toFloat());

    std::vector<bool> searchResults;

    if (ui->comboBoxSearch->currentText() == QString("<"))
        searchResults =
                numericalCompare(parameterValues,
                                 ui->lineEditSearchValue->text().toFloat(),
                                 std::less<float>());
    else if (ui->comboBoxSearch->currentText() == QString(">"))
        searchResults =
                numericalCompare(parameterValues,
                                 ui->lineEditSearchValue->text().toFloat(),
                                 std::greater<float>());
    else if (ui->comboBoxSearch->currentText() == QString(">="))
        searchResults =
                numericalCompare(parameterValues,
                                 ui->lineEditSearchValue->text().toFloat(),
                                 std::greater_equal<float>());
    else if (ui->comboBoxSearch->currentText() == QString("<="))
        searchResults =
                numericalCompare(parameterValues,
                                 ui->lineEditSearchValue->text().toFloat(),
                                 std::less_equal<float>());
    else if (ui->comboBoxSearch->currentText() == QString("=="))
        searchResults =
                numericalCompare(parameterValues,
                                 ui->lineEditSearchValue->text().toFloat(),
                                 std::equal_to<float>());
    else if (ui->comboBoxSearch->currentText() == QString("!="))
        searchResults =
                numericalCompare(parameterValues,
                                 ui->lineEditSearchValue->text().toFloat(),
                                 std::not_equal_to<float>());

    // Generate results table view
    model = new QStandardItemModel(searchResults.size(),2,this);
    model->setHeaderData(0, Qt::Horizontal, tr("Realization"));
    model->setHeaderData(1, Qt::Horizontal, tr("Param Value"));


    int numFound = 0;

    for (unsigned int i = 0; i < searchResults.size(); ++i)
    {
        if (searchResults.at(i))
        {
            model->setData(model->index(numFound,0,QModelIndex()),
                           propertyList_.at(i));
            model->setData(model->index(numFound,1,QModelIndex()),
                           parameterValues.at(i));

            numFound++;
        }
    }

    ui->tableViewResults->setModel(model);
    ui->tableViewResults->horizontalHeader()->setStretchLastSection(true);

}

void param_search_widget::plot()
{
    // First we need to build the two Lists of property and parameters we
    // will use to plot
    QModelIndexList selectedModels =
            ui->tableViewResults->selectionModel()->selectedIndexes();

    plotParameters_.clear(); plotValues_.clear();

    std::vector<int> cont;

    for (unsigned int i = 0; i < selectedModels.size(); ++i)
    {
        cont.push_back(selectedModels.at(i).row());
    }
    std::sort(cont.begin(), cont.end());
    cont.erase(std::unique(cont.begin(), cont.end()), cont.end());

    for (unsigned int i = 0; i < cont.size(); ++i)
    {
        plotParameters_.append(model->data(
                                   model->index(cont.at(i),0,
                                                QModelIndex())).toString());

        plotValues_.push_back(model->data(
                                  model->index(cont.at(i),1,
                                               QModelIndex())).toFloat());
    }

    param_plot_window * subWindow = new param_plot_window(this);

    subWindow->plot(plotParameters_,plotValues_,currentSearchParam_);
    mdiParent_->addSubWindow(subWindow);
    subWindow->show();
}
