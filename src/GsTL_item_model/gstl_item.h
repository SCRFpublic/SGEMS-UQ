#ifndef GSTL_ITEM_H
#define GSTL_ITEM_H

#include "common.h"
#include "MetricData.h"
#include "MetricDataManager.h"
#include "common.h"

#include <appli/project.h>
#include <grid/geostat_grid.h>

#include <QList>
#include <QVariant>

class ITEM_MODEL_DECL GsTL_item
{
public:
    GsTL_item();
    GsTL_item(GsTL_item* parentItem):parentItem_(parentItem){}

    virtual ~GsTL_item(){}

    virtual GsTL_item *child(int row) = 0;
    virtual int childCount() const=0;
    virtual int columnCount() const=0;
    virtual QVariant data(int column) const=0;
    virtual int row() const=0;
    GsTL_item *parent(){return parentItem_;}

protected:
    MetricDataManager* gridDataManager() const;

protected:
    GsTL_item *parentItem_;

};


class ITEM_MODEL_DECL GsTL_root_item : public GsTL_item
{
public:
    GsTL_root_item():GsTL_item(0){}

    GsTL_root_item(GsTL_project* proj);
    virtual ~GsTL_root_item();

    virtual GsTL_item *child(int row);
    virtual int childCount() const;
    virtual int columnCount() const;
    virtual QVariant data(int column) const;
    virtual int row() const;
 //   virtual GsTL_item *parent(){return 0;}

private:
    GsTL_project* proj_;

};


class ITEM_MODEL_DECL GsTL_grid_item : public GsTL_item
{
public:
    GsTL_grid_item();

    GsTL_grid_item(GsTL_project* proj, Geostat_grid* grid, QString name, GsTL_item *parent);
    virtual ~GsTL_grid_item();

    virtual GsTL_item *child(int row);
    virtual int childCount() const;
    virtual int columnCount() const;
    virtual QVariant data(int column) const;
    virtual int row() const;
//    virtual GsTL_item *parent();

    Geostat_grid* grid(){return grid_;}
    QString gridName(){return gridName_;}

private:
    Geostat_grid* grid_;
    QString gridName_;
    GsTL_project* proj_;

};




class ITEM_MODEL_DECL GsTL_property_item : public GsTL_item
{
public:
    GsTL_property_item();

    GsTL_property_item(Grid_continuous_property* prop, GsTL_item *parent = 0);
    virtual ~GsTL_property_item();


    virtual GsTL_item *child(int row);
    virtual int childCount() const;
    virtual int columnCount() const;
    virtual QVariant data(int column) const;
    virtual int row() const;
 //   virtual GsTL_item *parent();

private:
    Grid_continuous_property* prop_;
    QString gridName_;
};


class ITEM_MODEL_DECL GsTL_metric_item : public GsTL_item
{
public:
    GsTL_metric_item();

    GsTL_metric_item(QString gridName, QString propName, QString metricName, GsTL_item *parent = 0);
    virtual ~GsTL_metric_item();

    virtual GsTL_item *child(int row);
    virtual int childCount() const;
    virtual int columnCount() const;
    virtual QVariant data(int column) const;
    virtual int row() const;
//    virtual GsTL_item *parent();

private:
    QString gridName_;
    QString propName_;
    QString metricName_;

    MetricData* metric_;
};

#endif // GSTL_ITEM_H
