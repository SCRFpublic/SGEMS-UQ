/*
 * chart_data_from_property_mdi_area.h
 *
 *  Created on: Apr 29, 2011
 *      Author: aboucher
 */

#ifndef CHART_DATA_FROM_PROPERTY_MDI_AREA_H_
#define CHART_DATA_FROM_PROPERTY_MDI_AREA_H_

#include <common.h>
#include <QMdiArea>
#include <QDragEnterEvent>
#include <QDropEvent>

class METRICSGUI_DECL ChartDataFromPropertyMdiArea :public QMdiArea
{
	Q_OBJECT

public:
	ChartDataFromPropertyMdiArea(QWidget* parent=0);
	virtual ~ChartDataFromPropertyMdiArea();

public slots:
void dragEnterEvent(QDragEnterEvent *event);
void dropEvent(QDropEvent *event);

protected :
	void addChart(QString metricName, QStringList dataDirectoryList);

};

#endif /* CHART_DATA_FROM_PROPERTY_MDI_AREA_H_ */
