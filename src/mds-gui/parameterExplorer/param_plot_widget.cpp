#include "param_plot_widget.h"
#include "ui_param_plot_widget.h"
#include "param_plot_window.h"

param_plot_widget::param_plot_widget(QWidget *parent, Chart_mdi_area *mdi,
                                     QString mdsSpaceName) :
    QWidget(parent), param_base_class(mdi,mdsSpaceName),
    ui(new Ui::param_plot_widget)
{
    ui->setupUi(this);

    // Place list of available realizations into the list widget
    realizationsWidget.addItems(propertyList_);

    // Allow for multiple items to be selected within the listwidget
    ui->realizationScrollArea->setWidget(&realizationsWidget);

    // Update the parameter list every time the selection of realizations is
    // changed
    connect(&realizationsWidget,SIGNAL(itemSelectionChanged()),
            this,SLOT(updateParameterScrollArea()));

    connect(ui->plotPushButton,SIGNAL(clicked()),
            this,SLOT(plotParameterValues()));

}

param_plot_widget::~param_plot_widget()
{
    delete ui;
}

void param_plot_widget::updateParameterScrollArea()
{
    ui->parametersScrollArea->setWidget(findCommonParameters());
}

void param_plot_widget::plotParameterValues()
{
    createParameterPlotWindow();
}
bool param_plot_widget::update()
{
  return true;
}
