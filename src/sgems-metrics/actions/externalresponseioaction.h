#ifndef EXTERNALRESPONSEIOACTION_H
#define EXTERNALRESPONSEIOACTION_H

#include <ml_library_constants.h>

#include <metric_manager_repository.h>

#include <GsTLAppli/actions/common.h>
#include <GsTLAppli/actions/action.h>
#include <GsTLAppli/actions/unary_action.h>

#include <GsTLAppli/actions/defines.h>
#include <GsTLAppli/utils/gstl_messages.h>
#include <GsTLAppli/utils/string_manipulation.h>
#include <GsTLAppli/utils/error_messages_handler.h>
#include <GsTLAppli/utils/string_manipulation.h>
#include <GsTLAppli/grid/grid_model/geostat_grid.h>
#include <GsTLAppli/appli/manager_repository.h>
#include <GsTLAppli/appli/project.h>
#include <GsTL/utils/smartptr.h>

#include <QtXml>
#include <filters/externalresponseinputfilter.h>

#define READ 0
#define WRITE 1


class METRICSACTIONS_DECL ExternalResponseIOAction : public Action
{
public:
    ExternalResponseIOAction();

public:
  static Named_interface* create_new_interface( std::string& );

public:
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors );
  virtual bool initFromDom(QDomDocument *parameters, std::string &filename);

  virtual bool exec( void );

protected :
  //QDomDocument *configurationParameters;
  bool mode;

};

#endif // EXTERNALRESPONSEIOACTION_H
