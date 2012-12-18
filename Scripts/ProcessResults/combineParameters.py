from os import listdir
from os.path import isfile, join
import copy

from xml.dom import minidom
from xml.etree import ElementTree
import xml.etree.ElementTree as ET
from xml.etree.ElementTree import Element, SubElement, Comment

def prettify(elem):
    """Return a pretty-printed XML string for the Element.
    """
    rough_string = ElementTree.tostring(elem, 'utf-8')
    reparsed = minidom.parseString(rough_string)
    return reparsed.toprettyxml(indent=" ")


# Script to parse multiple parameter files into a single project

# The path to the project containing the facies models
baseParameterProj = '/home/lewisli/Desktop/WCA/generatedModels/FaciesModel/FaciesModel.prj/'

# The path to the projects containing porosity and permeability data
inputFolders = ['/home/lewisli/Desktop/WCA/generatedModels/Porosity/PoroProjectFull.prj/poro.grid/',
                '/home/lewisli/Desktop/WCA/generatedModels/Permeability/PermProj.prj/perm.grid/']
   
# Name of the properties we are copying parmeters from             
inputFoldersNames = ['porosity', 'permeability']

# Path to where we are storing the combined project
outputFolder = '/home/lewisli/Desktop/WCA/generatedProjects/WCA.prj/'

# Realizations to Process
realizationsOfInterest = []
realizationGridNames = []

# Names of grid where facie models are stored
#gridNames = ['TI1', 'TI2','TI3']
gridNames = ['TI1']
# Number of realizations generated per training image
facieModelRealizations = 10

# Number of porosity/perm realizations
permPoroRealizations = 10

# Number of facies
numFacies = 4

histoStart = [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14]
histoGrid = ['TI1', 'TI1','TI1', 'TI1', 'TI1', 'TI2','TI2','TI2','TI2','TI2','TI3','TI3','TI3','TI3','TI3']
histoElem  = [0,1,2,3,4,5,6,7,8,9]

for i in range(0,len(histoStart)):
    for j in range(0,len(histoElem)):
        realizationsOfInterest.append(histoStart[i]*10+histoElem[j]+1)
        realizationGridNames.append(histoGrid[i])
        
## -------------------------------------------- ##

# Addition parameters: Do not edit
numRealizationsofInterest = len(realizationsOfInterest)
minColLength = 10

numAlgorithms = len(inputFolders)
fileIdentifier = 'grid_geometry.xml'


# Open the porosity and permeability files
# Open all subfiles
subParameterNodes = []
for j in range(0,len(inputFoldersNames)):
    subParameterFilename = inputFolders[j] + fileIdentifier
    subParameterFile = ET.parse(subParameterFilename)
    subParameterNode = subParameterFile.find('Properties')
    subParameterNodes.append(subParameterNode)

# Iterate over each grid
for i in range(0,len(gridNames)):
    
    # Generate an output file for each grid
    outputFilename = outputFolder + gridNames[i] + '.grid/' + fileIdentifier
    out = open(outputFilename,'w')
    out.write('<!DOCTYPE gridGeometry>')
    
    # Get the path to the grid where we will be looking for our facie models
    baseParameterFilename = baseParameterProj + gridNames[i] + '.grid/' + fileIdentifier
    
    # Open the file
    # Read base parameter file
    baseFile = ET.parse(baseParameterFilename)
        
    # Read the Parameter of grid
    ParameterNode = baseFile.find('Properties')
    OutputParameterNode = Element('Properties')
    
    # Open all subfiles
    subParameterFilename = inputFolders[0] + fileIdentifier
    subParameterFile = ET.parse(subParameterFilename)
    subParameterNode = subParameterFile.find('Properties')
    
    # Copy the Geometry of grid
    top = Element('Grid')
    top.set('type',"Cgrid")
    top.set('name',gridNames[i])
    GeometryNode = baseFile.find('Geometry')    
    top.append(GeometryNode)

    facieList = ParameterNode.findall('GsTLGridProperty')
    propertyGroupString = ''
    for propID in range(0,facieModelRealizations):
        for j in range(0,permPoroRealizations):
            # We need permPoroRealizations per facie model
            Property = copy.deepcopy(facieList[propID])
    
            # Get the facie realization name
            propertyNo = i*permPoroRealizations*facieModelRealizations+propID*permPoroRealizations + j
            propertyName = "NTG__real" + str(propertyNo)
            Property.set('name',propertyName)
            Property.set('filepath', 'properties/property__' + propertyName + '.sgems')
            propertyGroupString += propertyName+';'

            # For each new model, we find the corresponding parameters in the 
            # facie model list
            newParameterText = Property.get('parameters')
            newParameterText = newParameterText.replace('</parameters>','')

            # Iterate over porosity and perm to find their parameters
            for k in range(0,len(inputFoldersNames)):
                # Create a node for the property
                newParameterText += '<' + inputFoldersNames[k] + '>'
                
                # Iterate over facies
                for l in range(0,numFacies):
                    propertyRealName = inputFoldersNames[k] + '_' + str(j) + '_' + str(l)
                    
                    # Search over the subproperty nodes
                    for subProperty in subParameterNodes[k]:
                        # Find corresponding object
                        if subProperty.get('name') == propertyRealName: 
                            # Read parameters associated with poro/perm         
                            subParameterString = subProperty.get('parameters')
                            subParameterString = subParameterString.replace('<parameters>','<facie'+str(l) + '>')
                            subParameterString = subParameterString.replace('/parameters>','</facie'+str(l) + '>')
                            newParameterText += subParameterString
                            break
                    
                newParameterText += '</' + inputFoldersNames[k] + '>'
            newParameterText += '</parameters>'
            #print newParameterText
            Property.set('parameters',newParameterText)
            

            OutputParameterNode.append(copy.deepcopy(Property))    

    # Append the parameters
    top.append(OutputParameterNode)
 
    # Read the Region
    RegionNode = baseFile.find('Regions')
    top.append(RegionNode)

    # Read the GsTLGridPropertyGroup
    propertyGroupString  = propertyGroupString[:len(propertyGroupString)-1]
    PropertyGroupsNode = baseFile.find('PropertyGroups')
    PropertyGroupsNode[0].set('memberProperties',propertyGroupString)
    top.append(PropertyGroupsNode)

    # Read the Categorical Definintions
    CategoricalDefinitionsNode = baseFile.find('CategoricalDefinitions')
    top.append(CategoricalDefinitionsNode)
    
    # Output string
    outputString = prettify(top);
    outputString = outputString.replace('<?xml version="1.0" ?>','')
    
    # Output the pased XML file
    out.write(outputString)
