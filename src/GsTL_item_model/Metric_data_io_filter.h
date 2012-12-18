
#ifndef __GSTLAPPLI_METRICDATA_FILTER_H__ 
#define __GSTLAPPLI_METRICDATA_FILTER_H__ 

#include <MetricData.h> 
#include <common.h>

#include <GsTLAppli/filters/filter.h>
#include <GsTLAppli/utils/named_interface.h> 


#include <QDomElement>
//Save the 

class ITEM_MODEL_DECL Metric_data_input_filter : public Input_filter
{
public:

  static Named_interface* create_new_interface(std::string&);


  Metric_data_input_filter(void);
  virtual ~Metric_data_input_filter(void);

  virtual std::string filter_name() const {return "Metric_data";} 
  virtual std::string file_extensions() const { return "*.xml"; }

  virtual bool can_handle( const std::string& filename );
  virtual Named_interface* read( const std::string& filename, 
                              std::string* errors = 0 );

private :

  MetricData* load_metric(QDomElement serialMetricElement);

};


class ITEM_MODEL_DECL Metric_data_output_filter : public Output_filter
{
public:

  static Named_interface* create_new_interface(std::string&);
  Metric_data_output_filter(void);
  virtual ~Metric_data_output_filter(void);

  virtual std::string filter_name() const {return "Metric_project_data";}
  virtual std::string file_extensions() const { return "*.xml"; }  
  virtual bool write( std::string outfile, const Named_interface* ni, 
                      std::string* errors = 0 );

  virtual std::string type_data() const {return "MetricDataProject";}
  virtual std::string name() const {return "MetricDataProject";}

};



class ITEM_MODEL_DECL Metric_data_project_input_filter : public Input_filter
{
public:

  static Named_interface* create_new_interface(std::string&);


  Metric_data_project_input_filter(void);
  virtual ~Metric_data_project_input_filter(void);

  virtual std::string filter_name() const {return "Metric_project_data";} 
  virtual std::string file_extensions() const { return "*.xml"; }

  virtual bool can_handle( const std::string& filename );
  virtual Named_interface* read( const std::string& filename, 
                              std::string* errors = 0 );

private :


};


class ITEM_MODEL_DECL Metric_data_project_output_filter : public Output_filter
{
public:

  static Named_interface* create_new_interface(std::string&);
  Metric_data_project_output_filter(void);
  virtual ~Metric_data_project_output_filter(void);

  virtual std::string filter_name() const {return "Metric_data";}
  virtual std::string file_extensions() const { return "*.xml"; }  
  virtual bool write( std::string outfile, const Named_interface* ni, 
                      std::string* errors = 0 );

  virtual std::string type_data() const {return "MetricData";}
  virtual std::string name() const {return "MetricData";}

private :
  bool removeDir(const QString &dirName);

};



#endif

