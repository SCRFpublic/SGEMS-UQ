#ifndef PARAM_DISPLAY_WIDGET_H
#define PARAM_DISPLAY_WIDGET_H

#include "param_base_class.h"
#include <QWidget>

namespace Ui {
    class param_display_widget;
}

class param_display_widget : public QWidget, public param_base_class
{
    Q_OBJECT

public:
    explicit param_display_widget(QWidget *parent = 0,
                                  Chart_mdi_area *mdi = 0,
                                  QString mdsSpaceName = QString(""));
    ~param_display_widget();
protected:
    bool update();
private slots:
    void updateDisplay(){this->update();}
private:
    Ui::param_display_widget *ui;

};

#endif // PARAM_DISPLAY_WIDGET_H
