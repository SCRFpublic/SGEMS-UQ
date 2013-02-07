/*
 * MdsObjectMdiSubWindow.cpp
 *
 *  Created on: May 1, 2011
 *      Author: aboucher
 */

#include "MdsObjectMdiSubWindow.h"

#include <metric_manager_repository.h>
#include <MetricData.h>
#include <MetricDataManager.h>
#include <GsTL/utils/smartptr.h>
#include <GsTLAppli/utils/manager.h>
#include <GsTLAppli/appli/manager_repository.h>

#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkAxesActor.h>
#include <vtkFloatArray.h>
#include <vtkDataObjectToTable.h>
#include <vtkRendererCollection.h>
#include <vtkStringArray.h>
#include <vtkPointSetToLabelHierarchy.h>
#include <vtkLabelPlacementMapper.h>
#include <vtkQtLabelRenderStrategy.h>
#include <vtkActor2D.h>
#include <vtkTextProperty.h>
#include <vtkQtLabelRenderStrategy.h>
#include <vtkVariant.h>

#include <QFrame>
#include <QToolBox>
#include <QListWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QSplitter>

#define VTK_CREATE(type, name) \
    vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>

// This class is what renders the view using VTK.
MdsObjectMdiSubWindow::MdsObjectMdiSubWindow(QString mdsObjectName, QWidget* parent)
    :QMdiSubWindow(parent), mdsObjectName_(mdsObjectName)
{

    this->setWindowTitle("Uncertainity Visualization For : "+mdsObjectName);
    this->setWindowIcon(QIcon(":/icons/MdsSpace.png"));
    this->mdsObjectName_ = mdsObjectName;


    SmartPtr<Named_interface> ni =
            Root::instance()->interface( mds_manager+"/"+
                                         mdsObjectName.toStdString());

    mdsObject_ = dynamic_cast<MultiDimScalingSpace*>(ni.raw_ptr());

    nPoints_ = mdsObject_->pointCount();
    selectedPairPoints_.first = 0;
    selectedPairPoints_.second = 0;

    QSplitter* splitter = new QSplitter(this);

    // This is the window which shows the visualiation
    qvtkWidget_ = new QVTKWidget(this);
    qvtkWidget_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    qvtkWidget_->setMinimumWidth(80);

    // Prepare the Rendering Widget
    this->setUpVtkRenderingWidget();

    QToolBox* toolbox = new QToolBox(this);
    tableView_ = vtkSmartPointer<vtkQtTableView>::New();
    tableView_->GetWidget()->setSizePolicy(QSizePolicy::Minimum,
                                           QSizePolicy::Expanding);

    // Read the data from the
    QTableWidget* propTable = new QTableWidget(this);
    propTable->setSelectionMode ( QAbstractItemView::MultiSelection );
    propTable->setSelectionBehavior(QAbstractItemView::SelectRows );
    propTable->setColumnCount(4);
    propTable->setRowCount(nPoints_);
    propTable->setHorizontalHeaderItem ( 0, new QTableWidgetItem(tr("Property") ));
    propTable->setHorizontalHeaderItem ( 1, new QTableWidgetItem(tr("Dim 1") ) );
    propTable->setHorizontalHeaderItem ( 2, new QTableWidgetItem(tr("Dim 2") ) );
    propTable->setHorizontalHeaderItem ( 3, new QTableWidgetItem(tr("Dim 3") ) );
    for(int i=0; i < nPoints_; i++) {
        propTable->setItem(i, 0, new QTableWidgetItem(tr("%1").arg( mdsObject_->getPropertyName(i).c_str()  )));
        propTable->setItem(i, 1, new QTableWidgetItem(tr("%1").arg( table_->GetValue(i, 1).ToFloat())));
        propTable->setItem(i, 2, new QTableWidgetItem(tr("%1").arg( table_->GetValue(i, 2).ToFloat())));
        propTable->setItem(i, 3, new QTableWidgetItem(tr("%1").arg( table_->GetValue(i, 3).ToFloat())));
    }

    QListWidget* metricList = new QListWidget(this);
    QStringList metricNameList;
    std::vector<std::string> metricNames = mdsObject_->getMetricNames();
    for(int i=0; i< metricNames.size(); ++i) {
        metricNameList.append(metricNames[i].c_str());
    }
    metricNameList.sort();
    metricList->addItems(metricNameList);

    clusterList_ = new QListWidget(this);
    QStringList clusterNameList;
    std::vector<MultiDimScalingSpace::Cluster> clusterObjects = mdsObject_->getClusters();
    for(int i=0; i< clusterObjects.size(); ++i) {
        clusterNameList.append(clusterObjects[i].name.c_str());
    }
    clusterNameList.sort();
    clusterList_->addItems(clusterNameList);

    QLabel *label_clusters = new QLabel("Number of clusters", this);
    num_clusters_spinbox_ = new QSpinBox(this);
    num_clusters_spinbox_->setMinimum(2);
    num_clusters_spinbox_->setMaximum(200);
    num_clusters_spinbox_->setValue(5);

    QPushButton *create_clusters_button = new QPushButton("&K-means clustering", this);

    QVBoxLayout* cluster_layout = new QVBoxLayout(this);
    QHBoxLayout* cluster_controls_layout = new QHBoxLayout(this);

    cluster_controls_layout->addWidget(label_clusters);
    cluster_controls_layout->addWidget(num_clusters_spinbox_);
    cluster_controls_layout->addWidget(create_clusters_button);

    QGroupBox* clusterControlsBox = new QGroupBox(this);
    clusterControlsBox->setLayout(cluster_controls_layout);

    cluster_layout->addWidget(clusterControlsBox);
    cluster_layout->addWidget(clusterList_);

    QGroupBox* clusterBox = new QGroupBox(this);
    clusterBox->setLayout(cluster_layout);

    QTableWidget* distanceTable = new QTableWidget(this);
    distanceTable->setColumnCount(nPoints_);
    distanceTable->setRowCount(nPoints_);
    for(int i=0; i< nPoints_; ++i) {
        QTableWidgetItem *itemHeader = new QTableWidgetItem(tr("%1").arg( mdsObject_->getPropertyName(i).c_str() ));
        distanceTable->setHorizontalHeaderItem ( i, itemHeader );
        distanceTable->setVerticalHeaderItem ( i, itemHeader );
        for(int j=i; j< nPoints_; ++j) {
            float dMetrics = mdsObject_->getDistanceMetrics(i,j);
            //	float dMds = mdsObject_->getDistanceMdsSpace(i,j);
            QTableWidgetItem *item = new QTableWidgetItem(tr("%1").arg( dMetrics));
            distanceTable->setItem(i, j, item);
            if(i!=j) distanceTable->setItem(j, i, new QTableWidgetItem(*item));

        }
    }
    toolbox->addItem(propTable, "Points Coordinates");
    toolbox->addItem(metricList, "Selected Metrics");
    toolbox->addItem(clusterBox, "Clustering");
    toolbox->addItem(distanceTable, "Distance Matrix");

    splitter->addWidget(qvtkWidget_);
    splitter->addWidget(toolbox);

    this->layout()->addWidget(splitter);

    connect( toolbox, SIGNAL(currentChanged(int)),
             this, SLOT(toolboxIndexChanged(int)) );

    connect( propTable, SIGNAL(itemClicked(QTableWidgetItem*)),
             this, SLOT(highlightSelectedPoint(QTableWidgetItem*)) );

    connect( distanceTable, SIGNAL(itemClicked(QTableWidgetItem*)),
             this, SLOT(highlightSelectedPairPoints(QTableWidgetItem*)) );

    connect(create_clusters_button, SIGNAL(clicked()),
            this, SLOT( create_clusters_button_clicked() ) );

    connect(clusterList_, SIGNAL(currentRowChanged(int)),
            this, SLOT(clusterList_selected(int)));

    //	ren_->ResetCamera();
    qvtkWidget_->show();
}

MdsObjectMdiSubWindow::~MdsObjectMdiSubWindow() {
    // TODO Auto-generated destructor stub
}

void MdsObjectMdiSubWindow::setUpVtkRenderingWidget(){

    table_ = vtkSmartPointer<vtkTable>::New();
    VTK_CREATE(vtkStringArray, labels);
    labels->SetName("Property");
    table_->AddColumn(labels);
    VTK_CREATE(vtkFloatArray, arrX);
    VTK_CREATE(vtkFloatArray, arrY);
    VTK_CREATE(vtkFloatArray, arrZ);

    // This stuff is just hard coded for the number of dimensions
    arrX->SetName("Dim 1");
    arrY->SetName("Dim 2");
    arrZ->SetName("Dim 3");
    table_->AddColumn(arrX);
    table_->AddColumn(arrY);
    table_->AddColumn(arrZ);

    // This table should have the same number of rows as the data we have read in
    table_->SetNumberOfRows(mdsObject_->pointCount());

    vtkData_ = vtkSmartPointer<vtkPolyData>::New();

    vtkSmartPointer<vtkPoints> mdsPts = vtkSmartPointer<vtkPoints>::New();
    mdsPts->SetDataTypeToFloat();
    mdsPts->SetNumberOfPoints( mdsObject_->pointCount() );

    for(int i=0; i<mdsObject_->pointCount(); i++)
    {
        // Read data from the object and set it into the table (row, col, data)
        table_->SetValue(i, 1,  mdsObject_->getXCoords(i));
        table_->SetValue(i, 2,  mdsObject_->getYCoords(i));
        table_->SetValue(i, 3,  mdsObject_->getZCoords(i));
        table_->SetValue(i, 0,  mdsObject_->getPropertyName(i).c_str());

        // Put it into the point form which is used for plotting later
        mdsPts->SetPoint(i, mdsObject_->getXCoords(i), mdsObject_->getYCoords(i), mdsObject_->getZCoords(i));
    }

    vtkSmartPointer<vtkCellArray> pts_connection = vtkSmartPointer<vtkCellArray>::New();
    for (int i = 0; i < mdsObject_->pointCount(); i++) {
        pts_connection->InsertNextCell(1);
        pts_connection->InsertCellPoint(i);
    }
    vtkData_->SetPoints(mdsPts);
    vtkData_->SetVerts(pts_connection);

    vtkData_->GetPointData()->AddArray(table_->GetColumn(0));

    VTK_CREATE(vtkPointSetToLabelHierarchy, hier);
    hier->SetInputData(vtkData_);
    hier->SetLabelArrayName("Property");
    hier->GetTextProperty()->SetColor(0.02, 0.02, 0.02);

    VTK_CREATE(vtkLabelPlacementMapper, lmapper);
    lmapper->SetInputConnection(hier->GetOutputPort());
    lmapper->SetShapeToRoundedRect();
    lmapper->SetBackgroundColor(1.0, 1.0, 0.7);
    lmapper->SetBackgroundOpacity(0.8);
    lmapper->SetMargin(2);

    VTK_CREATE(vtkQtLabelRenderStrategy, strategy);
    lmapper->SetRenderStrategy(strategy);

    VTK_CREATE(vtkActor2D, lactor);
    lactor->SetMapper(lmapper);

    vtkFloatArray* idArray = vtkFloatArray::New();
    idArray->SetNumberOfTuples(1);

    idArray->SetNumberOfValues(vtkData_->GetNumberOfPoints());
    for( int i = 0; i < vtkData_->GetNumberOfPoints(); i++ ) {
        idArray->SetValue(i,1);
    }
    vtkData_->GetCellData()->SetScalars(idArray);
    vtkData_->GetPointData()->SetScalars(idArray);



    // Mapper
    VTK_CREATE(vtkPolyDataMapper, mapper);
    mapper->ImmediateModeRenderingOn();
    mapper->SetInputData(vtkData_);

    // Actor in scene
    VTK_CREATE(vtkActor, actor);
    actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetPointSize(15);

    // VTK Renderer
    VTK_CREATE(vtkRenderer, ren);

    // Add Actor to renderer
    ren->AddActor(actor);
    ren->AddActor(lactor);
    ren->SetBackground(0.95,0.95,0.95);

    // VTK/Qt wedded
    qvtkWidget_->GetRenderWindow()->AddRenderer(ren);
    qvtkWidget_->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->ResetCamera();


    //	tableView_->SetRepresentationFromInput(table_);
    //	tableView_->Update();

    vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
    axes->SetShaftTypeToLine();
    axes->SetXAxisLabelText("X");
    axes->SetYAxisLabelText("Y");
    axes->SetZAxisLabelText("Z");
    axes->GetXAxisTipProperty()->SetColor(0,0,0);
    axes->GetYAxisTipProperty()->SetColor(0,0,0);
    axes->GetZAxisTipProperty()->SetColor(0,0,0);
    axes->GetXAxisShaftProperty()->SetColor(0,0,0);
    axes->GetYAxisShaftProperty()->SetColor(0,0,0);
    axes->GetZAxisShaftProperty()->SetColor(0,0,0);
    axes->SetTotalLength(0.5,0.5,0.5);

    orientationWidget_ = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    orientationWidget_->SetOrientationMarker(axes);
    orientationWidget_->SetOutlineColor(0.1,0.1,0.1);
    orientationWidget_->SetInteractor(qvtkWidget_->GetInteractor());
    orientationWidget_->SetViewport(0.0,0.0,0.25,0.25);
    orientationWidget_->SetEnabled(1);

}


void MdsObjectMdiSubWindow::highlightSelectedPoint(QTableWidgetItem* item){
    int row = item->row();
    if(row<0) return;

    float val = item->isSelected()?0.0:1.0;
    vtkVariant vtkVal(val);
    vtkData_->GetCellData()->GetScalars()->SetVariantValue(row,vtkVal);
    vtkData_->Modified();
    qvtkWidget_->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->Render();

}


void MdsObjectMdiSubWindow::toolboxIndexChanged(int index){
    if(index == 1) return;
    for( int i=0; i<nPoints_; ++i ) {
        vtkData_->GetCellData()->GetScalars()->SetVariantValue(i,1);
    }
    vtkData_->Modified();
    qvtkWidget_->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->Render();
}

void MdsObjectMdiSubWindow::highlightSelectedPairPoints(QTableWidgetItem* item)
{
    cout << "highlighting seelcted pair" << endl;
    int row = item->row();
    int col = item->column();
    if(row<0 || col<0) return;

    // Set the already selected points to 1
    vtkData_->GetCellData()->GetScalars()->SetVariantValue(selectedPairPoints_.first,1);
    vtkData_->GetCellData()->GetScalars()->SetVariantValue(selectedPairPoints_.second,1);

    selectedPairPoints_.first = row;
    selectedPairPoints_.second = col;
    vtkData_->GetCellData()->GetScalars()->SetVariantValue(selectedPairPoints_.first,0);
    vtkData_->GetCellData()->GetScalars()->SetVariantValue(selectedPairPoints_.second,0);

    vtkData_->Modified();
    qvtkWidget_->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->Render();

}

// execute k means clustering with specified number of clusters
void MdsObjectMdiSubWindow::create_clusters_button_clicked()
{
    unsigned int num_clusters = static_cast<unsigned int>(num_clusters_spinbox_->value());

    mdsObject_->kmeans(num_clusters);
    // repopulate cluster list from mdsobject
    clusterList_->clear();
    QStringList clusterNameList;
    std::vector<MultiDimScalingSpace::Cluster> clusterObjects = mdsObject_->getClusters();
    for(int i=0; i< clusterObjects.size(); ++i) {
        clusterNameList.append(clusterObjects[i].name.c_str());
    }
    clusterNameList.sort();

    cout << "Successfully Created K-Means" << endl;
    clusterList_->addItems(clusterNameList);

}

// cluster selected from the cluster list
void MdsObjectMdiSubWindow::clusterList_selected(int listIndex) {
    if (listIndex < 0) return;
    QString cluster_name = clusterList_->currentItem()->text();
    std::vector<MultiDimScalingSpace::Cluster> clusterObjects = mdsObject_->getClusters();
    for(int i=0; i< clusterObjects.size(); ++i) {
        // find cluster
        if (cluster_name.toStdString().compare(clusterObjects[i].name) == 0) {
            MultiDimScalingSpace::Cluster cluster = clusterObjects[i];

            // debug
            printf("Cluster assignments:\n");
            vector<unsigned int>::iterator vit;
            for (vit = cluster.clusters.begin(); vit != cluster.clusters.end(); vit++)
                printf("%d,", *vit);
            printf("\n");
            // TODO: update colors of dots to reflect cluster assignments in 'cluster.clusters'
            //       number of clusters = cluster.num_clusters
        }
    }


}
