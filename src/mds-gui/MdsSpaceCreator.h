/*
 * MdsSpaceCreator.h
 *
 *  Created on: Apr 30, 2011
 *      Author: aboucher
 */

#ifndef MDSSPACECREATOR_H_
#define MDSSPACECREATOR_H_

#include <common.h>
#include "MultipleMetricDataSelector.h"
#include "MetricDistanceChooser.h"
#include <GsTLAppli/appli/project.h>
#include <GsTLAppli/extra/qtplugins/selectors.h>

#include <QDialog>
#include <QHash>
#include <QTextEdit>

class METRICSGUI_DECL MdsSpaceCreator : public QDialog
{
	Q_OBJECT
public:
	MdsSpaceCreator(GsTL_project* proj, QWidget* parent =0);
	virtual ~MdsSpaceCreator();

public slots :
//	void getSelectedPropertyList();

signals :
	void selectedPropertyList(const QStringList& );

protected slots:
	void showMetricKernelChooser(const QStringList& metricNames);

protected :
	GsTL_project* proj_;

	GridSelectorBasic* gridSelector_;
	OrderedPropertySelector* mPropSelector_;
	MultipleMetricDataSelector* metricSelector_;
        QTextEdit *metricSpaceName_;

	QGroupBox* kernelBox_;
	QHash<QString,MetricDistanceChooser*> kernelHash_;
};

#endif /* MDSSPACECREATOR_H_ */
