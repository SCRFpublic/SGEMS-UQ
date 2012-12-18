#include "externalresponseioaction.h"

ExternalResponseIOAction::ExternalResponseIOAction()
{
}

Named_interface* ExternalResponseIOAction::create_new_interface( std::string&)
{

    return new ExternalResponseIOAction;

}

bool ExternalResponseIOAction::init(std::string& parameters, GsTL_project* proj,
                                  Error_messages_handler* errors)
{

    // Parse parameter to see if we are writing or reading
    if (parameters == "Write")
    {
        this->mode = WRITE;
        return true;
    }
    else if (parameters == "Read")
    {
        this->mode = READ;
        return true;
    }
    else
    {
        return false;
    }
}

bool ExternalResponseIOAction::exec()
{
    return true;
}

bool ExternalResponseIOAction::initFromDom(QDomDocument *parameters,
                                           std::string &filename)
{

    if (this->mode == READ)
    {
        SmartPtr<Named_interface> ni =
                Root::instance()->interface(externalResponseFilter_manager + "/"
                                            + ExternalResponseInputFilter
                                            ::filtername() );
        ExternalResponseInputFilter* filter =
                dynamic_cast<ExternalResponseInputFilter*>( ni.raw_ptr() );
        filter->loadParameters(parameters, filename);
        filter->exec();
    }

//    else if (this->mode == WRITE)
//    {
//        SmartPtr<Named_interface> ni =
//                Root::instance()->interface(MDSUncertFilter_manager + "/" + MDSUncertaintySpaceOutputFilter::filtername() );
//        MDSUncertaintySpaceOutputFilter* filter = dynamic_cast<MDSUncertaintySpaceOutputFilter*>( ni.raw_ptr() );
//        filter->loadParameters(parameters, filename);
//        filter->exec();
//    }

    return true;
}
