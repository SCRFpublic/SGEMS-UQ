#include "eda_mds_space.h"
#include "chart_mds.h"
#include <metric_utilities.h>
#include <multi_dim_scaling_space.h>

EDA_mds_space::EDA_mds_space(Chart_mdi_area* mdi_area, QWidget *parent) :
    Chart_creator(mdi_area, parent)
{

    QVBoxLayout* main_layout = new QVBoxLayout(this);
    main_layout->addWidget(new QLabel("Available Grids",this));
    mdsListTreeView_ = new QTreeView(this);
    main_layout->addWidget(mdsListTreeView_);
    
    main_layout->addWidget(new QLabel("Available Responses",this));    
    responseListTreeView_ = new QTreeView(this);
    main_layout->addWidget(responseListTreeView_);

    main_layout->addWidget(new QLabel("Available Properties",this));
    propertyListTreeView_  = new QTreeView(this);
    main_layout->addWidget(propertyListTreeView_);

    QHBoxLayout* button_layout = new QHBoxLayout(this);
    QPushButton* create_button = new QPushButton("Create",this);
    QPushButton* delete_button = new QPushButton("Delete",this);
    button_layout->addWidget(create_button);
    button_layout->addWidget(delete_button);
    QWidget* button_holder = new QWidget(this);
    button_holder->setLayout(button_layout);
    main_layout->addWidget(button_holder);
    
    QPushButton* display_button = new QPushButton("Display",this);
    main_layout->addWidget(display_button);

    this->setLayout(main_layout);
    this->setMDSTreeView();

    // Create an empty model that we will use as a placeholder
    this->emptyModel = new QStandardItemModel(this);

    // Connect signals for creating/deleting MDS Spaces
    connect(create_button,SIGNAL(clicked()),
            this,SLOT(createNewMDSSpace()));
    connect(delete_button,SIGNAL(clicked()),
            this,SLOT(deleteMDSSpace()));
    connect(display_button,SIGNAL(clicked()),
            this,SLOT(display_mds_space()));
    connect(mdsListTreeView_,SIGNAL(clicked(QModelIndex)),
            this,SLOT(updatePropertyExplorer(QModelIndex)));
    connect(mdsListTreeView_,SIGNAL(clicked(QModelIndex)),
            this,SLOT(updateResponseExplorer(QModelIndex)));
}

EDA_mds_space::~EDA_mds_space()
{
}

void EDA_mds_space::createNewMDSSpace()
{
    MDSSpaceCreatorDialog* mdsSpaceCreatorDialog =
            new MDSSpaceCreatorDialog(proj_,this);
    mdsSpaceCreatorDialog->show();

    //  connect(mdsSpaceCreatorDialog,SIGNAL(updateSpaceList()),
    //          this,SLOT(updateModel()));
}

void EDA_mds_space::deleteMDSSpace()
{
    // Find the currently selected MDS Space
    QModelIndex indexToDelete = mdsListTreeView_->currentIndex();


    QString deleteSpaceName = this->proxy_model->data(indexToDelete,
                                                      Qt::DisplayRole).toString();

    deleteSpaceName.prepend("/");

    // Remove space from root manager
    Root::instance()->delete_interface(mds_manager +
                                       deleteSpaceName.toStdString());

    propertyListTreeView_->setModel(this->emptyModel);
    responseListTreeView_->setModel(this->emptyModel);
}

void EDA_mds_space::setMDSTreeView()
{
    this->proxy_model = new Filter_root_proxy_model(this);
    mdsListTreeView_->setModel(proxy_model);
    //this->mdsBaseIndex = proxy_model->mds_root_index();
    this->mdsBaseIndex =
            proxy_model->manager_index(QString::fromStdString(mds_manager));
    mdsListTreeView_->setRootIndex(this->mdsBaseIndex);
    mdsListTreeView_->setItemsExpandable(false);

    // Allow for objects to be dragged from list view into MDI area
    mdsListTreeView_->setDragEnabled(true);
    mdsListTreeView_->setDragDropMode(QAbstractItemView::DragOnly);

}


void EDA_mds_space::updatePropertyExplorer(QModelIndex mdsSpaceIndex)
{
    // Create a sort filter
    QString spaceName = this->proxy_model->data(mdsSpaceIndex,
                                                Qt::DisplayRole).toString();

    propertyListTreeView_->setModel(this->proxy_model);
    propertyListTreeView_->setRootIndex(
                ModelUtilities::mds_metric_properties_root_index(
                    this->proxy_model, spaceName));

}

void EDA_mds_space::updateResponseExplorer(QModelIndex mdsSpaceIndex)
{
    // Create a sort filter
    QString spaceName = this->proxy_model->data(mdsSpaceIndex,
                                                Qt::DisplayRole).toString();

    responseListTreeView_->setModel(this->proxy_model);
    responseListTreeView_->setRootIndex(
                ModelUtilities::mds_metric_response_root_index(
                    this->proxy_model, spaceName));

}


void EDA_mds_space::display_mds_space()
{

  QModelIndexList mds_indexes =
          mdsListTreeView_->selectionModel()->selectedIndexes();
  if(mds_indexes.isEmpty()) return;

  QString mds_object_name = mds_indexes.at(0).data().toString();

  SmartPtr<Named_interface> ni =
          Root::instance()->interface( mds_manager+"/"+
                                        mds_object_name.toStdString());

  MultiDimScalingSpace* mdsObject = dynamic_cast
          <MultiDimScalingSpace*>(ni.raw_ptr());

  if(mdsObject==0) return;

  std::cout << "Adding MDS Space for " << mds_object_name.toStdString() << std::endl;

  // Create QWidget Type Uncertaininty SubWindow
  Chart_mds *chart = new Chart_mds(mds_object_name, mdi_area_, this);


  
  QMdiSubWindow* sub_window = mdi_area_->addSubWindow(chart);
  sub_window->setAttribute( Qt::WA_DeleteOnClose );
  sub_window->show();

}
