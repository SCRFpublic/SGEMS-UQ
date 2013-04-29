/*
 * parametertreeview.h
 *
 *  Created on: Apr 26, 2011
 *      Author: aboucher
 */

#ifndef PARAMETERTREEVIEW_H_
#define PARAMETERTREEVIEW_H_

#include <common.h>
#include <parameterdommodel.h>

#include <QTreeView>
#include <QDomDocument>

class METRICSGUI_DECL ParameterTreeView : public QWidget
{
	Q_OBJECT

public:
	ParameterTreeView(QWidget* parent=0);
	virtual ~ParameterTreeView();

public slots :
	void showParameters(const QModelIndex& index);

protected :
	QTreeView *treeView_;
	ParameterDomModel* parameterModel_;
	QDomDocument docParam_;


};

#endif /* PARAMETERTREEVIEW_H_ */
