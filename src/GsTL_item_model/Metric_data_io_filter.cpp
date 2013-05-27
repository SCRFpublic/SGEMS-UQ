#include "Metric_data_io_filter.h"

#include <MetricDataManager.h>
#include <metric_manager_repository.h>
#include <mds_space_filters.h>
#include <utils/manager.h>
#include <appli/manager_repository.h>
#include <grid/geostat_grid.h>
#include <QDomDocument>
#include <QDir>

Metric_data_input_filter::Metric_data_input_filter(void){}
Metric_data_input_filter::~Metric_data_input_filter(void){}


Named_interface* Metric_data_input_filter::create_new_interface(std::string&){
    return new Metric_data_input_filter;
}


bool Metric_data_input_filter::can_handle( const std::string& filename )
{
    // First Parse The XML file
    QDomDocument doc("ExternalResponses");

    QFile file(QString::fromStdString(filename));

    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }
    if (!doc.setContent(&file))
    {
        file.close();
        return false;
    }

    QString tagname = doc.firstChildElement().tagName();
    return tagname == "MetricResponses" || tagname == "MetricResponse";
}


Named_interface* Metric_data_input_filter::read( const std::string& filename, 
                                                 std::string* errors )
{

    SmartPtr<Named_interface> ni =
            Root::instance()->interface( metricData_manager + "/metricRegistrar");

    MetricDataManager* md_manager = dynamic_cast<MetricDataManager*>(ni.raw_ptr());

    if(md_manager==0)
    {
        qDebug() << "Couldn't find md_manager";
        return false;
    }
    QDomDocument doc("ExternalResponses");

    QFile file(QString::fromStdString(filename));

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open file";
        return false;
    }
    if (!doc.setContent(&file))
    {
        qDebug() << "Could not set content";
        file.close();
        return false;
    }

    QDomElement root = doc.firstChildElement();
    QString tagname = root.tagName();
    if ( tagname == "MetricResponses" )
    {
        QDomElement m_elem = root.firstChildElement();
        for(  ; !m_elem.isNull() ; m_elem = m_elem.nextSiblingElement() )
        {
            MetricData* mdata = this->load_metric(m_elem);
            if(mdata==0)
            {
                qDebug() << "Load Metric failed in " <<
                            QString::fromStdString(filename);
                continue;
            }
            md_manager->registerMetricData(mdata->grid()->name(),
                                           mdata->property()->name(), mdata );
        }

    }
    else if(tagname == "MetricResponse")
    {
        QDomElement m_elem = doc.firstChildElement();
        MetricData* mdata = this->load_metric(m_elem);
        md_manager->registerMetricData(mdata->property()->name(), mdata->grid()->name(), mdata );
    }
}

MetricData* Metric_data_input_filter::load_metric(QDomElement
                                                  serialMetricElement)
{

    QString format = serialMetricElement.attribute("format");

    QString xml;
    QTextStream stream(&xml);
    serialMetricElement.save(stream,0);



    Named_interface* ni =
            Root::instance()->new_interface_raw( format.toStdString()+
                                                 "://"+xml.toStdString(),
                                                 metricData_manager + "/" );

    return dynamic_cast<MetricData*>(ni);

}

/*
  -----------------------------------------
*/


Named_interface* Metric_data_output_filter::create_new_interface(std::string&){
    return new Metric_data_output_filter;
}

Metric_data_output_filter::Metric_data_output_filter(void){}
Metric_data_output_filter::~Metric_data_output_filter(void){}



bool Metric_data_output_filter::write( std::string outfile, const Named_interface* ni, 
                                       std::string* errors  )
{

    const MetricDataManager* md_manager = dynamic_cast<const MetricDataManager*>(ni);

    if(md_manager==0) return false;

    QDomDocument doc = md_manager->getMetricDomDocument();
    QDomElement elem = doc.firstChildElement();
    for( ; !elem.isNull(); elem = elem.nextSiblingElement()) {

        QString metric_name = elem.tagName();
        QString filename = QString("%1/%2.metric.xml").arg(outfile.c_str()).arg(metric_name);
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            return false;
        }

        QDomDocument dom;
        QDomElement root_metric_out_elem = dom.createElement("MetricResponses");
        root_metric_out_elem.setAttribute("Name",metric_name);
        dom.appendChild(root_metric_out_elem);

        //Loop over the grid name
        QDomElement elem_grid = elem.firstChildElement();
        for( ; !elem_grid.isNull(); elem_grid = elem_grid.nextSiblingElement()) {
            QString grid_name = elem_grid.tagName();

            QDomElement elem_prop = elem_grid.firstChildElement();
            for( ; !elem_prop.isNull(); elem_prop = elem_prop.nextSiblingElement()) {
                QString prop_name = elem_prop.tagName();
                const MetricData* mdata = md_manager->getMetricData(grid_name.toStdString(), prop_name.toStdString(), metric_name.toStdString());
                const QDomElement serialized_data = mdata->serializedDataXml();
                root_metric_out_elem.appendChild(serialized_data);
            }
        }
        QTextStream xmlStream (&file);
        xmlStream << dom.toString();
        file.close();
    }


    return true;

}



/*
  ------------------------------------------------
*/


Named_interface* Metric_data_project_output_filter::create_new_interface(std::string&){
    return new Metric_data_project_output_filter;
}
Metric_data_project_output_filter::Metric_data_project_output_filter(void){}
Metric_data_project_output_filter::~Metric_data_project_output_filter(void){}

bool Metric_data_project_output_filter::write( std::string outfile, const Named_interface* ni, 
                                               std::string* errors){


    const MetricDataManager* md_manager = dynamic_cast<const MetricDataManager*>(ni);

    if(md_manager==0) return false;

    if(md_manager->empty()) return true;


    // Create the folder that will contains both the metric data and the mds_space

    QString dirname = outfile.c_str();
    if( !dirname.endsWith( ".metrics" ) && !dirname.endsWith( ".metrics/" ) )
        dirname.append( ".metrics" );

    QDir dir(dirname);
    if (dir.exists()) {
        //	  bool ok = dir.rmdir(dirname);
        bool ok = this->removeDir(dirname);
        if( !ok ) {
            errors->append( "can't overwrite directory: " + dirname.toStdString() );
            return false;
        }

    }

    dir.mkdir(dirname);

    //write the metric data
    Metric_data_output_filter metric_out_filter;
    //bool ok = metric_out_filter.write(dirname.toStdString()+"/data.metrics.xml",md_manager,errors);
    bool ok = metric_out_filter.write(dirname.toStdString(),md_manager,errors);
    if(!ok) {
        return false;
    }

    //Loop over all the mds space and write them to disk.
    SmartPtr<Named_interface> dir_mds_ni = Root::instance()->interface(mds_manager);
    Manager* dir_mds = dynamic_cast<Manager*>( dir_mds_ni.raw_ptr() );
    if(dir_mds==0) return true;

    //Get all the object in the directory
    Manager::interface_iterator it_object = dir_mds->begin_interfaces();
    for( ; it_object != dir_mds->end_interfaces(); ++it_object ) {
        Mds_space_output_filter mds_out_filter;
        ok = mds_out_filter.write(dirname.toStdString()+"/"+it_object->raw_ptr()->name()+".mds.xml", it_object->raw_ptr(),errors);
    }

    return errors->empty();

}


bool Metric_data_project_output_filter::removeDir(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }

    return result;
}


/*
  ----------------------------------------------------
  */
Named_interface* Metric_data_project_input_filter::create_new_interface(std::string&){
    return new Metric_data_project_input_filter;
}


Metric_data_project_input_filter::Metric_data_project_input_filter(void){}
Metric_data_project_input_filter::~Metric_data_project_input_filter(void){}


bool Metric_data_project_input_filter::can_handle( const std::string& filename ){
    QDir dir(filename.c_str());

    bool isDir = dir.exists();
    if(!isDir ) return false;

    if(dir.absolutePath().endsWith(".metrics")) return true;
    else return false;
    /*
 QString outFilename = dir.absolutePath()+"/data.metrics.xml";

 QFile file( outFilename );
  if( !file.open( QIODevice::ReadOnly ) ) {
   return false;
  }
  return true;
  */
}

Named_interface* Metric_data_project_input_filter::read( const std::string& filename, 
                                                         std::string* errors )
{
    QDir dir(filename.c_str());



    QStringList filenames = dir.entryList();

    //first loop over the filename that can be loaded with the metrics filter
    Metric_data_input_filter mdata_input_filter;
    for( int i=0; i<filenames.size(); ++i )
    {
        std::string filename = dir.absolutePath().toStdString()+"/"+
                filenames.at(i).toStdString();

        if( !mdata_input_filter.can_handle(filename))
        {
            // qDebug() << "Can't handle " << QString::fromStdString(filename);
            continue;
        }
        Named_interface* ni = mdata_input_filter.read(filename,errors);
        {
            if (ni == 0)
            {
                return 0;
            }
        }
    }

    /*
  std::string mdata_filename = dir.absolutePath().toStdString()+"/data.metrics.xml";
  Metric_data_input_filter mdata_input_filter;
  bool ok = mdata_input_filter.can_handle(mdata_filename);
  if(!ok) return 0;

  Named_interface* ni = mdata_input_filter.read(mdata_filename,errors);
  if(ni==0) return 0;
  */
    //Load all the mds-space in the directory

    //  QStringList filenames = dir.entryList();
    Mds_space_input_filter mds_input_filter;
    for( int i=0; i<filenames.size(); ++i ) {
        std::string filename = dir.absolutePath().toStdString()+"/"+filenames.at(i).toStdString();
        if( !mds_input_filter.can_handle(filename  )  ) continue;
        mds_input_filter.read(filename, errors);
    }

    return 0;

}
