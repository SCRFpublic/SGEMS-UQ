/*
 * MetricDataListWidget.h
 *
 *  Created on: Apr 22, 2011
 *      Author: aboucher
 */

#ifndef METRICDATATABLE_H_
#define METRICDATATABLE_H_

#include <common.h>
#include <Qt>
#include <QTableView>
#include <QStandardItemModel>
#include <appli/project.h>

class METRICSGUI_DECL MetricDataTable : public QWidget
{

	Q_OBJECT

public:
	MetricDataTable(GsTL_project* proj, QWidget* parent=0);
	virtual ~MetricDataTable();

public slots:
	void setMetricModel();

signals :
	void clicked(QModelIndex& index);

protected :
	QTableView* tableView_;
	QStandardItemModel* model_;

	GsTL_project* proj_;
};

#endif /* METRICDATALISTWIDGET_H_ */
