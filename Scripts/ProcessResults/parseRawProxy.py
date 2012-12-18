from os import listdir
from os.path import isfile, join

# Script to load well data and convert it into a form that can be read by SGEMS

## ---------------- Parameters ---------------- ##

# Folder where unprocessed response data are stored
inputFolder = '/home/lewisli/Desktop/WCA/proxyResults/raw/'

# Folder to place parsed response data
outputFolder = '/home/lewisli/Desktop/WCA/proxyResults/parsed/'

# Name of wells on which we are reading data from
wellNames = ['3dsl.svs']

# Columns from which we are to read data from
#columnsOfInterest = [1,3,12,14]
columnsOfInterest = [1,3,10,12]

# Type of response data we are reading
dataFormat = ['Vector','TimeSeries','TimeSeries','TimeSeries']

# Number of lines to skip when reading data file
linesSkip = 5

# Realizations to Process
realizationsOfInterest = [0,2,5,7,9]
realizationGridNames = ['TI1', 'TI1', 'TI1','TI1','TI1']

# Alternatively use a loop to stipulate what we are reading from
realizationsOfInterest = []
realizationGridNames = []
histoStart = [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17]
histoGrid = ['TI1', 'TI1','TI1', 'TI1', 'TI1', 'TI1','TI1','TI1','TI1',
'TI1','TI2','TI2','TI2','TI2','TI2','TI2','TI2','TI2']
histoElem  = [0,1,2,3,4,5,6,7,8,9]

for i in range(0,len(histoStart)):
    for j in range(0,len(histoElem)):
        realizationsOfInterest.append(histoStart[i]*10+histoElem[j]+1)
        realizationGridNames.append(histoGrid[i])
        
## -------------------------------------------- ##



# Addition parameters: Do not edit
numWells = len(wellNames)
numColumnsOfInterest = len(columnsOfInterest)
numRealizationsofInterest = len(realizationsOfInterest)
minColLength = 10

# Before we read all the files, we read a sample file to get the names of the
# response variables we will be loading
filename = inputFolder + 'real' + str(1) + '/' + wellNames[0]
rawInput = open(filename)
# First 3 lines are useless
for k in range(0,linesSkip-1):
    line = rawInput.readline()
   
# Fourth line stores the names of each column   
columnNames = rawInput.readline()
columnNames = columnNames.split()


##Now we start reading all the other files and putting them into proper pose
for i in range(0,numWells):
   
    # Each response must have its own output file
    # The first column is always the time, hence not needed
    for j in range(1,numColumnsOfInterest):
       
        # Create a new XML file here
        responseName = columnNames[columnsOfInterest[j]]
        responseName = responseName.replace('/','_')
       
        outputName = outputFolder + responseName + "_Well_" + str(i) + \
                    ".metric.xml"
       
        fout = open(outputName, 'w')
       
        fout.write('<MetricResponses Name="'+responseName+'">\n')
        
        for k in range(0,numRealizationsofInterest):
            
            # Filename of the unprocessed response data that we will read
            inputFilename = inputFolder + 'real' + \
                            str(realizationsOfInterest[k]) + '/' + wellNames[i]
            
            rawInput = open(inputFilename)
            
            responseList = []
            timeList = []
            
            # First 5 lines are useless
            for l in range(0,linesSkip):
                line = rawInput.readline();
                
            # We only read the columns corresponding to the data in
            # columnsOfInterest
            while 1:
                line = rawInput.readline()
                if not line:
                    break
                else:
                    line = line.split()
                    if (len(line) < minColLength):
                        break
                    responseList.append(line[columnsOfInterest[j]-1])
                    timeList.append(line[0])
                    
                rawInput.close
                
                        
            # Parse into the xml format
            grid = realizationGridNames[k]
            property = "NTG__real" + str(realizationsOfInterest[k]-1)
            
            # Each response must have its own file, but we can put all the ones
            # with the same well in the same file
            fout.write('<MetricResponse grid="'+grid+
            '" format = "'+ dataFormat[j] + '" property="'+
            property+'" name = "'+responseName+'">\n')
             
            fout.write('<Data type = "float">')
             
            for l in range(0,len(responseList)-1):
                fout.write(responseList[l] + ';')
            
            fout.write(responseList[len(responseList)-1])
            fout.write('</Data>\n')   
            fout.write('<Time type = "float">')
                
            for l in range(0,len(responseList)-1):
                fout.write(timeList[l] + ';')
            
            fout.write(timeList[len(responseList)-1])
            fout.write('</Time>\n')  
            fout.write('</MetricResponse>\n')
            
        fout.write('</MetricResponses>')
            
        fout.close
            
##wellNames = ['3dsl.KU-A04.wel', '3dsl.KU-A08.wel',
##'3dsl.KU-C04.wel', '3dsl.KU-C09.wel']

#
#        fout.close
