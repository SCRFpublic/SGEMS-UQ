#ifndef PARAM_CLUSTER_WIDGET_H
#define PARAM_CLUSTER_WIDGET_H

#include <QWidget>
#include "param_base_class.h"

namespace Ui {
    class param_cluster_widget;
}

class param_cluster_widget : public QWidget, public param_base_class
{
    Q_OBJECT

public:
    explicit param_cluster_widget(QWidget *parent = 0,
                                  Chart_mdi_area *mdi = 0,
                                  QString mdsSpaceName = QString(""));
    ~param_cluster_widget();


protected:
    bool update();

public slots:
    // Update instances of cluster
    void updateClusterComboBox();

private slots:

    // Update cluster list
    void updateClusterList(QString numClusters);

    // Update cluster realizations list
    void updateClusterPropertyList(int row);

    // Update the parameters within each cluster
    void updateClusterParameters();

    void plotParameterValues();

private:
    Ui::param_cluster_widget *ui;
};

#endif // PARAM_CLUSTER_WIDGET_H
