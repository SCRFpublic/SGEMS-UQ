/*
 * MdsSpaceAnalyst.h
 *
 *  Created on: Apr 29, 2011
 *      Author: aboucher
 */

#ifndef MDSSPACEANALYST_H_
#define MDSSPACEANALYST_H_

#include <common.h>
#include "mds_mdi_area.h"
#include <GsTLAppli/appli/project.h>
#include <QListView>
#include <QStringListModel>

class METRICSGUI_DECL MdsSpaceAnalyst : public QFrame
{

    Q_OBJECT

public:
	MdsSpaceAnalyst(GsTL_project* proj, QWidget *parent=0);
	virtual ~MdsSpaceAnalyst();

public slots:
	void updateModel();
	void create_mds_space_button_clicked();
        void updateList();

protected :



protected :
    GsTL_project* proj_;

    MdsMdiArea *mdiArea_;
    QListView* mdsObjectView_;
    QStringListModel* model_;

};

#endif /* MDSSPACEANALYST_H_ */
