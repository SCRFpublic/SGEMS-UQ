#ifndef PARAM_BASE_CLASS_H
#define PARAM_BASE_CLASS_H


// SGems Specific Headers
#include <QTabWidget>
#include <multi_dim_scaling_space.h>
#include <metric_manager_repository.h>
#include <MetricData.h>
#include <MetricDataManager.h>
#include <GsTL/utils/smartptr.h>
#include <GsTLAppli/utils/manager.h>
#include <GsTLAppli/appli/manager_repository.h>
#include <GsTLAppli/charts/chart_mdi_area.h>

// Qt Specific Headers
#include <QWidget>
#include <QStringList>
#include <QStringListModel>
#include <QString>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QHash>
#include <QDomDocument>
#include <QXmlDefaultHandler>
#include <QItemSelectionModel>

#include <algorithm>

#define NONCASTABLE -1
#define FLOAT 0
#define INT 1

class parameterNode;
class parameterModel;

// We define this struct to store the individual parameters
// We check if the parameter value is castable to a floating point
// number and store what type it is.
struct geoStatParam
{
    // Raw string
    QString str;

    // What type of variable this is
    int type;

    // Value (Should be a union? use float for now)
    float val;
};

class param_base_class
{

public:
    explicit param_base_class(Chart_mdi_area *mdi,
                              QString mdsSpaceName = QString("")
                              );
protected:
    QSqlDatabase db_;

    QStringList propertyList_;

    QHash<QString, geoStatParam> parameterList_;

    QString mdsSpaceName_;

    parameterModel *displayParameterModel;

    QAbstractItemModel *model;

    bool connectToDatabase();
    QString queryDatabase(QString parameter, QString property);
    bool getParameterList(QString selectedProperty);
    bool isCastable(QString valueStr, float &valueFlt);

    QString flattenPath(parameterNode* inputNode);
    void traversePath(parameterNode *node, QString *currentPath);

    QStringList plotParameters_ ;
    std::vector<float> plotValues_;


    // Objects to interface with SGeMs
    Chart_mdi_area* mdiParent_;
    MultiDimScalingSpace* mdsObject_;

    virtual bool update();

signals:

public slots:

};

#endif // PARAM_BASE_CLASS_H
