#ifndef PARAM_COMMONPARAM_WIDGET_H
#define PARAM_COMMONPARAM_WIDGET_H

#include <QWidget>
#include "param_base_class.h"

namespace Ui {
    class param_commonParam_widget;
}

class param_commonParam_widget : public QWidget, public param_base_class
{
    Q_OBJECT

public:
    explicit param_commonParam_widget(QWidget *parent=0,
                                      Chart_mdi_area *mdi=0,
                                      QString mdsSpaceName = QString(""));
    ~param_commonParam_widget();

protected:
    bool update();

private slots:
    void search();
    void findCommonParameters();

private:
    Ui::param_commonParam_widget *ui;


};

#endif // PARAM_COMMONPARAM_WIDGET_H
