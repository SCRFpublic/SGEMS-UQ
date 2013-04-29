TEMPLATE = lib
TARGET = /home/lewisli/code-dev/ar2tech-SGeMS-public/plugins/actions/MDS_gui
CONFIG += PLUGIN
CONFIG += debug
QT += core \
    gui \
    xml \
    opengl \
    sql
LIBS += -L/usr/local/lib \
    -lvtkCommonCore-6.0 \
    -lvtksys-6.0 \
    -lvtkGUISupportQt-6.0 \
    -lvtkViewsQt-6.0 \
    -lvtkViewsCore-6.0 \
    -lvtkInteractionWidgets-6.0 \
    -lvtkRenderingCore-6.0 \
    -lvtkRenderingVolume-6.0 \
    -lvtkRenderingQt-6.0\
    -lvtkInfovisCore-6.0 \
    -lvtkImagingCore-6.0 \
    -lvtkIOCore-6.0 \
    -lvtkFiltersCore-6.0 \
    -lvtkFiltersHybrid-6.0 \
    -lvtkImagingHybrid-6.0 \
    -lvtkViewsCore-6.0 \
    -lvtkRenderingLabel-6.0 \
    -lvtkGUISupportQtSQL-6.0 \
    -lvtkGUISupportQtOpenGL-6.0 \
    -lvtkChartsCore-6.0
INCLUDEPATH += /home/lewisli/code-dev/ar2tech-SGeMS-public
INCLUDEPATH += /home/lewisli/code-dev/ar2tech-SGeMS-public/ar2GsTL
INCLUDEPATH += /home/lewisli/code-dev/SGEMS-UQ/src/GsTL_item_model
INCLUDEPATH += /home/lewisli/code-dev/SGEMS-UQ/src/sgems-metrics
INCLUDEPATH += /usr/local/include/vtk-6.0
LIBS += -L/home/lewisli/code-dev/ar2tech-SGeMS-public/lib/linux \
    -lGsTLAppli_actions \
    -lGsTLAppli_appli \
    -lGsTLAppli_gui \
    -lGsTLAppli_extragui \
    -lGsTLAppli_filters \
    -lGsTLAppli_geostat \
    -lGsTLAppli_grid \
    -lGsTLAppli_math \
    -lGsTLAppli_utils \
    -lGsTLwidgets \
    -L/home/lewisli/code-dev/ar2tech-SGeMS-public/plugins/designer \
    -L/home/lewisli/code-dev/ar2tech-SGeMS-public/plugins/Geostat \
    -lGsTL_item_model \
    -lsgems-metrics
HEADERS += \
    MetricDistanceChooser.h \
    MultipleMetricDataSelector.h \
    mds_mdi_area.h \
    data_from_property_mdi_area.h \
    data_from_property_chart_mdi_subwindow.h \
    data_from_metric_chart_mdi_subwindow.h \
    chartmdiarea.h \
    vtkmdisubwindow.h \
    data_analyst_from_metric.h \
    parametertreeview.h \
    metric_explorer.h \
    MetricDataTable.h \
    mds_gui.h \
    MDS_gui_action.h \
    mdsspacecreatordialog.h \
    mdsspaceanalystframe.h \
    mdsuncertainitysubwindow.h \
    ../G_stl_item_model/mdsutil.h \
    metricmanager/mdsmetricmanagerframe.h \
    metricmanager/metricplotsmdiarea.h \
    metric_utilities.h \
    metricmanager/mdsmetricplotsubwindow.h \
    metricmanager/colorlisteditor.h \
    metricmanager/mdsresponseitemdelegate.h \
    parameterExplorer/mdsparameterexplorertabwidget.h \
    parameterExplorer/mdsparametertablemodel.h \
    eda_mds_space.h \
    metricmanager/eda_response.h \
    chart_mds.h \
    metricmanager/chart_response.h \
    parameterExplorer/param_base_class.h \
    parameterExplorer/param_display_widget.h \
    parameterExplorer/param_search_widget.h \
    parameterExplorer/param_plot_window.h \
    parameterExplorer/param_sensitivity_widget.h \
    parameterExplorer/param_plot_widget.h \
    parameterExplorer/param_cluster_widget.h \
    parameterExplorer/param_commonmodels_widget.h \
    parameterExplorer/param_commonparam_widget.h
SOURCES += \
    MetricDistanceChooser.cpp \
    MultipleMetricDataSelector.cpp \
    mds_mdi_area.cpp \
    data_from_property_mdi_area.cpp \
    data_from_property_chart_mdi_subwindow.cpp \
    data_from_metric_chart_mdi_subwindow.cpp \
    chartmdiarea.cpp \
    vtkmdisubwindow.cpp \
    data_analyst_from_metric.cpp \
    parametertreeview.cpp \
    metric_explorer.cpp \
    MetricDataTable.cpp \
    mds_gui.cpp \
    MDS_gui_action.cpp \
    mdsspacecreatordialog.cpp \
    mdsspaceanalystframe.cpp \
    mdsuncertainitysubwindow.cpp \
    mds_gui_library_init.cpp \
    metricmanager/mdsmetricmanagerframe.cpp \
    metricmanager/metricplotsmdiarea.cpp \
    metric_utilities.cpp \
    metricmanager/mdsmetricplotsubwindow.cpp \
    metricmanager/colorlisteditor.cpp \
    metricmanager/mdsresponseitemdelegate.cpp \
    parameterExplorer/mdsparameterexplorertabwidget.cpp \
    parameterExplorer/mdsparametertablemodel.cpp \
    eda_mds_space.cpp \
    metricmanager/eda_response.cpp \
    chart_mds.cpp \
    metricmanager/chart_response.cpp \
    parameterExplorer/param_base_class.cpp \
    parameterExplorer/param_display_widget.cpp \
    parameterExplorer/param_search_widget.cpp \
    parameterExplorer/param_plot_window.cpp \
    parameterExplorer/param_sensitivity_widget.cpp \
    parameterExplorer/param_plot_widget.cpp \
    parameterExplorer/param_cluster_widget.cpp \
    parameterExplorer/param_commonmodels_widget.cpp \
    parameterExplorer/param_commonparam_widget.cpp
FORMS += \  
    mdsspacecreatordialog.ui \
    mdsspaceanalystframe.ui \
    mdsuncertainitysubwindow.ui \
    metricmanager/mdsmetricmanagerframe.ui \
    metricmanager/mdsmetricplotsubwindow.ui \
    parameterExplorer/mdsparameterexplorertabwidget.ui \
    parameterExplorer/param_display_widget.ui \
    parameterExplorer/param_search_widget.ui \
    parameterExplorer/param_plot_window.ui \
    parameterExplorer/param_sensitivity_widget.ui \
    parameterExplorer/param_plot_widget.ui \
    parameterExplorer/param_cluster_widget.ui \
    parameterExplorer/param_commonmodels_widget.ui \
    parameterExplorer/param_commonparam_widget.ui
RESOURCES += mds-gui.qrc
