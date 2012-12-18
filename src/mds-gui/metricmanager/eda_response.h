#ifndef EDA_RESPONSE_H
#define EDA_RESPONSE_H

#include <QObject>
#include <GsTLAppli/gui/charts/eda_charts.h>
#include <GsTLAppli/gui/charts/chart_mdi_area.h>
#include <GsTLAppli/gui/common.h>

#include <QDialog>
#include <QMainWindow>
#include <QItemSelectionModel>
#include <QLineEdit>



// GsTL Dependencies
#include <GsTL/utils/smartptr.h>
#include <common.h>
// GsTLAppli Dependencies
#include <GsTLAppli/utils/manager.h>
#include <GsTLAppli/appli/manager_repository.h>
#include <GsTLAppli/appli/root_model.h>
#include <GsTLAppli/appli/project.h>
#include <GsTLAppli/extra/qtplugins/project_proxy_model.h>
#include <GsTLAppli/extra/qtplugins/property_proxy_model.h>

#include <QFrame>
#include <QStandardItemModel>
#include <QTreeView>
//#include "metricplotsmdiarea.h"

//#include <actions/externalresponseioaction.h>


class METRICSGUI_DECL EDA_response : public EDA_chart
{
    Q_OBJECT
public:
    EDA_response(Chart_mdi_area* mdi_area, QWidget *parent = 0);
    ~EDA_response();

public slots:
    void setResponseTreeView();
    void updatePropertyExplorer(QModelIndex responseIndex);
    void updateGridSelection(QModelIndex gridIndex);

    private slots :
      void show_responses();

protected:
    Filter_root_proxy_model* proxy_model;
    Filter_root_proxy_model* grid_proxy_model;
    QStandardItemModel  *emptyModel;


    void setGridTreeView();

    QModelIndex responseBaseIndex;

//    MetricPlotsMdiArea *metricPlotsMdi;

    GsTL_project *proj_;

private:
    QTreeView* responseListTreeView_;
    QTreeView* propertyListTreeView_;
    QTreeView* gridListTreeView_;
};



class METRICSGUI_DECL EDA_response_factory : public EDA_chart_factory 
{

public:

  static Named_interface* create_new_interface(std::string&) {
    return new EDA_response_factory;
  }

  EDA_response_factory(){}
  ~EDA_response_factory(){}

  virtual QString title_name() const {return "View Responses";}
  virtual QString tab_name() const{return "Responses";}
  std::string name() const {return "Responses";}

  virtual EDA_chart* get_interface(Chart_mdi_area* mdi_area){return new EDA_response(mdi_area);}

};




#endif // EDA_RESPONSE_H
