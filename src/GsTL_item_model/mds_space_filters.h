
#ifndef __GSTLAPPLI_MDSSPACE_FILTER_H__ 
#define __GSTLAPPLI_MDSSPACE_FILTER_H__ 

#include <MetricData.h> 

#include <filters/filter.h>
#include <utils/named_interface.h> 

#include <QDomElement>
//Save the 

class Mds_space_input_filter : public Input_filter
{
public:

  static Named_interface* create_new_interface(std::string&);


  Mds_space_input_filter(void);
  virtual ~Mds_space_input_filter(void);

  virtual std::string filter_name() const {return "Mds_space";} 
  virtual std::string file_extensions() const { return "*.xml"; }

  virtual bool can_handle( const std::string& filename );
  virtual Named_interface* read( const std::string& filename, 
                              std::string* errors = 0 );

private :

  MetricData* load_metric(QDomElement serialMetricElement);

};


class Mds_space_output_filter : public Output_filter
{
public:

  static Named_interface* create_new_interface(std::string&);
  Mds_space_output_filter(void);
  virtual ~Mds_space_output_filter(void);

  virtual std::string filter_name() const {return "Mds_space";}
  virtual std::string file_extensions() const { return "*.xml"; }  
  virtual bool write( std::string outfile, const Named_interface* ni, 
                      std::string* errors = 0 );

  virtual std::string type_data() const {return "MdsSpace";}
  virtual std::string name() const {return "MdsSpace";}

};

#endif

