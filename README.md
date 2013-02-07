SGEMS-UQ
=======

1. SGEMS-UQ Compilation Instruction

SGEMS-UQ requires the installation of SGEMS3.0. SGEMS-UQ has no further dependencies and can be compiled using 
qmake (or Visual Studio with Qt Add-in http://doc.qt.digia.com/vs-add-in/index.html). The order in which compilation 
must be completed is: 1. GsTL_item_model 2. sgems-metrics 3. mds-gui. The resulting shared libraries (or .dll files) must 
be placed in the sgems plugins/actions folder. Once SGEMS-UQ has been installed, the main SGEMS program can be started, 
and SGEMS-UQ can be started using Data Analysis -> Data Explorer (or simply Control-E). 

The latest version of sgems is hosted here: https://github.com/ar2tech/ar2tech-SGeMS-public
SGEMS-UQ required the same external libraries than SGeMS.

Windows specific instructions:
 - Compile SGeMS from the git repository (see the readme file for instructions).
 - Open the solution metrics-sgems.sln
 - Build the solutions (The projects use the same env. variables for paths as SGeMS)
 - Be sure that the path to the $(AR2TECH_SGEMS_DIR)/plugins/Geostat is added in the PATH environment variable.

2. Data Generation

The (Scripts->GenerateModels->Main.py) script can be used to generate appropriate facies, permeability and porosity values. 
The appropriate initial data and histograms can be found under InitialData. The results of the simulations are given in ProxyResults->raw. 
Scripts->ProcessResults->parseRawProxy.py is used to parse appropriate responses of interest into the SGEMS-UQ XML format. 
Likewise, the (Scripts->ProcessResults->copyFacieProjects.py and combineParameters.py) scripts are used to generate the appropriate SGEMS project.

3. Using SGEMS-UQ

In our particular example, the WCA.prj project can be loaded by simply dragging into the main SGEMS window. 
The SGEMS-UQ window can be initialized using Data Analysis -> Data Explorer (or simply Control-E).

a) Plotting Response Data
Under the SGEMS-UQ window selecting Response->View Responses brings up available grids, responses and realizations. 
Selecting any subset of the realizations and pressing plot will create a new window with the appropriate response plot.

b) Creating MDS Space
Responses->MDS Space shows a list of existing MDS spaces. Generating a new space can be done using the Create button. 
Once again available grids, realizations and responses are shown. Selecting an appropriate distance kernel for 
each response will generate the appropriate MDS Space.

c) Visualizing MDS Space
Selecting the desired MDS space and pressing Plot will create a new window with the visualization of the MDS space in 3D. 
Within the window k-means clustering can be performed in addition to parameter exploration.