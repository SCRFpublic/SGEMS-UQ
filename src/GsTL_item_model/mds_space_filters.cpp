#include "mds_space_filters.h"


#include "Metric_data_io_filter.h"
#include <multi_dim_scaling_space.h>

#include <MetricDataManager.h>
#include <metric_manager_repository.h>

#include <utils/manager.h>
#include <appli/manager_repository.h>
#include <grid/geostat_grid.h>
#include <QDomDocument>
#include <QDomElement>

Mds_space_input_filter::Mds_space_input_filter(void){}
Mds_space_input_filter::~Mds_space_input_filter(void){}


Named_interface* Mds_space_input_filter::create_new_interface(std::string&){
  return new Mds_space_input_filter;
}


bool Mds_space_input_filter::can_handle( const std::string& filename ) { 
    // First Parse The XML file
    QDomDocument doc("MDSParameters");

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

    QDomElement root = doc.documentElement();

    return (!root.firstChildElement("MetricData").isNull()) &&
            (!root.firstChildElement("Spacename").isNull());
}


Named_interface* Mds_space_input_filter::read( const std::string& filename, 
                              std::string* errors ){



  QFile file(filename.c_str());
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
          return false;
  }

  QDomDocument doc("MDS");
  doc.setContent(&file);

	std::vector<MultiDimScalingSpace::grid_prop_pairT> props_grid;

	SmartPtr<Named_interface> ni =
			Root::instance()->interface( metricData_manager + "/metricRegistrar");

	MetricDataManager* mDataRegistrar = dynamic_cast<MetricDataManager*>(ni.raw_ptr());

	// Get root element
	QDomElement root = doc.firstChildElement("MDS");

	// Read kernel type
	QDomElement elemKernel = root.firstChildElement("Kernel");
	std::string kernel_type = elemKernel.attribute("type").toStdString();

	// Read space name
	QDomElement spaceName = root.firstChildElement("Spacename");
	std::string space_name = spaceName.attribute("name").toStdString();

	// Read metric data
	QDomElement elemMetricData = root.firstChildElement("MetricData");
	QDomElement elemMetric = elemMetricData.firstChildElement("Metric");
	std::vector<std::string> mdata_names; // vector of metric names
	for( ; !elemMetric.isNull(); elemMetric = elemMetric.nextSiblingElement("Metric")  ) {
		std::string metric_name = elemMetric.attribute("name").toStdString();
		mdata_names.push_back(metric_name);
	}

  std::vector<MetricDataCollection> mdataCollections;

	// Read Grid
	QDomElement elem = root.firstChildElement("Grid");

	for( ; !elem.isNull(); elem = elem.nextSiblingElement("Grid")  )
	{
		std::string grid_name = elem.attribute("name").toStdString();
		QDomElement gridProps = elem.firstChildElement("Property");
		for (; !gridProps.isNull(); gridProps = gridProps.nextSiblingElement("Property"))
		{
			std::string property_name = gridProps.attribute("name").toStdString();
			props_grid.push_back(MultiDimScalingSpace::grid_prop_pairT(grid_name,property_name));
			MetricDataCollection mdataCollection =
					mDataRegistrar->getMetricDataCollection(grid_name, property_name, mdata_names);

                        if(mdataCollection.empty()) {
				*errors = "Some metric data for the property "+property_name+ " on the grid "+grid_name+" could not be found";
				return false;
			}
			mdataCollections.push_back(mdataCollection);
		}
	}

	int num_inputs = mdataCollections.size();

	// set kernels
	SmartPtr<Named_interface> kernel_ni =
			Root::instance()->new_interface( kernel_type, kernel_manager + "/" );

	if (!kernel_ni.raw_ptr()) {
		*errors = "Kernel "+kernel_type+" could not be found";
		return 0;
	}
	Distance_kernel* kernel = dynamic_cast<Distance_kernel*>(kernel_ni.raw_ptr());
	if (!kernel) {
		*errors = "Kernel "+kernel_type+" has an invalid kernel type for operating on the given data";
		return 0;
	}
	for (int i =0; i < num_inputs; i++) {
		mdataCollections[i].setKernel(kernel->clone());
	}

	// Parse cluster information
	vector<MDSUtil::clusterBlock> kmeansResults;

	QDomElement cluster = root.firstChildElement("Clusters");
	QDomElement clusterUsed = cluster.firstChildElement("ClustersUsed");
	for (;!clusterUsed.isNull(); clusterUsed = clusterUsed.nextSiblingElement("ClustersUsed") )
	{
		// Top level of blocks
		MDSUtil::clusterBlock currentBlock;
		currentBlock.numBlock = clusterUsed.attribute("num").toInt();

		QDomElement clusterID = clusterUsed.firstChildElement("ClusterID");
		for (; !clusterID.isNull(); clusterID = clusterID.nextSiblingElement("ClusterID"))
		{
			std::string currentClustNo = clusterID.attribute("ID").toStdString();
			QDomElement property = clusterID.firstChildElement("Property");
			MDSUtil::cluster currentCluster;

			for (; !property.isNull(); property = property.nextSiblingElement("Property"))
			{
				pair<QString, int> currentProperty;
				currentProperty.first = property.attribute("name");
				currentProperty.second = property.attribute("num").toInt();
				currentCluster.components.push_back(currentProperty);
			}

			currentCluster.numComponents = currentCluster.components.size();

			currentBlock.block.push_back(currentCluster);
		}

		kmeansResults.push_back(currentBlock);
	}

  if(!errors->empty()) return 0;

	ni = Root::instance()->new_interface( "multi_dim_scaling",mds_manager + "/" + space_name);

	MultiDimScalingSpace*  mds_space = dynamic_cast<MultiDimScalingSpace*>(ni.raw_ptr());

	bool ok = mds_space->initialize(space_name,props_grid,mdata_names);
  if(!ok) return 0;

	mds_space->setKMeanResults(kmeansResults);

  return mds_space;


}

/*
  -----------------------------------------
*/


Named_interface* Mds_space_output_filter::create_new_interface(std::string&){
  return new Mds_space_output_filter;
}

Mds_space_output_filter::Mds_space_output_filter(void){}
Mds_space_output_filter::~Mds_space_output_filter(void){}

bool Mds_space_output_filter::write( std::string outfile,
                                     const Named_interface* ni,
                      std::string* errors  ){

  QFile file(outfile.c_str());
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
          return false;
  }

  const MultiDimScalingSpace* mds_space = dynamic_cast<const MultiDimScalingSpace*>(ni);

  if(mds_space==0) return false;


  // Generate new XML Document
  QDomDocument configurationParameter("MDSParameters");

  // Create Root Element
  QDomElement root = configurationParameter.createElement("MDS");
  configurationParameter.appendChild(root);

  // Write grid name: Here is a problem we can't handle multiple grid names
  QDomElement grid = configurationParameter.createElement("Grid");
  grid.setAttribute("name","grid");

  // Write grid elements
  for (int i = 0;
        i < mds_space->pointCount(); i++)
  {
      QDomElement prop = configurationParameter.createElement("Property");
      prop.setAttribute("name",QString(mds_space->getPropertyName(i).
                                        c_str()) );
      grid.appendChild(prop);
  }

  // Get metric names
  std::vector<std::string> metricName = mds_space->getMetricNames();
  // Write Metric Tag
  QDomElement metric = configurationParameter.createElement("MetricData");

  // Write metric
  for (int i = 0; i < metricName.size(); i++)
  {
      QDomElement met = configurationParameter.createElement("Metric");
      met.setAttribute("name",QString(metricName.at(i).c_str()));
      metric.appendChild(met);
  }

  // Write Kernel Type
  QDomElement kernelType = configurationParameter.createElement("Kernel");
  kernelType.setAttribute("type", QString("Euclidean"));

  // Write space name
  QDomElement spaceName = configurationParameter.createElement("Spacename");
  spaceName.setAttribute("name", mds_space->name().c_str());

  // Write cluster information
  const std::vector< MDSUtil::clusterBlock> *kmeanResults =
          mds_space->getKmeans();

  QDomElement clusterResult=configurationParameter.createElement("Clusters");

  // For the first level of cluster result we will write the clusters kmeans
  for (unsigned int i = 0 ; i < kmeanResults->size(); i++)
  {
      QString numstring;
      numstring.append(QString("%1").arg(kmeanResults->at(i).numBlock));

      QDomElement clustersUsed =
              configurationParameter.createElement("ClustersUsed");
      clustersUsed.setAttribute("num",numstring);

      // Second level we iterate through each code block
      for (unsigned int j = 0; j < kmeanResults->at(i).numBlock; ++j)
      {

          QDomElement clustersID =
                  configurationParameter.createElement("ClusterID");
          numstring.clear();
          numstring.append(QString("%1").arg(j));
          clustersID.setAttribute("ID",numstring);

          // Third level we iterate through each cluster and put all the point
          unsigned int numPtsInCluster = kmeanResults->at(i).
                  block.at(j).numComponents;

          for (unsigned int k = 0; k < numPtsInCluster; ++k)
          {
              QDomElement pointID =
                      configurationParameter.createElement("Property");
              pointID.setAttribute("name", kmeanResults->at(i).block.at(j).
                                    components.at(k).first);
              pointID.setAttribute("num",kmeanResults->at(i).block.at(j).
                                    components.at(k).second);
              clustersID.appendChild(pointID);
          }

          clustersUsed.appendChild(clustersID);
      }
      clusterResult.appendChild(clustersUsed);
  }

  // Put under root
  root.appendChild(kernelType);
  root.appendChild(grid);
  root.appendChild(metric);
  root.appendChild(clusterResult);
  root.appendChild(spaceName);

  QTextStream xmlStream (&file);
  xmlStream << configurationParameter.toString();
  file.close();
  return true;

}


