/*
 * chartmdiarea.h
 *
 *  Created on: Apr 27, 2011
 *      Author: aboucher
 */

#ifndef CHARTMDIAREA_H_
#define CHARTMDIAREA_H_

#include <common.h>
#include <QMdiArea>
#include <QDragEnterEvent>
#include <QDropEvent>

class METRICSGUI_DECL ChartDataFromMetricMdiArea : public QMdiArea
{
	Q_OBJECT

public:
	ChartDataFromMetricMdiArea(QWidget* parent=0);
	virtual ~ChartDataFromMetricMdiArea();

public slots:
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	void addCharts(QHash<QString, QStringList>& metricNameAndDataDirectoryPairs);

};

#endif /* CHARTMDIAREA_H_ */
