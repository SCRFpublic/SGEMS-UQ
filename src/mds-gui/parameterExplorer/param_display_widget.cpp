#include "param_display_widget.h"
#include "ui_param_display_widget.h"

#include "mdsparametertablemodel.h"

param_display_widget::param_display_widget(QWidget *parent,Chart_mdi_area *mdi,
                                           QString mdsSpaceName) :
    QWidget(parent), param_base_class(mdi,mdsSpaceName),
    ui(new Ui::param_display_widget)
{
    ui->setupUi(this);

    // Set the list
    for (unsigned int i = 0; i < propertyList_.size(); ++i)
    {
        ui->propertyListView->addItem(propertyList_.at(i));
    }

    connect(ui->propertyListView,SIGNAL(itemSelectionChanged()),
            this,SLOT(updateDisplay()));

}

param_display_widget::~param_display_widget()
{
    delete ui;
}

bool param_display_widget::update()
{
    QList<QListWidgetItem*> selProp =
            this->ui->propertyListView->selectedItems();

    // Make sure something is selected
    if (selProp.size()>0)
    {
        getParameterList(selProp.at(0)->data(Qt::DisplayRole).toString());

        this->ui->parametersTreeScrollArea->setWidget(displayParameterModel);
    }
}
