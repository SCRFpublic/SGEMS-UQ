#include "chart_mds.h"
#include "ui_mdsuncertainitysubwindow.h"
#include <QFile>

static unsigned char red[3] = {175, 0, 0};
static unsigned char green[3] = {0, 255, 0};
static unsigned char blue[3] = {30, 144, 255};
static unsigned char white[3]= {255,255,255};

Chart_mds::Chart_mds(QString mdsObjectName, Chart_mdi_area* mdi_area,
                     QWidget* parent)
    :Chart_base(parent), mdi_area_(mdi_area),mdsObjectName_(mdsObjectName),
      ui(new Ui::MDSUncertainitySubWindow)
{
    ui->setupUi(this);

    // Set window resize properties
    //this->setLayout(this->ui->mainLayout);
    this->setWindowTitle("Uncertainity Space Visualization For : "
                         + mdsObjectName);

    SmartPtr<Named_interface> ni =
            Root::instance()->interface( mds_manager+"/"+
                                         mdsObjectName.toStdString());

    mdsObject_ = dynamic_cast<MultiDimScalingSpace*>(ni.raw_ptr());

    this->mdsObjectName_ = mdsObjectName;

    nPoints_ = mdsObject_->pointCount();
    selectedPairPoints_.first = 0;
    selectedPairPoints_.second = 0;

    // This is the window which shows the visualiation
    qvtkWidget_ = new QVTKWidget(this);
    qvtkWidget_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    qvtkWidget_->setMinimumWidth(80);

    // Prepare the Rendering Widget
    this->setUpVtkRenderingWidget();

    this->ui->vtkScrollArea->setWidget(this->qvtkWidget_);
    this->qvtkWidget_->show();

    // Prepare cluster widget
    this->ui->clusterArea->setLayout(this->ui->clusterLayout);
    this->setupClusterWidget();

    this->parameterExploreModule = new MDSParameterExplorerTabWidget(mdi_area_,this->mdsObjectName_,this);

    // Add items to toolbox
    this->ui->toolBox->removeItem(0);
    this->ui->toolBox->removeItem(0);
    this->ui->toolBox->removeItem(0);
    this->ui->toolBox->addItem(this->ui->tableWidget, "Point Locations");
    this->ui->toolBox->addItem(this->ui->clusterArea,"Clustering");
    this->ui->toolBox->addItem(parameterExploreModule, "Parameter Explorer");


    //    this->ui->toolBox->setMinimumWidth(410);
    this->ui->toolBox->setMaximumWidth(410);

    // Write tree
    this->clusterStandardModel = new QStandardItemModel;

    connect(this->ui->generateClusters,SIGNAL(clicked()),this,
            SLOT(create_clusters_button_clicked()));
    connect(this->ui->clusterTreeView,SIGNAL(clicked(QModelIndex)),this,
            SLOT(colorCluster(QModelIndex)));
    connect(parameterExploreModule,SIGNAL(highlightCommonModels(QStringList&)),
            this,SLOT(highlightPoints(QStringList&)));
    connect(ui->browsePushButton,SIGNAL(clicked()),this,SLOT(openFileDialog()));
    connect(ui->savePushButton,SIGNAL(clicked()),this,SLOT(saveClusterInfo()));

    this->updatePointLocations();

    // Grab any saved kmeans results
    this->kmeansResults = *this->mdsObject_->getKmeans();
    this->updateClusterView();

}

Chart_mds::~Chart_mds()
{
    delete ui;
}

void Chart_mds::setUpVtkRenderingWidget()
{
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

    // This table should have the same number of rows as the data we have read
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
        mdsPts->SetPoint(i, mdsObject_->getXCoords(i),
                         mdsObject_->getYCoords(i),
                         mdsObject_->getZCoords(i));
    }

    vtkSmartPointer<vtkCellArray> pts_connection =
            vtkSmartPointer<vtkCellArray>::New();
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
    hier->GetTextProperty()->SetFontSize(18);

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


    // Setup colors
    vtkSmartPointer<vtkUnsignedCharArray> colors =
            vtkSmartPointer<vtkUnsignedCharArray>::New();
    colors->SetNumberOfComponents(3);
    colors->SetName ("Colors");

    for (unsigned int i = 0; i < this->nPoints_; i++)
    {
        colors->InsertNextTupleValue(blue);
    }

    vtkData_->GetCellData()->SetScalars(colors);
    vtkData_->GetPointData()->SetScalars(colors);

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
    ren->SetBackground(1,1,1);

    // VTK/Qt wedded
    qvtkWidget_->GetRenderWindow()->AddRenderer(ren);
    qvtkWidget_->GetRenderWindow()->GetRenderers()->
            GetFirstRenderer()->ResetCamera();


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
void Chart_mds::updatePointLocations()
{

    // Set TableView properties
    this->ui->tableWidget->setSelectionMode ( QAbstractItemView::MultiSelection );

    this->ui->tableWidget->setSelectionMode ( QAbstractItemView::MultiSelection );
    this->ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows );
    this->ui->tableWidget->setColumnCount(4);
    this->ui->tableWidget->setRowCount(nPoints_);
    this->ui->tableWidget->setHorizontalHeaderItem ( 0, new QTableWidgetItem(tr("Property") ));
    this->ui->tableWidget->setHorizontalHeaderItem ( 1, new QTableWidgetItem(tr("Dim 1") ) );
    this->ui->tableWidget->setHorizontalHeaderItem ( 2, new QTableWidgetItem(tr("Dim 2") ) );
    this->ui->tableWidget->setHorizontalHeaderItem ( 3, new QTableWidgetItem(tr("Dim 3") ) );
    for(int i=0; i < nPoints_; i++)
    {
        this->ui->tableWidget->setItem(i, 0, new QTableWidgetItem(tr("%1").arg( mdsObject_->getPropertyName(i).c_str()  )));
        this->ui->tableWidget->setItem(i, 1, new QTableWidgetItem(tr("%1").arg( table_->GetValue(i, 1).ToFloat())));
        this->ui->tableWidget->setItem(i, 2, new QTableWidgetItem(tr("%1").arg( table_->GetValue(i, 2).ToFloat())));
        this->ui->tableWidget->setItem(i, 3, new QTableWidgetItem(tr("%1").arg( table_->GetValue(i, 3).ToFloat())));
    }

}
void Chart_mds::setupClusterWidget()
{
    QStringList clusterNameList;
    std::vector<MultiDimScalingSpace::Cluster> clusterObjects =
            mdsObject_->getClusters();
    for(int i=0; i< clusterObjects.size(); ++i) {
        clusterNameList.append(clusterObjects[i].name.c_str());
    }
    clusterNameList.sort();
    //this->ui->clusterListWidget->addItems(clusterNameList);
}
void Chart_mds::create_clusters_button_clicked()
{
    this->generateClusters();
}
void Chart_mds::generateClusters()
{
    // Cast points from float into VTKPoints
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    for (unsigned int i = 0; i < this->nPoints_; i++)
    {
        float x = this->table_->GetValue(i,1).ToFloat();
        float y = this->table_->GetValue(i,2).ToFloat();
        float z = this->table_->GetValue(i,3).ToFloat();
        points->InsertNextPoint(x,y,z);
    }

    // Get the points into the format needed for KMeans
    vtkSmartPointer<vtkTable> inputData = vtkSmartPointer<vtkTable>::New();

    for( int c = 0; c < 3; ++c )
    {
        std::stringstream colName;
        colName << "dim " << c << endl;
        vtkSmartPointer<vtkDoubleArray> doubleArray =
                vtkSmartPointer<vtkDoubleArray>::New();
        doubleArray->SetNumberOfComponents(1);
        doubleArray->SetName( colName.str().c_str() );
        doubleArray->SetNumberOfTuples(points->GetNumberOfPoints());
        for( int r = 0; r < points->GetNumberOfPoints(); ++ r )
        {
            double p[3];
            points->GetPoint(r, p);
            doubleArray->SetValue( r, p[c] );
        }
        inputData->AddColumn( doubleArray );
    }

    // Perform VTK K-Means
    vtkSmartPointer<vtkKMeansStatistics> kMeansStatistics =
            vtkSmartPointer<vtkKMeansStatistics>::New();

    kMeansStatistics->SetInputData( vtkStatisticsAlgorithm::INPUT_DATA, inputData);
    kMeansStatistics->SetColumnStatus( inputData->GetColumnName( 0 ) , 1 );
    kMeansStatistics->SetColumnStatus( inputData->GetColumnName( 1 ) , 1 );
    kMeansStatistics->SetColumnStatus( inputData->GetColumnName( 2 ) , 1 );
    kMeansStatistics->RequestSelectedColumns();
    kMeansStatistics->SetAssessOption( true );
    kMeansStatistics->SetDefaultNumberOfClusters(ui->numClusterSpinBox->value());
    kMeansStatistics->Update() ;

    // Display the results (for debugging)
    kMeansStatistics->GetOutput()->Dump();

    vtkMultiBlockDataSet* outputMetaDS =
            vtkMultiBlockDataSet::SafeDownCast(
                kMeansStatistics->GetOutputDataObject(
                    vtkStatisticsAlgorithm::OUTPUT_MODEL ) );

    vtkSmartPointer<vtkTable> outputMeta =
            vtkTable::SafeDownCast( outputMetaDS->GetBlock( 0 ) );

    outputMeta->Dump();

    vtkDoubleArray* coord0 =
            vtkDoubleArray::SafeDownCast(outputMeta->GetColumn(5));
    vtkDoubleArray* coord1 =
            vtkDoubleArray::SafeDownCast(outputMeta->GetColumn(6));
    vtkDoubleArray* coord2 =
            vtkDoubleArray::SafeDownCast(outputMeta->GetColumn(7));

    // Generate an vector pairs, storing point name and associated cluster
    this->clusterPairsVector.clear();
    for(int r = 0; r < kMeansStatistics->GetOutput()->GetNumberOfRows(); r++)
    {
        pair<QString, int> clusterPoint;
        vtkVariant v = kMeansStatistics->GetOutput()->
                GetValue(r,kMeansStatistics->GetOutput()->
                         GetNumberOfColumns() - 1);
        clusterPoint.first = QString(mdsObject_->getPropertyName(r).c_str());
        clusterPoint.second = v.ToInt();
        clusterPairsVector.push_back(clusterPoint);
    }

    // Define a clusterBlock with the number of clusters that was used for
    // this particular run of Kmeans.
    MDSUtil::clusterBlock kmeansBlock;
    int numBlock = this->ui->numClusterSpinBox->value();
    kmeansBlock.numBlock = numBlock;

    // Push this cluster pair vector onto a hash table so we can recover it
    // when we have loaded more than one set of clusters
    pointClusterHash[numBlock] = clusterPairsVector;
    for (unsigned int i = 0; i < this->ui->numClusterSpinBox->value(); i++)
    {
        // Create a new cluster struct here which represents the ith cluster
        MDSUtil::cluster currentCluster;

        // We need this to find the realization that is closest to the center
        // of the cluster
        double minDistance = numeric_limits<double>::max( );
        double cX = double(coord0->GetValue(i));
        double cY = double(coord1->GetValue(i));
        double cZ = double(coord2->GetValue(i));

        // Here we search through our vector of pairs to find all points within
        // the cluster on hand.
        for (unsigned int j = 0; j < this->nPoints_; j++)
        {
            // If this point is within the current cluster
            if (clusterPairsVector.at(j).second == i)
            {
                // Push the name of the property in the ith cluster onto our
                // cluster struct
                std::pair<QString,int> component;

                // Name of property
                component.first = clusterPairsVector.at(j).first;

                // ID of property
                component.second = j;

                // Compute the distance
                double pX = this->table_->GetValue(j,1).ToDouble();
                double pY = this->table_->GetValue(j,2).ToDouble();
                double pZ = this->table_->GetValue(j,3).ToDouble();

                double distance = sqrt((pX-cX)*(pX-cX) +
                                       (pY-cY)*(pY-cY) + (pZ-cZ)*(pZ-cZ));

                if (distance < minDistance)
                {
                    minDistance = distance;
                    currentCluster.centerName = component.first;
                }

                currentCluster.components.push_back(component);
            }
        }

        // Write the number of properties in the ith cluster
        currentCluster.numComponents = currentCluster.components.size();

        // Put the ith cluster onto our current block
        kmeansBlock.block.push_back(currentCluster);
    }

    // Put our block onto our total number of blocks
    this->kmeansResults.push_back(kmeansBlock);

    this->updateClusterView();


    // Write the results to the mdsObject
    this->mdsObject_->setKMeanResults(this->kmeansResults);

    // Update view of parameter explorer
    this->parameterExploreModule->loadClusterComboBox();
}
QList<QStandardItem *> Chart_mds::prepareRow(const QString &first,
                                             const QString &second,
                                             const QString &third)
{
    QList<QStandardItem *> rowItems;
    rowItems << new QStandardItem(first);
    rowItems << new QStandardItem(second);
    rowItems << new QStandardItem(third);
    return rowItems;
}
void Chart_mds::colorCluster(QModelIndex index)
{
    // Setup colors
    vtkSmartPointer<vtkUnsignedCharArray> colors =
            vtkSmartPointer<vtkUnsignedCharArray>::New();
    colors->SetNumberOfComponents(3);
    colors->SetName ("Colors");

    // Need to get the number of clusters that we have selected
    // Index is of Model: this->clusterStandardModel

    QModelIndex parentIndex = index.parent();

    if (parentIndex != QModelIndex())
    {
        QString clusterName = clusterStandardModel->data(parentIndex,Qt::DisplayRole).toString();

        // Get the number of clusters from the cluster name
        QString trunClustName = clusterName.remove("Number of Clusters: ");

        int clusterNumber = trunClustName.toInt();

        // This means we aren't at the top
        bool atTop = true;
        if (clusterNumber == 0)
        {
            atTop = false;
            // Look at index's grandparent. This should be done recursively
            // but for now we only have 2 rows

            QModelIndex grandParentIndex  = parentIndex.parent();

            if (grandParentIndex != QModelIndex())
            {
                clusterName = clusterStandardModel->
                        data(grandParentIndex).toString();
                trunClustName = clusterName.remove("Number of Clusters: ");
                clusterNumber = trunClustName.toInt();
            }
        }

        // Select the current selected cluster
        for (unsigned int i = 0; i < this->nPoints_; i++)
        {
            int clusterRow = pointClusterHash[clusterNumber].at(i).second;
            int propertyIndex = atTop ? index.row() : index.parent().row();
            if (clusterRow == propertyIndex)
            {
                colors->InsertNextTupleValue(red);
            }
            else
                colors->InsertNextTupleValue(blue);
        }

        vtkData_->GetCellData()->SetScalars(colors);
        vtkData_->GetPointData()->SetScalars(colors);

        vtkData_->Modified();
        qvtkWidget_->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->Render();
        qvtkWidget_->repaint();
    }


}
void Chart_mds::updateClusterView()
{
    this->clusterStandardModel->clear();

    // We use the vector this->kmeansResults to generate our table view
    for (unsigned int k = 0; k < this->kmeansResults.size(); ++k)
    {
        // The first level of our tree indicates the number of clusters that was
        // used for this particular run of Kmeans. We denote this first-level
        QList<QStandardItem*> topLevelElement;
        QString numClusterString = "Number of Clusters: ";
        std::stringstream out; out << this->kmeansResults.at(k).numBlock;
        numClusterString.append(QString::fromStdString(out.str()));

        topLevelElement << new QStandardItem(numClusterString);
        topLevelElement << new QStandardItem(QString("Location"));

        // Get the root item of the tree, append the first level item as child
        QStandardItem *item = this->clusterStandardModel->invisibleRootItem();
        item->appendRow(topLevelElement);

        // The second level of our tree indicates the cluster number. The third
        // level indicates the name of the point within that particular cluster
        for (unsigned int i = 0; i < this->kmeansResults.at(k).numBlock; i++)
        {
            QList<QStandardItem*> secondLevelElement;
            QString clusterNoString = "Cluster #";
            std::stringstream out1; out1 << i;
            clusterNoString.append(QString::fromStdString(out1.str()));
            secondLevelElement << new QStandardItem(clusterNoString);

            // Here we iterate through each cluster and abstract the points
            // associated with each cluster
            int clusterSize=this->kmeansResults.at(k).block.at(i).numComponents;

            for (unsigned int j = 0; j < clusterSize; ++j)
            {
                QList<QStandardItem*> thirdLevelElement;
                thirdLevelElement << new QStandardItem(
                                         this->kmeansResults.at(k).block.at(i).
                                         components.at(j).first);

                std::stringstream out2;
                int l = this->kmeansResults.at(k).block.at(i).components.
                        at(j).second;
                out2 << "(" << table_->GetValue(l, 1).ToFloat() << ","
                     << table_->GetValue(l, 2).ToFloat() << ","
                     << table_->GetValue(l, 3).ToFloat() << ")";

                thirdLevelElement << new QStandardItem(
                                         QString::fromStdString(out2.str()));

                secondLevelElement.first()->appendRow(thirdLevelElement);

            }

            // Append cluster to our table
            topLevelElement.first()->appendRow(secondLevelElement);
        }

    }

    this->ui->clusterTreeView->setModel(this->clusterStandardModel);
    this->ui->clusterTreeView->resizeColumnToContents(0);
    this->ui->clusterTreeView->resizeColumnToContents(1);
    this->ui->clusterTreeView->expandAll();
}
void Chart_mds::highlightPoints(QStringList &selPoints)
{
    // Setup colors

    vtkSmartPointer<vtkUnsignedCharArray> colors =
            vtkSmartPointer<vtkUnsignedCharArray>::New();
    colors->SetNumberOfComponents(3);
    colors->SetName ("Colors");

    for (unsigned int i = 0; i < this->nPoints_; i++)
    {
        std::string pointName = table_->GetValue(i,0).ToString();
        QString pointNameQStr = QString::fromStdString(pointName);

        if (selPoints.contains(pointNameQStr))
            colors->InsertNextTupleValue(red);
        else
            colors->InsertNextTupleValue(blue);
    }

    vtkData_->GetCellData()->SetScalars(colors);
    vtkData_->GetPointData()->SetScalars(colors);

    vtkData_->Modified();
    qvtkWidget_->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->Render();
    qvtkWidget_->repaint();

}

void Chart_mds::openFileDialog()
{
    QString caption = tr("Save As");
    QString currentDirectory = QDir::current().path();
    QString filter = tr("Text files (*.txt)");
    QString fileName = QFileDialog::getSaveFileName (this,
                                                     caption,
                                                     currentDirectory,
                                                     filter);
    if (!fileName.contains(".txt"))
        fileName.append(QString(".txt"));

    ui->filePathLineEdit->setText(fileName);

}

void Chart_mds::saveClusterInfo()
{
    // Get Filename
    if (ui->filePathLineEdit->text().size() == 0)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("Specify Save Location");
        msgBox.exec();
        return;
    }
    else
    {
        QFile file(ui->filePathLineEdit->text());
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);


        for (unsigned int i = 0; i < this->kmeansResults.size(); i++)
        {

            MDSUtil::clusterBlock cBlock = this->kmeansResults.at(i);
            out << "Instance " << i << ": Used " << cBlock.numBlock << " Clusters\n";

            for (unsigned int j = 0; j < cBlock.numBlock; j++)
            {
                MDSUtil::cluster currentClu =  cBlock.block.at(j);
                out << "Cluster " << j << " : " << currentClu.centerName <<"\n";
            }
            out << "\n";
        }

        file.close();

    }

}
