#ifndef MDS_GUI_H
#define MDS_GUI_H

#include <common.h>
#include <QtGui/QMainWindow>
#include <QStandardItemModel>

class GsTL_project;


class METRICSGUI_DECL mds_gui : public QMainWindow
{
    Q_OBJECT

public:
    mds_gui( GsTL_project* proj, QWidget *parent = 0);
    ~mds_gui();

public slots :
	void setMainWindowTitle(int pageNumber);

private:
    GsTL_project* proj_;
    QAbstractItemModel *topLevelMetricModel_;
//    QStandardItemModel *topLevelMetricModel_;

private :
    void buildModel();

};

#endif // MDS_GUI_H
