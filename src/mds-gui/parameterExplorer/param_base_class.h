#ifndef PARAM_BASE_CLASS_H
#define PARAM_BASE_CLASS_H


// SGems Specific Headers
#include <QTabWidget>
#include <multi_dim_scaling_space.h>
#include <metric_manager_repository.h>
#include <MetricData.h>
#include <MetricDataManager.h>
#include <GsTL/utils/smartptr.h>
#include <utils/manager.h>
#include <appli/manager_repository.h>
#include <charts/chart_mdi_area.h>

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
#include <QMessageBox>
#include <QListWidget>
#include <QTreeWidgetItem>

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


    // ************************ SGEMS Interface Objects************************/
    Chart_mdi_area* mdiParent_;
    MultiDimScalingSpace* mdsObject_;
    QString mdsSpaceName_;


    // ************************ SQL Interface Objects**************************/
    QSqlDatabase db_;
    QStringList propertyList_;
    QHash<QString, geoStatParam> parameterList_;

    // ************************ Plotting Objects ******************************/
    QStringList plotParameters_;
    std::vector<float> plotValues_;

    // ************************** SQL Functions *******************************/
    // These functions are used to communicate with the SQL database
    // Connect to database
    bool connectToDatabase();
    // Read values given parameter and realization names
    QString queryDatabase(const QString &parameter, const QString &realization);
    // A Wrapper around queryDatabase for reading multiple parameters at the
    // same time
    QStringList queryValues(const QStringList &parameters,
                            const QString &realization);
    // Helper function that determines if parameter value can be cast to float
    bool isCastable(const QString &valueStr, float &valueFlt);
    // Takes a node and returns the full path of the node by flattening its
    // parent nodes
    QString flattenPath(parameterNode* inputNode);
    // Recursive function that prepends parent node names to currentPath
    void traversePath(parameterNode *node, QString *currentPath);
    // Function that given a set of set of realizations generates a
    // parameterModel
    parameterModel *findCommonParameters();

    // Function that takes realization and writes parameters to
    // displayParameterModel
    void getParameterList(const QString &realization);
    // Function that given a list of realizations, finds all parameters that
    // are shared by their generating algorithms
    QStringList findSharedParameters(const QStringList &realizations);

    // ************************* Item Models *********************************/
    // These item models are used to store data in our MVC framework
    // Model : ListWidget to store currently available realizations
    QListWidget realizationsWidget;

    // Model : ParameterModel to store common parameter names among selected
    // realizations
    parameterModel *commonParametersModel;

    // Model: ParameterModel that stores the parameter name and value to be
    // displayed
    parameterModel *displayParameterModel;

    // Model: AbstractModel used to store the results of searching
    QAbstractItemModel *model;

    virtual bool update();

protected slots:

    // Function to plot parameters given realizations and parameter
    void createParameterPlotWindow();

signals:
    // Signal used to highlight selected realizations
    void highlightRealizations(const QStringList &realizations);

public slots:

};

#endif // PARAM_BASE_CLASS_H
