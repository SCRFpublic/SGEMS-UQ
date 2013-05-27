QT += core \
    gui \
    xml \
    sql
TARGET = /home/lewisli/code-dev/ar2tech-SGeMS-public/plugins/Geostat/sgems-metrics
CONFIG += PLUGIN
CONFIG += debug
TEMPLATE = lib
INCLUDEPATH += /home/lewisli/code-dev/ar2tech-SGeMS-public/ar2GsTL
INCLUDEPATH += /home/lewisli/code-dev/ar2tech-SGeMS-public
INCLUDEPATH += /home/lewisli/code-dev/SGEMS-UQ/src/GsTL_item_model
LIBS += -L/home/lewisli/code-dev/ar2tech-SGeMS-public/lib/linux \
    -L/home/lewisli/code-dev/ar2tech-SGeMS-public/plugins/designer \
    -L/home/lewisli/code-dev/ar2tech-SGeMS-public/plugins/Geostat \
    -lar2gems_actions \
    -lar2gems_appli \
    -lar2gems_extragui \
    -lar2gems_io \
    -lar2gems_geostat \
    -lar2gems_grid \
    -lar2gems_math \
    -lar2gems_utils \
    -lar2gems_widgets \
    -lGsTL_item_model
HEADERS += \
    mds_action.h \
    kmeans_action.h \
    kmeans.h \
    GenerateMetricsAction.h \
    metrics_action_common.h \
    metric_gbody_algo.h \
    MetricFilterPackage.h \
    MetricFilterCollection.h \
    MetricFilter.h \
    Metric_algo_stats.h \
    Metric_algo.h \
    ml_library_constants.h \
    filters/MDSUncertaintySpaceOutputFilter.h \
    filters/MDSUncertaintySpaceInputFilter.h \
    filters/MDSUncertaintySpaceFilters.h \
    filters/externalresponseinputfilter.h \
    filters/externalresponsefilter.h \
    actions/UncertainSpaceIOAction.h \
    actions/externalresponseioaction.h

SOURCES += \
    mds_action.cpp \
    kmeans_action.cpp \
    GenerateMetricsAction.cpp \
    metric_gbody_algo.cpp \
    MetricFilterPackage.cpp \
    MetricFilterCollection.cpp \
    MetricFilter.cpp \
    Metric_algo_stats.cpp \
    ml_library_init.cpp \
    filters/MDSUncertaintySpaceOutputFilter.cpp \
    filters/MDSUncertaintySpaceInputFilter.cpp \
    filters/MDSUncertaintySpaceFilters.cpp \
    filters/externalresponseinputfilter.cpp \
    filters/externalresponsefilter.cpp \
    actions/UncertainSpaceIOAction.cpp \
    actions/externalresponseioaction.cpp

FORMS += \
    MeanData.ui \
    VarianceData.ui
