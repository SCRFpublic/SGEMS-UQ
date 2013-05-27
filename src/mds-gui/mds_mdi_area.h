/*
 * mds_mdi_area.h
 *
 *  Created on: Apr 29, 2011
 *      Author: aboucher
 */

#ifndef MDS_MDI_AREA_H_
#define MDS_MDI_AREA_H_

#include <common.h>
#include <QMdiArea>
#include <QString>

class parameterPlotSubWindow;
#include <QHash>

class MDSUncertainitySubWindow;

class sharedParameterSubwindow;


class METRICSGUI_DECL MdsMdiArea : public QMdiArea
{
	Q_OBJECT

public:
	MdsMdiArea(QWidget* parent=0);
	virtual ~MdsMdiArea();

public slots:
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	void addChart(QString mdsObjectName);
  void showParameters(QString mdsObjectName,QString mdsPropName){}

  /*
        void showParametersChart(QString mdsObjectName);

        void addParameterSubViewer(std::vector<parameterPlotSubWindow*>
                                   subWindows);

        void addSharedParamSubViewer(sharedParameterSubwindow* subWindow);
        */
protected:
        // Use a hash table to store the MDSUncertainitySubWindow and
        // associated QMdiSubWindow that is displaying it
        QHash<QString, std::pair<MDSUncertainitySubWindow*, QMdiSubWindow*> >
        hashObjectWindows_;

};

#endif /* MDS_MDI_AREA_H_ */
