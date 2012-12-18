/*
 * MetricDistanceChooser.h
 *
 *  Created on: May 1, 2011
 *      Author: aboucher
 */

#ifndef METRICDISTANCECHOOSER_H_
#define METRICDISTANCECHOOSER_H_

#include <common.h>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>

class METRICSGUI_DECL MetricDistanceChooser : public QGroupBox
{
	Q_OBJECT

public:
	MetricDistanceChooser(QString metricName, QWidget* parent=0);
	virtual ~MetricDistanceChooser();

	QString getSelectedKernel();

  QString metricName(){return metricNameLabel_->text();}

  QString getCurrentKernel() {return kernelChooser_->currentText();}

protected :
	void populateKernelTypes();

protected :
	QLabel* metricNameLabel_;
	QComboBox* kernelChooser_;
};

#endif /* METRICDISTANCECHOOSER_H_ */
