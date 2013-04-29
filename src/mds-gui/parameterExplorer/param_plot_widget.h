#ifndef PARAM_PLOT_WIDGET_H
#define PARAM_PLOT_WIDGET_H

#include <QWidget>

#include "param_base_class.h"
#include "mdsparametertablemodel.h"
#include <metric_utilities.h>

namespace Ui {
    class param_plot_widget;
}

class param_plot_widget : public QWidget, public param_base_class
{
    Q_OBJECT

public:
    explicit param_plot_widget(QWidget *parent = 0, Chart_mdi_area *mdi =0,
                               QString mdsSpaceName = QString(""));
    ~param_plot_widget();

protected:
    bool update();

private:
    Ui::param_plot_widget *ui;
    parameterModel *sharedModel;

private slots:
    // Slot that calls function to compute the common parameters for the
    // selected realizations
    void updateParameterScrollArea();

    // Slot to plot
    void plotParameterValues();
};

#endif // PARAM_PLOT_WIDGET_H
