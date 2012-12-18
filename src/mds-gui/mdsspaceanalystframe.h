#ifndef MDSSPACEANALYSTFRAME_H
#define MDSSPACEANALYSTFRAME_H

#include <common.h>
#include <QFrame>
#include "mds_mdi_area.h"
#include <GsTLAppli/appli/project.h>
#include <QListView>
#include <QStringListModel>

#include <metric_manager_repository.h>
#include <MdsSpaceObjectModel.h>

// GsTL Dependencies
#include <GsTL/utils/smartptr.h>

// GsTLAppli Dependencies
#include <GsTLAppli/utils/manager.h>
#include <GsTLAppli/appli/manager_repository.h>
#include <GsTLAppli/appli/root_model.h>
#include <GsTLAppli/appli/project.h>
#include <GsTLAppli/extra/qtplugins/project_proxy_model.h>
#include <GsTLAppli/extra/qtplugins/property_proxy_model.h>


#include <mdsspacecreatordialog.h>

#include <actions/UncertainSpaceIOAction.h>
#include <filters/MDSUncertaintySpaceInputFilter.h>
#include <filters/MDSUncertaintySpaceOutputFilter.h>

#include <QVBoxLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QPushButton>
#include <QFileDialog>
#include <QTreeView>

namespace Ui {
    class MDSSpaceAnalystFrame;
}

class METRICSGUI_DECL MDSSpaceAnalystFrame : public QFrame
{
    Q_OBJECT

public:
    explicit MDSSpaceAnalystFrame(GsTL_project *proj, QWidget *parent = 0);
    ~MDSSpaceAnalystFrame();

public slots:
    void updateModel(){}
    void createNewMDSSpace();
    void deleteMDSSpace();
    void loadMDSSpace();
    void saveMDSSpace();
    void updatePropertyExplorer(QModelIndex mdsSpaceIndex);
    void updateResponseExplorer(QModelIndex mdsSpaceIndex);


protected:
    GsTL_project *proj_;
    MdsMdiArea *mdiArea_;
    QListView *mdsObjectView_;
    QModelIndex mdsBaseIndex;

    Filter_root_proxy_model* proxy_model;
    QStandardItemModel  *emptyModel;


    // Object used to store the currently selected mdsobject
   // MultiDimScalingSpace* mdsObject_;

    void setMDSTreeView();
    void setTestTreeView();

private:
    Ui::MDSSpaceAnalystFrame *ui;
};

#endif // MDSSPACEANALYSTFRAME_H
