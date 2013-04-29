#ifndef PARAM_SENSITIVITY_WIDGET_H
#define PARAM_SENSITIVITY_WIDGET_H

#include "mdsparametertablemodel.h"
#include "param_base_class.h"
#include "eigen3/Eigen/Dense"
#include <eigen3/Eigen/SVD>
#include <metric_utilities.h>

using namespace Eigen;


template<typename _Matrix_Type_>
bool pseudoInverse(const _Matrix_Type_ &a, _Matrix_Type_ &result, double
epsilon = std::numeric_limits<typename _Matrix_Type_::Scalar>::epsilon())
{
  if(a.rows()<a.cols())
      return false;

  Eigen::JacobiSVD< _Matrix_Type_ > svd = a.jacobiSvd();

  typename _Matrix_Type_::Scalar tolerance = epsilon * std::max(a.cols(),
a.rows()) * svd.singularValues().array().abs().maxCoeff();

  result = svd.matrixV() * _Matrix_Type_(_Matrix_Type_(
(svd.singularValues().array().abs() > tolerance).select(svd.singularValues().
      array().inverse(), 0) ).diagonal()) * svd.matrixU().adjoint();
}

namespace Ui {
class param_sensitivity_widget;
}

class param_sensitivity_widget : public QWidget, public param_base_class
{
    Q_OBJECT

public:
    explicit param_sensitivity_widget(QWidget *parent = 0,
                                      Chart_mdi_area *mdi = 0,
                                      QString mdsSpaceName = QString(""));
    ~param_sensitivity_widget();

protected:
    bool update();

private:
    Ui::param_sensitivity_widget *ui;
    QString currentAlgo_;
    QString currentSearchParam_;



    parameterModel *sharedModel;

    Filter_root_proxy_model* proxy_model;

    VectorXf readResponseValues(QStringList realizations, QString response);
    MatrixXf readParameterValues(QStringList realizations, QStringList parameters);

    VectorXf computeSensitivity(MatrixXf &parameterMatrix,
                                VectorXf &responseVector);

    QStringList sortSensitivity(VectorXf &senVal, QStringList &paramNames);

    QStringList cleanParameterNames(QStringList &parameterNames);
    QModelIndex responseBaseIndex;


private slots:
    void updateParameterList();
    void updateResponseList();
    void addSelectedParameter();

    void plot();
};

#endif // PARAM_SENSITIVITY_WIDGET_H
