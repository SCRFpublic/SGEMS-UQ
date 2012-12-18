TEMPLATE = lib
TARGET = /home/lewisli/code-dev/ar2gems/plugins/actions/MDS_gui
CONFIG += PLUGIN
CONFIG += debug
QT += core \
    gui \
    xml \
    opengl \
    sql
LIBS += -L/usr/local/lib/vtk-5.10/ \
    -lvtkCommon \
    -lvtksys \
    -lQVTK \
    -lvtkViews \
    -lvtkWidgets \
    -lvtkRendering \
    -lvtkVolumeRendering \
    -lvtkInfovis \
    -lvtkGraphics \
    -lvtkImaging \
    -lvtkIO \
    -lvtkFiltering \
    -lvtkHybrid \
    -lvtkViews \
    -lvtkCharts
INCLUDEPATH += /home/lewisli/code-dev/ar2gems
INCLUDEPATH += /home/lewisli/code-dev/ar2gems/ar2GsTL
INCLUDEPATH += /home/lewisli/code-dev/metrics-ar2gems/GsTL_item_model
INCLUDEPATH += /home/lewisli/code-dev/metrics-ar2gems/sgems-metrics
INCLUDEPATH += /usr/local/include/vtk-5.10
LIBS += -L/home/lewisli/code-dev/ar2gems/lib/linux \
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
    -L/home/lewisli/code-dev/ar2gems/plugins/designer \
    -L/home/lewisli/code-dev/ar2gems/plugins/Geostat \
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
    parameterExplorer/parameterplotsubwindow.h \
    parameterExplorer/sharedparametersubwindow.h \
    eda_mds_space.h \
    metricmanager/eda_response.h \
    chart_mds.h \
    metricmanager/chart_response.h \
    parameterExplorer/param_base_class.h \
    parameterExplorer/param_display_widget.h \
    parameterExplorer/param_search_widget.h \
    parameterExplorer/param_plot_window.h
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
    parameterExplorer/parameterplotsubwindow.cpp \
    parameterExplorer/sharedparametersubwindow.cpp \
    eda_mds_space.cpp \
    metricmanager/eda_response.cpp \
    chart_mds.cpp \
    metricmanager/chart_response.cpp \
    parameterExplorer/param_base_class.cpp \
    parameterExplorer/param_display_widget.cpp \
    parameterExplorer/param_search_widget.cpp \
    parameterExplorer/param_plot_window.cpp
FORMS += \  
    mdsspacecreatordialog.ui \
    mdsspaceanalystframe.ui \
    mdsuncertainitysubwindow.ui \
    metricmanager/mdsmetricmanagerframe.ui \
    metricmanager/mdsmetricplotsubwindow.ui \
    parameterExplorer/mdsparameterexplorertabwidget.ui \
    parameterExplorer/parameterplotsubwindow.ui \
    parameterExplorer/sharedparametersubwindow.ui \
    parameterExplorer/param_display_widget.ui \
    parameterExplorer/param_search_widget.ui \
    parameterExplorer/param_plot_window.ui
RESOURCES += mds-gui.qrc
