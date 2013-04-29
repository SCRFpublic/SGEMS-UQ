/*
 * MeetricData.h
 *
 *  Created on: Apr 1, 2011
 *      Author: aboucher
 */

#ifndef METRICDATA_H_
#define METRICDATA_H_

#include "GenericKernels.h"
#include "common.h"

#include <GsTLAppli/grid/grid_model/geostat_grid.h>
#include <GsTLAppli/grid/grid_model/grid_property.h>
#include <GsTLAppli/math/continuous_distribution.h>
#include <GsTLAppli/math/non_parametric_distribution.h>

#include <QtXml/QtXml>

using namespace std;

// Class that store the metrics of a GsTLGridProperty

// Problem to be solved
// When the data is a GsTLGridPropertyGrid we should not have to

class MetricData;
class ITEM_MODEL_DECL MetricDataCollection {
public :

    MetricDataCollection(){}

    virtual ~MetricDataCollection(){}

    void addMetricData(MetricData* mdata){mdata_.push_back(mdata);}

    MetricData* getMetricData(int index){return mdata_[index];}
    int size() const {return mdata_.size();}

    bool empty() const { return mdata_.empty(); }

    std::vector<MetricData*>::iterator begin(){return mdata_.begin();}
    std::vector<MetricData*>::iterator end(){return mdata_.end();}

    std::vector<MetricData*>::const_iterator begin() const {return mdata_.begin();}
    std::vector<MetricData*>::const_iterator end() const {return mdata_.end();}

    MetricData* metric(int i){return mdata_[i];}

    // The distance between two metrics is to be handled by the metrics object
    float distance(const MetricDataCollection& mdataCollection) const;

    // set kernels
    void setKernel(Distance_kernel* distance_kernel);

    // TODO: make accessor function
    const GsTLGridProperty* prop_;

protected :

    std::vector<MetricData*> mdata_;

};

class ITEM_MODEL_DECL MetricData : public Named_interface {
public :
//    static Named_interface* create_new_interface( std::string& );
    MetricData():distance_kernel_(0){}
    MetricData(const GsTLGridProperty* prop,QDomElement metaDataXml);

    virtual bool initialize(QDomElement serializedXml);

    virtual ~MetricData();
    virtual std::string classname() const { return "MetricData"; }
    virtual std::string name() const { return metricName_; }
    virtual std::string algoName() const { return algoName_; }
    virtual std::string dataFormat() const =0;

    const QDomElement& metaDataXml(){return metaDataXml_;}
    const QDomElement& serializedDataXml() const{
        return serializedDataXml_.firstChildElement ();}

    // The distance between two metrics is to be handled by the metrics object
    virtual float distance(const MetricData* metric) const { return 0.0f; }

    virtual int size() const=0;
    virtual float data(int index) const=0;

    void setKernel(Distance_kernel* distance_kernel) {
        distance_kernel_ = distance_kernel; }

    const GsTLGridProperty* property() const {return prop_;}
    const Geostat_grid* grid() const {return grid_;}

    //GsTL_object_item
    virtual QString item_name() const {return metricName_.c_str();}
    virtual GsTL_object_item *child(int row) {
        return static_cast<GsTL_object_item *>(0);}
    virtual int childCount() const {return 0;}
    virtual int columnCount() const {return 2;}
    virtual QVariant item_data(int column) const {
        if(column == 0) return this->item_name();
        else if(column == 1) return this->item_type();
        else return QVariant();
    }

    //ToDO: make accessor func
        const GsTLGridProperty* prop_;

protected :

  const Geostat_grid* grid_;

    QDomElement metaDataXml_;
    QDomDocument serializedDataXml_;
    std::string metricName_;
    std::string algoName_;


    Distance_kernel *distance_kernel_;

    //	std::vector<float> data_;



};


class ITEM_MODEL_DECL MetricScalarData : public MetricData {
public :

    static Named_interface* create_new_interface( std::string& );

    MetricScalarData(){}
    MetricScalarData(const GsTLGridProperty* prop,
                     QDomElement metaDataXml, float data );

    virtual bool initialize(QDomElement serializedXml);

    virtual ~MetricScalarData(){}
    virtual std::string classname() const { return "MetricScalarData"; }
    virtual std::string dataFormat() const { return "Scalar"; }

    virtual float data(int index) const {return data_;}
    virtual int size() const {return 1;}
    void setData(float data) { data_ = data; }

    // The distance between two metrics is to be handled by the metrics object
    virtual float distance(const MetricData* metric) const;

    // GsTL_object_item
    virtual QString item_type() const {return "Metric Scalar";}


private :
    //	const GsTLGridProperty* prop_;
    //	QDomElement metaDataXml;

    float data_;

};


class ITEM_MODEL_DECL MetricVectorData : public MetricData {
public :
    
  static Named_interface* create_new_interface( std::string& );
    
    MetricVectorData(){}
    MetricVectorData(const GsTLGridProperty* prop,
                     QDomElement metaDataXml, std::vector<float> data );

    virtual ~MetricVectorData(){}

    bool initialize(QDomElement serializedXml);

    virtual std::string classname() const { return "MetricVectorData"; }
    virtual std::string dataFormat() const { return "Vector"; }

    const std::vector<float>& data() const {return data_;}

    virtual float data(int index) const {return data_[index];}
    virtual int size() const {return data_.size();}


    void setData(std::vector<float> &data) { data_ = data;}
    //	const QDomElement& metaData(){return metaData_;}

    // The distance between two metrics is to be handled by the metrics object
    virtual float distance(const MetricData* metric) const;

    // GsTL_object_item
    virtual QString item_type() const {return "Metric Vector";}


private :
    //	const GsTLGridProperty* prop_;
    //	QDomElement metaDataXml;

    std::vector<float> data_;

};

//Any property per default should be used as a metric
// this class should simply use as a wrapper when a property is selected
class ITEM_MODEL_DECL MetricPropertyData : public MetricData {
public :

  static Named_interface* create_new_interface( std::string& );

    MetricPropertyData(){}
    MetricPropertyData(const GsTLGridProperty* prop,
                       QDomElement metaDataXml, const GsTLGridProperty* data_prop);

    virtual ~MetricPropertyData(){}
    virtual std::string classname() const { return "MetricPropertyData"; }
    virtual std::string dataFormat() const { return "Property"; }

    bool initialize(QDomElement serializedXml);

    const GsTLGridProperty* data() const {return data_prop_;}
    virtual float data(int index) const {return data_prop_->get_value(index);}
    virtual int size() const {return data_prop_->size();}

    //	const QDomElement& metaData(){return metaData_;}

    // The distance between two metrics is to be handled by the metrics object
    virtual float distance(const MetricData* metric) const;

    // GsTL_object_item
    virtual QString item_type() const {return "Metric Property";}


private :
    const GsTLGridProperty* data_prop_;
    //	const GsTLGridProperty* prop_;
    //	QDomElement metaDataXml;

};

class ITEM_MODEL_DECL MetricTimeSeriesData : public MetricData {
public :

  static Named_interface* create_new_interface( std::string& );

    MetricTimeSeriesData(){}
    MetricTimeSeriesData(const GsTLGridProperty* prop,
                     QDomElement metaDataXml, std::vector<float> data,
                     std::vector<float> time);

    virtual ~MetricTimeSeriesData(){}

    virtual std::string classname() const { return "MetricTimeSeriesData"; }
    virtual std::string dataFormat() const { return "TimeSeries"; }

    bool initialize(QDomElement serializedXml);

    const std::vector<float>& data() const {return data_;}
    const std::vector<float>& time() const {return time_;}

    virtual float data(int index) const {return data_[index];}
    virtual float time(int index) const {return time_[index];}
    virtual int size() const {return data_.size();}

    void setData(std::vector<float> &data) { data_ = data;}
    void setTime(std::vector<float> &time) { time_ = time;}

    // The distance between two metrics is to be handled by the metrics object
    virtual float distance(const MetricData* metric) const;

    // GsTL_object_item
    virtual QString item_type() const {return "Metric TimeSeries";}


private :
    //	const GsTLGridProperty* prop_;
    //	QDomElement metaDataXml;

    std::vector<float> data_;
    std::vector<float> time_;

};


class ITEM_MODEL_DECL MetricContinuousDistributionData : public MetricData {
public :

  static Named_interface* create_new_interface( std::string& );

    MetricContinuousDistributionData();
    MetricContinuousDistributionData(const GsTLGridProperty* prop,
                     QDomElement metaDataXml, Continuous_distribution* dist);

    virtual ~MetricContinuousDistributionData(){ delete dist_;}

    virtual std::string classname() const { return "MetricContinuousDistributionData"; }
    virtual std::string dataFormat() const { return "ContinuousDistribution"; }

    bool initialize(QDomElement serializedXml);

    const Continuous_distribution* distribution() const{return dist_;}

    virtual float data(int index) const {return 0;}
    virtual int size() const {return 1;}


    //Takes ownership of the distribution
    void distribution(Continuous_distribution* dist) {dist_ = dist; }

    // The distance between two metrics is to be handled by the metrics object
    virtual float distance(const MetricData* metric) const;

    // GsTL_object_item
    virtual QString item_type() const {return "Metric ContinuousDistribution";}


private:
  Continuous_distribution* initialize_non_parametric_distribution(const QDomElement& elem );
  Continuous_distribution* initialize_parametric_distribution(const QDomElement& elem );

private :
  Continuous_distribution* dist_;

};


class ITEM_MODEL_DECL MetricDataGridItem : public GsTL_object_item
{
public:
    MetricDataGridItem();


};

// A dirty dirty hack
class ITEM_MODEL_DECL MetricDataPropertyItem : public GsTL_object_item
{
public:
    MetricDataPropertyItem();
    MetricDataPropertyItem(MetricData *metricData, GsTL_object_item* parent)
        : GsTL_object_item(parent), metricData_(metricData){}

    virtual void* data_pointer(){return static_cast<void*>(metricData_);}

    bool initialize(QDomElement serializedXml);

    MetricData* metricData(){return metricData_;}
    void metricData(MetricData *metricData) {metricData_= metricData;}

    // GsTL_object_item
    virtual QString item_type() const {return metricData_->item_type();}

    // There are no children
    virtual GsTL_object_item *child(int row)
    {
        return static_cast<GsTL_object_item *>(0);
    }

    virtual int childCount() const {return 0;}
    virtual int columnCount() const {return 2;}

    virtual QVariant item_data(int column) const
    {
        // First column we will display the name
        if (column == 0)
        {
            return this->metricData_->prop_->item_name();
        }
        else if (column == 1)
        {
            return QString::fromStdString(this->metricData_->algoName());
        }
        else
            return QVariant();
    }


private :
    MetricData* metricData_;
    QString gridName_;
};


#endif /* METRICDATA_H_ */
