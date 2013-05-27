#ifndef __GSTLAPPLI_METRICDATA_ACTIONS_H__ 
#define __GSTLAPPLI_METRICDATA_ACTIONS_H__ 


#include <MetricData.h> 
#include <Metric_data_io_filter.h> 

#include <actions/action.h>
#include <utils/named_interface.h> 
#include <utils/error_messages_handler.h>


class MultiDimScalingSpace;

class Load_data_metric_action :  public Action
{
public:

  static Named_interface* create_new_interface(std::string&);

  Load_data_metric_action(void){}
  virtual ~Load_data_metric_action(void){}

  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec(); 

private :

  std::string filename_;

};

class Save_data_metric_action :  public Action
{
public:

  static Named_interface* create_new_interface(std::string&);

  Save_data_metric_action(void){}
  virtual ~Save_data_metric_action(void){}

  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec(); 

private :
  std::string filename_;
  std::string mname_;


};


class Load_mds_space_action :  public Action
{
public:

  static Named_interface* create_new_interface(std::string&);

  Load_mds_space_action(void){}
  virtual ~Load_mds_space_action(void){}

  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec(); 

private :

  std::string filename_;

};


class Save_mds_space_action :  public Action
{
public:

  static Named_interface* create_new_interface(std::string&);

  Save_mds_space_action(void){}
  virtual ~Save_mds_space_action(void){}

  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec(); 

private :

  std::string filename_;
  std::string mds_space_name_;
  MultiDimScalingSpace* mds_space_;

};


#endif