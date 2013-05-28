# -------------------------------------------------
# Project created by QtCreator 2011-04-23T14:21:27
# -------------------------------------------------
QT += core \
    gui \
    xml \
    sql
TARGET = $$(SGEMS_AR2TECH_DIR)/plugins/Geostat/GsTL_item_model
CONFIG += PLUGIN
CONFIG += debug
TEMPLATE = lib
INCLUDEPATH += $$(SGEMS_AR2TECH_DIR)/ar2GsTL
INCLUDEPATH += $$(SGEMS_AR2TECH_DIR)
INCLUDEPATH += $$(SGEMS_AR2TECH_DIR)/ar2gems
INCLUDEPATH += ../sgems-metrics
LIBS += -L$$(SGEMS_AR2TECH_DIR)/lib/linux \
    -L$(SGEMS_AR2TECH_DIR)/plugins/designer \
    -L$(SGEMS_AR2TECH_DIR)/plugins/Geostat \
    -lar2gems_actions \
    -lar2gems_appli \
    -lar2gems_extragui \
    -lar2gems_io \
    -lar2gems_geostat \
    -lar2gems_grid \
    -lar2gems_math \
    -lar2gems_utils \
    -lar2gems_widgets
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
