#ifndef PARAM_SEARCH_WIDGET_H
#define PARAM_SEARCH_WIDGET_H

#include "param_base_class.h"

namespace Ui {
class param_search_widget;
}

class param_search_widget : public QWidget, public param_base_class
{
    Q_OBJECT

public:
    explicit param_search_widget(QWidget *parent = 0,
                                 Chart_mdi_area *mdi = 0,
                                 QString mdsSpaceName = QString(""));
    ~param_search_widget();


protected:
    bool update();

private slots:
    void updateParameterList(QString selectProp);
    void updateSelectedParameter();

    void plot();

    void search(){this->update();}
private:

    template < class UnaryOperator >
    std::vector<bool> numericalCompare(std::vector<float> &a,
                                       float val, UnaryOperator(op))
    {
        // Is there such a thing as a const_iterator in STL?
        std::vector<float> constVect;
        std::vector<bool> resultVect;
        constVect.reserve(a.size());
        resultVect.resize(a.size());

        for(unsigned int i = 0; i < a.size(); i++)
            constVect.push_back(val);

        // Perform transform
        std::transform(a.begin(),a.end(),constVect.begin(),resultVect.begin(),op);

        return resultVect;
    }


    QString currentAlgo_;
    QString currentSearchParam_;

    Ui::param_search_widget *ui;
};

#endif // PARAM_SEARCH_WIDGET_H
