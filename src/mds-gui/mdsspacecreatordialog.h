#ifndef MDSSPACECREATORDIALOG_H
#define MDSSPACECREATORDIALOG_H

#include <common.h>
#include <GsTLAppli/appli/project.h>

#include <MultipleMetricDataSelector.h>
#include <MetricDistanceChooser.h>
#include <GsTLAppli/appli/project.h>
#include <GsTLAppli/extra/qtplugins/selectors.h>
#include <GsTL/utils/smartptr.h>
#include <GsTLAppli/utils/manager.h>
#include <GsTLAppli/appli/manager_repository.h>
#include "metric_manager_repository.h"

#include <QDialog>
#include <QHash>
#include <QTextEdit>
#include "mds_action.h"

#include <GsTLAppli/utils/error_messages_handler.h>

#include <iostream>

#include <QtXml>

namespace Ui {
    class MDSSpaceCreatorDialog;
}

class METRICSGUI_DECL MDSSpaceCreatorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MDSSpaceCreatorDialog(GsTL_project* proj, QWidget* parent =0);
    ~MDSSpaceCreatorDialog();

private:
    Ui::MDSSpaceCreatorDialog *ui;


protected :
        GsTL_project* proj_;

        GridSelectorBasic* gridSelector_;
        OrderedPropertySelector* mPropSelector_;
        MultipleMetricDataSelector* metricSelector_;
        QTextEdit *metricSpaceName_;

        QStringList currentMetricList;
        QString currentKernel;
        QStringList currentProperties;

        QGroupBox* kernelBox_;
        QHash<QString,MetricDistanceChooser*> kernelHash_;

protected slots:
        void showMetricKernelChooser(const QStringList& metricNames);
        void generateSetParameters();
        void updatedMetricSelection(QStringList latestMetric);

private slots:
        void generateXML();

signals:
        void updateSpaceList();
      
};

#endif // MDSSPACECREATORDIALOG_H
