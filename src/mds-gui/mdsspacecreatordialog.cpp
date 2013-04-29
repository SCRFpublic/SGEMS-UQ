#include <mdsspacecreatordialog.h>
#include <ui_mdsspacecreatordialog.h>
#include <QMessageBox>

MDSSpaceCreatorDialog::MDSSpaceCreatorDialog(GsTL_project* proj,
                                             QWidget* parent):
    QDialog(parent),proj_(proj), ui(new Ui::MDSSpaceCreatorDialog)
{
    // Create Dialog for generating new MDS spaces
    ui->setupUi(this);

    // Declare object for loading existing properties
    this->gridSelector_ = new GridSelectorBasic(this, "Grid", proj_);
    this->mPropSelector_ = new OrderedPropertySelector( this, "Properties" );
    this->setWindowTitle(QString("MDS Space Creator"));

    // Declar object used to select metric
    this->metricSelector_ = new MultipleMetricDataSelector(this);

    // Add to UI basewidget
    this->ui->propertiesLayout->addWidget(this->gridSelector_);
    this->ui->propertiesLayout->addWidget(this->mPropSelector_);
    this->ui->metricLayout->addWidget(this->metricSelector_);

    // This is a window that opens and displays possible kernels for the
    // selected sets of data
    kernelBox_ = new QGroupBox(this);
    QVBoxLayout* distanceLayout = new QVBoxLayout(this);
    distanceLayout->addWidget(new QLabel("", this));
    kernelBox_->setLayout(distanceLayout);
    this->ui->kernelLayout->addWidget(kernelBox_);

    connect(gridSelector_, SIGNAL(activated(const QString&)),
            mPropSelector_, SLOT(show_properties(const QString&)) );

    connect(gridSelector_, SIGNAL(activated(const QString&)),
            metricSelector_, SLOT(setSelectedGrid(const QString&)) );

    connect(mPropSelector_, SIGNAL(forwardSelectedProperties(const
                                                             QStringList&)),
            metricSelector_, SLOT(setMetricsList(const QStringList&)) );

    connect(metricSelector_, SIGNAL(selectionApproved(const QStringList& )),
            this, SLOT(showMetricKernelChooser(const QStringList&)));

    connect(metricSelector_, SIGNAL(selectionApproved(QStringList)),this,
            SLOT(updatedMetricSelection(QStringList)));

    connect(this->ui->generateButton, SIGNAL(clicked()),
            this,SLOT(generateXML()));
    connect(this->ui->cancelButton,SIGNAL(clicked()),
            this,SLOT(close()));
}

MDSSpaceCreatorDialog::~MDSSpaceCreatorDialog()
{
    delete ui;
}

void MDSSpaceCreatorDialog::showMetricKernelChooser(const QStringList&
                                                    metricNames)
{
    // Metrics to be removed from the list
    QList<QString> keys = kernelHash_.keys();
    for(int i=0; i<keys.size(); ++i) {
        if( metricNames.contains(keys.at(i))  ) continue;
        MetricDistanceChooser* kernelToBeRemoved = kernelHash_.value(keys.at(i));
        kernelBox_->layout()->removeWidget(kernelToBeRemoved);
        kernelHash_.remove(keys.at(i));
        delete kernelToBeRemoved;
    }

    for(int i=0; i<metricNames.size(); ++i)
    {
        if( kernelHash_.contains(metricNames.at(i)) ) continue;
        MetricDistanceChooser* kernelChooser =
                new MetricDistanceChooser(metricNames.at(i), this);
        kernelBox_->layout()->addWidget(kernelChooser);
        kernelHash_.insert(metricNames.at(i), kernelChooser);
        this->currentKernel = kernelChooser->getSelectedKernel();
    }
}

void MDSSpaceCreatorDialog::generateSetParameters()
{
    // Generate new XML Document
    QDomDocument *configurationParameter = new QDomDocument("MDSParameters");

    // Create Root Element
    QDomElement root = configurationParameter->createElement("MDS");
    configurationParameter->appendChild(root);

    // Write filename
    QDomElement spaceName = configurationParameter->createElement("Spacename");
    spaceName.setAttribute("name",this->ui->gridNameLineEdit->text());


    // Write Kernel Type
    QDomElement kernelType = configurationParameter->createElement("Kernel");
    kernelType.setAttribute("type", this->currentKernel);

    // Write Grid Name
    QDomElement grid = configurationParameter->createElement("Grid");
    grid.setAttribute("name", this->gridSelector_->currentText().
                      toStdString().c_str());

    // Write Metric Tag
    QDomElement metric = configurationParameter->createElement("MetricData");

    // Write grid elements
    for (int i = 0;
         i < this->mPropSelector_->selected_properties().size(); i++)
    {
        QDomElement prop = configurationParameter->createElement("Property");
        prop.setAttribute("name",QString(this->mPropSelector_->
                                         selected_properties().at(i).toAscii()));
        grid.appendChild(prop);
    }

    // Write metric
    for (int i = 0; i < this->currentMetricList.size(); i++)
    {
        QDomElement met = configurationParameter->createElement("Metric");
        met.setAttribute("name",QString(this->currentMetricList.at(i).toAscii()));
        metric.appendChild(met);
    }

    // Put under root
    root.appendChild(kernelType);
    root.appendChild(grid);
    root.appendChild(metric);
    root.appendChild(spaceName);

    // Register MDS_actions
    SmartPtr<Named_interface> ni =
            Root::instance()->new_interface("MDS",actions_manager+"/");
    MDS_action* create_mds_object = dynamic_cast<MDS_action*> (ni.raw_ptr());

    if( create_mds_object == 0 ) return;

    Error_messages_handler temp;

    create_mds_object->initFromDom(*configurationParameter,&*this->proj_,&temp);

    this->updateSpaceList();

    this->window()->close();
}

void MDSSpaceCreatorDialog::generateXML()
{
    // Get list of existing MDS Space names
    Filter_root_proxy_model * proxy_model = new Filter_root_proxy_model(this);
    QModelIndex mdsBaseIndex = proxy_model->manager_index(
                QString::fromStdString(mds_manager));

    int numSpaces = proxy_model->rowCount(mdsBaseIndex);

    for (int i = 0; i < numSpaces; i++)
    {
        QModelIndex index = proxy_model->index(i,0, mdsBaseIndex);
        QString spaceName = proxy_model->data(index).toString();

        if (spaceName==ui->gridNameLineEdit->text())
        {
            QMessageBox msgBox;
             msgBox.setWindowTitle("Error!");
             msgBox.setText("Name Already Exists");
             msgBox.setDetailedText("A MDS Space with the same name already"
                                    " exists. Please choose another");
             msgBox.exec();

            return;
        }
    }
    generateSetParameters();
}

void MDSSpaceCreatorDialog::updatedMetricSelection(QStringList latestMetric)
{
    this->currentMetricList = latestMetric;
}
