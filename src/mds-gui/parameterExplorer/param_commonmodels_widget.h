#ifndef PARAM_COMMONMODELS_WIDGET_H
#define PARAM_COMMONMODELS_WIDGET_H

#include <QWidget>
#include <QString>
#include <QStringList>
#include "param_base_class.h"

namespace Ui {
    class param_commonModels_widget;
}

class param_commonModels_widget : public QWidget, public param_base_class
{
    Q_OBJECT

public:
    explicit param_commonModels_widget(QWidget *parent = 0,
                                       Chart_mdi_area *mdi = 0,
                                       QString mdsSpaceName = QString(""));
    ~param_commonModels_widget();

protected:
    bool update();

private slots:
    void updateParameterList();
    void search();

private:
    Ui::param_commonModels_widget *ui;

    QStringList findCommonModels(QString &model,QStringList &parameterList);


};

#endif // PARAM_COMMONMODELS_WIDGET_H
