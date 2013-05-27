#include "metric_explorer.h"

//#include "MetricFilterTreeWidget.h"

#include "parametertreeview.h"

#include <gui/appli/qt_algo_control_panel.h>

#include <Qt>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStringList>
#include <QLabel>
#include <QGroupBox>
#include <QFileDialog>

#include <QTreeView>
#include <QtDebug>

MetricExplorer::MetricExplorer(GsTL_project* proj, QWidget *parent) :
    QFrame(parent), proj_(proj)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    this->setLayout(mainLayout);

    QSplitter* mainSplitter = new QSplitter(Qt::Horizontal, this);
    this->layout()->addWidget(mainSplitter);

    // Left is for the visualization
    // Right is to create new data


    // Left of the main splitter
    QSplitter* leftSplitter = new QSplitter(Qt::Vertical, this);

    metricsView_ = new QTreeView(this);
    metricsView_->setContextMenuPolicy(Qt::ActionsContextMenu);
    leftSplitter->addWidget(metricsView_);
    ParameterTreeView* paramView = new ParameterTreeView(this);
    leftSplitter->addWidget(paramView);
    mainSplitter->addWidget(leftSplitter);

    // Right:

    QGroupBox * algoBox = new QGroupBox(this);
    QVBoxLayout* algoLayout = new QVBoxLayout(this);
    algoLayout->addWidget(new QLabel("Create New Data Metric", this));
    Algo_control_panel* algo_panel = new Algo_control_panel(proj_,this);
    algo_panel->filter_algo_by_category("Data");
    algoLayout->addWidget(algo_panel);
    algoLayout->addStretch(1);
    algoBox->setLayout(algoLayout);
    mainSplitter->addWidget(algoBox);


    connect(metricsView_,SIGNAL(activated( const QModelIndex &  )),
            paramView, SLOT( showParameters(const QModelIndex& ) ) );

    connect(metricsView_,SIGNAL(activated( const QModelIndex &  )),
            paramView, SLOT( showParameters(const QModelIndex& ) ) );

    QAction* saveAct = new QAction(QIcon(":/images/save.png"),
                                   tr("&Save to file"), this);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(SaveToFileClicked()));
    metricsView_->addAction(saveAct);
}

MetricExplorer::~MetricExplorer()
{
}

void MetricExplorer::SaveToFileClicked()
{
    // get current selected item
    QModelIndex selectedItem = metricsView_->currentIndex();
    if (selectedItem.isValid()) {

        QString initialPath = QDir::currentPath() + "/MetricDataSave.xml";

        QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                        initialPath,
                                                        tr("XML Files (*.xml);;All Files (*)"));

        if (fileName.isEmpty())
            return;

        emit SaveToFile(selectedItem, fileName);
    }
}


void MetricExplorer::setTopLevelMetricModel(QAbstractItemModel* model){
    metricsView_->setModel(model);
    // connect save action to model
    connect(this, SIGNAL(SaveToFile(const QModelIndex&, const QString&)),
            model, SLOT(SaveToFile(const QModelIndex&, const QString&)));

}
