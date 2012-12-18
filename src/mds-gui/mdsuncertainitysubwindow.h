#ifndef MDSUNCERTAINITYSUBWINDOW_H
#define MDSUNCERTAINITYSUBWINDOW_H

#include <common.h>
#include <QWidget>

#include <multi_dim_scaling_space.h>

#include <QVTKWidget.h>
#include <vtkTable.h>
#include <vtkQtTableView.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkMaskPoints.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkLookupTable.h>

#include <QMdiSubWindow>
#include <QString>
#include <QSpinBox>
#include <QTableWidgetItem>
#include <QListWidget>

//#include "MdsObjectMdiSubWindow.h"

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
#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkIntArray.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkPoints.h>
#include <vtkTable.h>
#include <vtkDoubleArray.h>
#include <vtkKMeansStatistics.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkInteractorStyleTrackballCamera.h>

#include <QFrame>
#include <QToolBox>
#include <QListWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QSplitter>

#include <QTreeView>
#include <QStandardItemModel>
#include <QItemSelectionModel>


//#include <parameterExplorer/mdsparameterexplorerbase.h>
//#include <parameterExplorer/mdsparameterexplorertabwidget.h>


#define VTK_CREATE(type, name) \
        vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QStringListModel>
#include <QFile>

#include <vector>
#include <utility>

#include <mdsutil.h>

class clusterParameterGraph;
class MDSParameterExplorerTabWidget;
namespace Ui {
    class MDSUncertainitySubWindow;
}

class METRICSGUI_DECL MDSUncertainitySubWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MDSUncertainitySubWindow(QString mdsObjectName, QWidget* parent=0);

    // Class that is used to save parameters
    MDSParameterExplorerTabWidget *parameterExploreModule;

    ~MDSUncertainitySubWindow();

private:
    Ui::MDSUncertainitySubWindow *ui;

public slots:

        void create_clusters_button_clicked();
        void colorCluster(QModelIndex index);
        void getSelectedPropName(){}
        void highlightPoints(QStringList & selPoints);

protected :
        void setUpVtkRenderingWidget();
        void updatePointLocations();

        // Set up cluster widget
        void setupClusterWidget();

        // Generate clusters
        void generateClusters();
        void updateClusterView();

protected :

        int nPoints_;
        std::pair<int,int> selectedPairPoints_;
        MultiDimScalingSpace* mdsObject_;

        vector< pair<QString, int> > clusterPairsVector;

        QHash<int, vector <pair<QString, int> > > pointClusterHash;

        vector<MDSUtil::clusterBlock> kmeansResults;

        QString mdsObjectName_;
        QSpinBox* num_clusters_spinbox_;
        QListWidget* clusterList_;
        QVTKWidget *qvtkWidget_;

        QStringListModel *propertyListModel;

        vtkSmartPointer<vtkTable> table_;
        vtkSmartPointer<vtkQtTableView> tableView_;

        vtkSmartPointer<vtkPolyData> vtkData_;
        vtkSmartPointer<vtkOrientationMarkerWidget> orientationWidget_;

        vtkSmartPointer<vtkMaskPoints> vtk_points_;

        // Standard model which we will use to store cluster info
        QStandardItemModel *clusterStandardModel;
        QStandardItem *rootNode;

        QList<QStandardItem *> prepareRow(const QString &first,
                                          const QString &second,
                                          const QString &third);


};

#endif // MDSUNCERTAINITYSUBWINDOW_H
