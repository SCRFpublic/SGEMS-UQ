#ifndef METRICGENERATION_H
#define METRICGENERATION_H

//#include "MetricDataItemModel.h"
#include <common.h>
#include "MetricDataTable.h"

#include <appli/project.h>

#include <Qt>
#include <QFrame>
#include <QMdiArea>
#include <QTreeWidget>
#include <QTreeView>
#include <QListView>
#include <QComboBox>
#include <QStackedWidget>
#include <QMap>
#include <QString>

#include <string>

class QTreeWidgetItem;
class QLabel;


class METRICSGUI_DECL MetricExplorer : public QFrame
{
    Q_OBJECT

public:
    explicit MetricExplorer(GsTL_project* proj, QWidget *parent = 0);
    ~MetricExplorer();

    void setTopLevelMetricModel(QAbstractItemModel* model);

public slots :
	void SaveToFileClicked();

signals:
	void SaveToFile(const QModelIndex&, const QString& fileName);

protected :

    GsTL_project* proj_;
    QTreeView* metricsView_;




protected :
//    void setupMetricUI();

    QWidget* CreateFilterParameter(const QString& type );

protected slots:
 //   void ShowFilterParameter(QTreeWidgetItem*,int);


};

#endif // METRICGENERATION_H
