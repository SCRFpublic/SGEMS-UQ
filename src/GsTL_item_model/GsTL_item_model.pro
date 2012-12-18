# -------------------------------------------------
# Project created by QtCreator 2011-04-23T14:21:27
# -------------------------------------------------
QT += core \
    gui \
    xml \
    sql
TARGET = /home/lewisli/code-dev/ar2gems/plugins/Geostat/GsTL_item_model
CONFIG += PLUGIN
CONFIG += debug
TEMPLATE = lib
INCLUDEPATH += /home/lewisli/code-dev/ar2gems/ar2GsTL
INCLUDEPATH += /home/lewisli/code-dev/ar2gems
INCLUDEPATH += /home/lewisli/code-dev/metrics-ar2gems/sgems-metrics
LIBS += -L/home/lewisli/code-dev/ar2gems/lib/linux \
    -L/home/lewisli/code-dev/ar2gems/plugins/designer \
    -L/home/lewisli/code-dev/ar2gems/plugins/Geostat \
    -lGsTLAppli_actions \
    -lGsTLAppli_appli \
    -lGsTLAppli_extragui \
    -lGsTLAppli_filters \
    -lGsTLAppli_geostat \
    -lGsTLAppli_grid \
    -lGsTLAppli_math \
    -lGsTLAppli_utils \
    -lGsTLwidgets
SOURCES += MdsSpaceObjectModel.cpp \
    multi_dim_scaling_space.cpp \
    domattributeitem.cpp \
    parameterdommodel.cpp \
    metricdommodel.cpp \
    dommodel.cpp \
    domitem.cpp \
    library_metric_init.cpp \
    gstl_item.cpp \
    MetricData.cpp \
    MetricDataManager.cpp \
    Metric_data_io_filter.cpp \
    Metric_data_actions.cpp \
    mds_space_filters.cpp
HEADERS += MdsSpaceObjectModel.h \
    common.h \
    mds.h \
    multi_dim_scaling_space.h \
    domattributeitem.h \
    parameterdommodel.h \
    metricdommodel.h \
    dommodel.h \
    domitem.h \
    GenericKernels.h \
    gstl_item.h \
    metric_manager_repository.h \
    MetricData.h \
    MetricDataManager.h \
    mdsutil.h \
    Metric_data_io_filter.h \
    Metric_data_actions.h \
    mds_space_filters.h
