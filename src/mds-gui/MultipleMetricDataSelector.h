/*
 * MultipleMdsSpaceObjectSelector.h
 *
 *  Created on: Apr 30, 2011
 *      Author: aboucher
 */

#ifndef MULTIPLEMDSSPACEOBJECTSELECTOR_H_
#define MULTIPLEMDSSPACEOBJECTSELECTOR_H_

#include <common.h>
#include <QListWidget>
#include <QPushButton>
#include <QGroupBox>

class METRICSGUI_DECL MultipleMetricDataSelector : public QGroupBox
{
	Q_OBJECT

public:
	MultipleMetricDataSelector(QWidget* parent=0);
	virtual ~MultipleMetricDataSelector();

public slots :
	void setMetricsList(const QStringList& propSelection);
	void getSelectedMetricsList();

	void setSelectedGrid(const QString& gridName);




signals :
 void selectionApproved(const QStringList& );
 void selectedMetrics(QList<QListWidgetItem>& metrics);

protected :
	QListWidget* list_;
	QPushButton* selectButton_;

	//QStringList gridNames_;
	QString gridName_;
	QStringList propNames_;
	QStringList metricNames_;

};

#endif /* MULTIPLEMDSSPACEOBJECTSELECTOR_H_ */
