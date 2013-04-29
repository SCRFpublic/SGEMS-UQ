/*
 * MetricData.cpp
 *
 *  Created on: Apr 1, 2011
 *      Author: aboucher
 */

#include <GsTLAppli/utils/manager.h>
#include <GsTLAppli/appli/manager_repository.h>
#include <GsTLAppli/grid/grid_model/geostat_grid.h>
#include <GsTLAppli/filters/distribution_filter.h>

#include "MetricData.h"
#include <cmath>
#include <QtCore/QDebug>
#include <QDomAttr>


MetricData::MetricData(const GsTLGridProperty* prop,QDomElement metaDataXml)
    :distance_kernel_(0),prop_(prop),grid_(0), metaDataXml_(metaDataXml)
{

  grid_ = dynamic_cast<const Geostat_grid*>(prop_->parent()->parent());
    QTextStream text;
    metaDataXml_.save(text,2);
    qDebug() << text.readAll();

    QDomElement dom = metaDataXml_.firstChildElement("algorithm");
    if( !dom.isNull() && dom.hasAttribute("name") )
    {
        QString name = dom.attribute("name");
        algoName_ = name.toStdString();
    }
    dom = metaDataXml_.firstChildElement("Name");

    if( !dom.isNull()  && dom.hasAttribute("value") )
    {
        QString name = dom.attribute("value");
        metricName_ = name.toStdString();
    }
}

MetricData::~MetricData()
{
    delete distance_kernel_;
}


// The distance between two metrics is to be handled by the metrics object
float MetricDataCollection::distance(const MetricDataCollection&
                                     mdataCollection) const
{
    if( this->size() != mdataCollection.size() )
        return -1;

    float d = 0.0;

    std::vector<MetricData*>::const_iterator it1 = this->begin();
    std::vector<MetricData*>::const_iterator it2 = mdataCollection.begin();
    for( ; it1 != this->end(); ++it1, ++it2) {
        d += (*it1)->distance( *it2 );
    }
    return d;

}

// sets kernel for each of the metric data in this collection
// The Metric Data own the kernel.
void MetricDataCollection::setKernel(Distance_kernel* distance_kernel) {
    std::vector<MetricData*>::const_iterator it = this->begin();
    for( ; it != this->end(); ++it) {
        (*it)->setKernel(distance_kernel);
    }
}

/*
Named_interface* 
MetricData::create_new_interface( std::string& s ) {
    // create appropriate class
    if (s.find("vector") != std::string::npos)
        return new MetricVectorData();
    else if (s.find("property") != std::string::npos)
        return new MetricPropertyData();
    else
        return 0;
}
*/


bool MetricData::initialize(QDomElement serializedXml){
  serializedDataXml_.clear();
  QDomElement elemRootData = serializedDataXml_.createElement("MetricResponse");
  serializedDataXml_.appendChild(serializedXml);

  QString mnames = serializedXml.attribute("name");
  QString mprop = serializedXml.attribute("property");
  QString mgrid = serializedXml.attribute("grid");
  QString mformat = serializedXml.attribute("format");
 
  metricName_ = mnames.toStdString();
  

  if(mformat.toStdString() != this->dataFormat()) return false;

  //Get the grid
    SmartPtr<Named_interface> grid_ni =
            Root::instance()->interface(gridModels_manager +
                                        "/" +
                                        mgrid.toStdString());

  grid_ = dynamic_cast<Geostat_grid*>(grid_ni.raw_ptr());
  if (grid_ == 0)
  {
      std::cerr << "Grid not found" << std::endl;
      std::cerr << "Grid requested was " <<  mgrid.toStdString() << std::endl;
      return false;

  }
  prop_ = grid_->property(mprop.toStdString());
  if (prop_ == 0)
  {
      std::cerr << "Property not found" << std::endl;
      std::cerr << "Property requested was " <<  mprop.toStdString() << std::endl;
      return false;

  }


  return true;
}





// Here we assume that we want the euclidean distance
// We should pass a functor for the distance (that could include weights)


Named_interface* MetricScalarData::create_new_interface( std::string& xml){
  QDomDocument doc;

  QString error;
  doc.setContent( QString::fromStdString(xml), &error );

  MetricScalarData* metric = new MetricScalarData();

  bool ok = true;

  if(error.isEmpty()) {
    ok = metric->initialize( doc.firstChildElement() );
  }

  if(ok) {
    return metric;
  }
  else {
    delete metric;
    return 0;
  }
}

float MetricScalarData::distance(const MetricData* metric) const{
    const MetricScalarData* smetric = dynamic_cast<const MetricScalarData*>(metric);
    if( smetric == 0 ) return -1.0f;
    if (!distance_kernel_) return -1.0f;

    return (*distance_kernel_)(this->data_ - smetric->data_, 2);
}


Named_interface* MetricVectorData::create_new_interface( std::string& xml){
  QDomDocument doc;

  QString error;
  doc.setContent( QString::fromStdString(xml), &error );

  MetricVectorData* metric = new MetricVectorData();

  bool ok= true;
  if(error.isEmpty()) {
    ok = metric->initialize( doc.firstChildElement() );
  }
  if(ok) {
    return metric;
  }
  else {
    delete metric;
    return 0;
  }
}


float MetricVectorData::distance(const MetricData* metric) const{
    const MetricVectorData* vmetric = dynamic_cast<const MetricVectorData*>(metric);
    if( vmetric == 0 ) return -1.0f;
    if (!distance_kernel_) return -1.0f;

    std::vector<float>::const_iterator it1 = data_.begin();
    std::vector<float>::const_iterator it2 = vmetric->data().begin();
    float e2 = 0.0;
    for( ; it1 != data_.end(); ++it1, ++it2) {
        e2 += (*distance_kernel_)(*it1, *it2);
    }
    return e2;
}


Named_interface* MetricTimeSeriesData::create_new_interface( std::string& xml){
  QDomDocument doc;

  QString error;
  doc.setContent( QString::fromStdString(xml), &error );

  MetricTimeSeriesData* metric = new MetricTimeSeriesData();

  bool ok = true;
  if(error.isEmpty()) {
    ok = metric->initialize( doc.firstChildElement() );
  }

  if(ok) {
    return metric;
  }
  else {
    delete metric;
    return 0;
  }
}


float MetricTimeSeriesData::distance(const MetricData *metric) const
{
    // Assume we are only going to be computing distances from other timeseries
    const MetricTimeSeriesData* tsmetric =
            dynamic_cast<const MetricTimeSeriesData*>(metric);

    if (tsmetric == 0)
        return -1.0f;
    if (!distance_kernel_)
        return -1.0f;

//    std::vector<float>::const_iterator it1 = this->data_.end()-1;
//    std::vector<float>::const_iterator it2 = tsmetric->data().end()-1;

    std::vector<float>::const_iterator it1 = this->data_.begin();
    std::vector<float>::const_iterator it2 = tsmetric->data().begin();


    float e2 = 0.0;
    for (; it1 != this->data_.end(); ++it1, ++it2)
    {
        e2 += (*distance_kernel_)(*it1,*it2);
    }

    return e2;
}

Named_interface* MetricPropertyData::create_new_interface( std::string& xml){
  QDomDocument doc;

  QString error;
  doc.setContent( QString::fromStdString(xml), &error );

  MetricPropertyData* metric = new MetricPropertyData();

  if(error.isEmpty()) {
    metric->initialize( doc.firstChildElement() );
  }

  return metric;
}


float MetricPropertyData::distance(const MetricData* metric) const{
    const MetricPropertyData* pmetric = dynamic_cast<const MetricPropertyData*>(metric);
    if( pmetric == 0 ) return -1;
    if (!distance_kernel_) return -1.0f;

    GsTLGridProperty::const_iterator it1 = data_prop_->begin();
    GsTLGridProperty::const_iterator it2 = pmetric->data()->begin();
    float e2 = 0.0;
    for( ; it1 != data_prop_->end(); ++it1, ++it2) {
        e2 += (*distance_kernel_)(*it1, *it2);
    }
    return e2/data_prop_->size();
}

MetricScalarData::MetricScalarData(const GsTLGridProperty* prop,
                                   QDomElement metaDataXml, float data )
    : MetricData(prop,metaDataXml), data_(data) {
    // serialize data
    serializedDataXml_.clear();
    QDomElement elemRootData = serializedDataXml_.createElement("MetricResponse");
    serializedDataXml_.appendChild(elemRootData);
    elemRootData.setAttribute("name",this->name().c_str());
    elemRootData.setAttribute("format","Scalar");
    elemRootData.setAttribute("property",prop->name().c_str());
    elemRootData.setAttribute("grid",grid_->name().c_str());

    QDomElement elemData = serializedDataXml_.createElement("Data");
    elemRootData.appendChild(elemData);
    elemData.setAttribute("type","float");
    
    QString buf = QString("%1").arg(this->data_);
    QDomText v_dom_text = serializedDataXml_.createTextNode(buf);
    elemData.appendChild(v_dom_text);
    //elemData.setNodeValue(buf);
   // QDomAttr attrData = serializedDataXml_.createAttribute("float");

 //   QString buf = QString("%1").arg(this->data_);
 //   qDebug()<<buf;
 //   attrData.setNodeValue(buf);
 //   elemRootData.setAttributeNode(attrData);
    //elemRootData.appendChild(attrData);
}


bool MetricScalarData::initialize(QDomElement serializedXml){

  bool ok = MetricData::initialize(serializedXml);
  if(!ok) return false;

 // QDomElement data_elem= serializedXml.firstChildElement("Data");
  data_ = serializedXml.firstChildElement("Data").text().toFloat();
  //data_ = serializedXml.attributeNode("float").nodeValue().toFloat();

  return true;

}


MetricVectorData::MetricVectorData(const GsTLGridProperty* prop,
                                   QDomElement metaDataXml,
                                   std::vector<float> data )
    : MetricData(prop,metaDataXml), data_(data)
{
    serializedDataXml_.clear();
    QDomElement elemRootData = serializedDataXml_.createElement("MetricResponse");
    serializedDataXml_.appendChild(elemRootData);
    elemRootData.setAttribute("name",this->name().c_str());
    elemRootData.setAttribute("format","Vector");
    elemRootData.setAttribute("property",prop->name().c_str());
    elemRootData.setAttribute("grid",grid_->name().c_str());

    QDomElement elemData = serializedDataXml_.createElement("Data");
    elemRootData.appendChild(elemData);
    elemData.setAttribute("type","float");
    
    std::vector<float>::iterator it = data_.begin();
    QStringList data_str;
    for( ; it != data_.end(); ++it) {
      data_str.append(  QString("%1").arg(*it) );
    }
    QDomText v_dom_text = serializedDataXml_.createTextNode(data_str.join(";"));
    elemData.appendChild(v_dom_text);


    QDomAttr attrData = serializedDataXml_.createAttribute("floats");
    attrData.setNodeValue(data_str.join(";"));
    elemRootData.setAttributeNode(attrData);
    //elemRootData.appendChild(attrData);
/*
    for( ; it != data_.end(); ++it)
    {
        QDomAttr attrData = serializedDataXml_.createAttribute("float");
        QString buf = QString("%1").arg(*it);
        attrData.setNodeValue(buf);
        elemRootData.appendChild(attrData);
    }
*/
};

bool MetricVectorData::initialize(QDomElement serializedXml){

  bool ok = MetricData::initialize(serializedXml);

  if(!ok) return false;

  //QDomElement data_elem= serializedXml.firstChildElement("Data");
  QStringList values = serializedXml.firstChildElement("Data").text().split(";");

  data_.reserve(values.size());
  for(int i=0; i<values.size(); ++i) {
    data_.push_back( values.at(i).toFloat() );
  }

  return true;
}


MetricTimeSeriesData::MetricTimeSeriesData(const GsTLGridProperty *prop,
                                           QDomElement metaDataXml,
                                           std::vector<float> data,
                                           std::vector<float> time) :
    MetricData(prop,metaDataXml), data_(data), time_(time)
{
    serializedDataXml_.clear();
    QDomElement elemRootData = serializedDataXml_.createElement("MetricResponse");
    serializedDataXml_.appendChild(elemRootData);
    elemRootData.setAttribute("name",this->name().c_str());
    elemRootData.setAttribute("format","TimeSeries");
    elemRootData.setAttribute("property",prop_->name().c_str());
    elemRootData.setAttribute("grid",grid_->name().c_str());

    std::vector<float>::iterator it = data_.begin();
    std::vector<float>::iterator it_time = time_.begin();
    QStringList data_str;
    QStringList time_str;
    for( ; it != data_.end(); ++it, ++it_time) {
      data_str.append(  QString("%1").arg(*it) );
      time_str.append(  QString("%1").arg(*it_time) );
    }

    QDomElement elemData = serializedDataXml_.createElement("Data");
    elemRootData.appendChild(elemData);
    elemData.setAttribute("type","float");
    QDomText v_dom_text = serializedDataXml_.createTextNode(data_str.join(";"));
    elemData.appendChild(v_dom_text);

    QDomElement elemTime = serializedDataXml_.createElement("Time");
    elemRootData.appendChild(elemTime);
    elemTime.setAttribute("type","float");
    QDomText t_dom_text = serializedDataXml_.createTextNode(time_str.join(";"));
    elemTime.appendChild(t_dom_text);

/*
    QDomAttr attrData = serializedDataXml_.createAttribute("floats");
    attrData.setNodeValue(data_str.join(";"));
    elemRootData.appendChild(attrData);
    QDomAttr attrTime = serializedDataXml_.createAttribute("time");
    attrTime.setNodeValue(time_str.join(";"));
    elemRootData.appendChild(attrTime);
*/
/*
    for( ; it != data_.end(); ++it)
    {
        QDomAttr attrData = serializedDataXml_.createAttribute("float");
        QString buf = QString("%1").arg(*it);
        attrData.setNodeValue(buf);
        elemRootData.appendChild(attrData);
    }
*/
}

bool MetricTimeSeriesData::initialize(QDomElement serializedXml){

  bool ok = MetricData::initialize(serializedXml);
  if(!ok) return false;
  
  QStringList values = serializedXml.firstChildElement("Data").text().split(";");
  data_.reserve(values.size());
  for(int i=0; i<values.size(); ++i) {
    data_.push_back( values.at(i).toFloat() );
  }

  values = serializedXml.firstChildElement("Time").text().split(";");
  time_.reserve(values.size());
  for(int i=0; i<values.size(); ++i) {
    time_.push_back( values.at(i).toFloat() );
  }

  return data_.size() == time_.size();

}



MetricPropertyData::MetricPropertyData(const GsTLGridProperty* prop,
                                       QDomElement metaDataXml,
                                       const GsTLGridProperty* data_prop)
    : MetricData(prop,metaDataXml), data_prop_(data_prop){
    // serialize data
    /*
 serializedDataXml_.clear();
 serializedDataXml_.setTagName("Data");
 */
};


bool MetricPropertyData::initialize(QDomElement serializedXml){
  return true;
}


/*
  ---------------------------------------------
*/


Named_interface* MetricContinuousDistributionData::create_new_interface( std::string& xml){
  QDomDocument doc;

  QString error;
  doc.setContent( QString::fromStdString(xml), &error );

  MetricContinuousDistributionData* metric = new MetricContinuousDistributionData();

  bool ok = true;
  if(error.isEmpty()) {
    ok = metric->initialize( doc.firstChildElement() );
  }

  if(ok) {
    return metric;
  }
  else {
    delete metric;
    return 0;
  }

}

MetricContinuousDistributionData::MetricContinuousDistributionData():dist_(0){}

MetricContinuousDistributionData::MetricContinuousDistributionData(const GsTLGridProperty* prop,
                     QDomElement metaDataXml, Continuous_distribution* dist){

  serializedDataXml_.clear();
  QDomElement elemRootData = serializedDataXml_.createElement("MetricResponse");
  serializedDataXml_.appendChild(elemRootData);
  elemRootData.setAttribute("name",this->name().c_str());
  elemRootData.setAttribute("format","TimeSeries");
  elemRootData.setAttribute("property",prop_->name().c_str());
  elemRootData.setAttribute("grid",grid_->name().c_str());

  QDomElement d_elem = Distribution_output_filter::serialize_distribution(dist, serializedDataXml_);
  elemRootData.appendChild(d_elem);

}


bool MetricContinuousDistributionData::initialize(QDomElement serializedXml){
  bool ok = MetricData::initialize(serializedXml);
  if(!ok) return false;

  QDomElement elem = serializedXml.firstChildElement("Distribution");
     
  if( !elem.hasAttribute("type") ) return 0;
  if (elem.attribute("type") == "Non-parametric")
    dist_ = this->initialize_non_parametric_distribution(elem);
  else
    dist_ = this->initialize_parametric_distribution(elem);

  return true;

}


//From IEEE TRANSACTIONS ON INFORMATION THEORY, VOL. 49, NO. 7, JULY 2003
// A New Metric for Probability Distributions
// Dominik M. Endres and Johannes E. Schindelin
float MetricContinuousDistributionData::distance(const MetricData* metric) const{
  const MetricContinuousDistributionData* dist_metric = dynamic_cast<const MetricContinuousDistributionData*>(metric);
  if( dist_metric==0 ) return -1;

  const Continuous_distribution* dist = dist_metric->distribution();

  double q1 = this->dist_->inverse(0.00001);
  double q2 =  dist->inverse(0.00001);
  double max1 = this->dist_->inverse(0.99999);
  double max2 = dist->inverse(0.99999);
  double inc1 = (max1-q1)/100;
  double inc2 = (max2-q2)/100;
  float d=0;
  for( ; q1<max1; q1+=inc1, q2+=inc2) {
    float p1 = this->dist_->prob(q1);
    float p2 = dist->prob(q2);

    d += p1*std::log(  2*p1/(p1+p2) ) + p2*std::log(  2*p2/(p1+p2) );
  }
  return d;


/*
  float min = std::min( this->dist_->inverse(0.00001),dist->inverse(0.00001) );
  float max = std::max( this->dist_->inverse(0.99999),dist->inverse(0.99999) );
  float inc = (max-min)/100;
  float d=0;
  for(float q = min; q<max; q+=inc) {
    float p1 = this->dist_->prob(q);
    float p2 = dist->prob(q);

    d += p1*std::log(  2*p1/(p1+p2) ) + p2*std::log(  2*p2/(p1+p2) );
  }
  return d;
*/
}



Continuous_distribution* MetricContinuousDistributionData::initialize_parametric_distribution(const QDomElement& elem){

  QString type = elem.attribute("type");
  if( type.isEmpty() ) return 0;

  // Not registered at first to ensure the correct parameter were provided
  Named_interface* ni = Root::instance()->new_interface_raw(type.toStdString(),continuous_distributions_manager+"/");
  Parametric_continuous_distribution* p_dist = dynamic_cast<Parametric_continuous_distribution*>(ni);
  if(p_dist == 0) return 0;

  std::vector<std::string> pnames = p_dist->parameter_names();
  std::vector<float> params;
  bool ok = true;
  for(int i=0; i<pnames.size(); ++i) {
    //QString param_str = elem.attribute("name");
    float param = elem.attribute(pnames[i].c_str()).toFloat(&ok);
    if(ok) {
      params.push_back(param);
    }
  }
  if(params.size() != pnames.size() ) return 0;

  p_dist->initialize(params);
  return p_dist;
}

Continuous_distribution* MetricContinuousDistributionData::initialize_non_parametric_distribution(const QDomElement& elem ){

  QString type = elem.attribute("type");

  if( type.isEmpty() || type != "Non-parametric") return 0;

  QDomElement p_elem = elem.firstChildElement("Cdf");
  QDomElement q_elem = elem.firstChildElement("Quantiles");
  if(p_elem.isNull()  || q_elem.isNull()) return 0;

  QStringList p_list = p_elem.firstChild().toText().data().split(";");
  QStringList q_list = q_elem.firstChild().toText().data().split(";");
  if(q_list.size() != p_list.size() ) return 0;

  std::vector<double> p;
  std::vector<double> q;
  p.reserve(q_list.size());
  q.reserve(q_list.size());
  for(int i=0; i< q_list.size() ; ++i) {
    p.push_back(p_list.at(i).toDouble());
    q.push_back(q_list.at(i).toDouble());
  }


  Named_interface* ni = Root::instance()->new_interface_raw("Non-parametric",
                                    continuous_distributions_manager+"/");

  Non_parametric_distribution* np_dist = dynamic_cast<Non_parametric_distribution*>(ni);
  if(np_dist == 0) return 0;
  np_dist->initialize(q.begin(), q.end(), p.begin());


  QDomElement lti_elem = elem.firstChildElement("LowerTail");
  QString lti_type = lti_elem.attribute("type");
  if(lti_type == "No Extrapolation") {
    np_dist->lower_tail_interpolator( Tail_interpolator( new No_TI()) );
  }
  else if(lti_type == "Power-Lower") {
    float min = lti_elem.attribute("min").toFloat();
    float omega = lti_elem.attribute("omega").toFloat();
    np_dist->lower_tail_interpolator( Tail_interpolator( new Power_LTI(min, omega)) );
  }
  else  if(lti_type == "Exponential-Lower") {
    np_dist->lower_tail_interpolator( Tail_interpolator( new Exponential_LTI()) );
  }


  QDomElement uti_elem = elem.firstChildElement("UpperTail");
  QString uti_type = uti_elem.attribute("type");
  if(uti_type == "No Extrapolation") {
    np_dist->upper_tail_interpolator( Tail_interpolator( new No_TI()) );
  }
  else if(uti_type == "Power-Upper") {
    float max = uti_elem.attribute("max").toFloat();
    float omega = uti_elem.attribute("omega").toFloat();
    np_dist->upper_tail_interpolator( Tail_interpolator( new Power_UTI(max, omega)) );
  }
  else  if(uti_type == "Hyperbolic-Upper") {
    float omega = uti_elem.attribute("omega").toFloat();
    np_dist->upper_tail_interpolator( Tail_interpolator( new Hyperbolic_UTI(omega)) );
  }

  return np_dist;

}
