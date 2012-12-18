from os import listdir
from os.path import isfile, join
import numpy as np

# Script to compute all the histogram means/variances
folderPath = '/home/lewisli/code-dev/research/Histograms/real_'

for i in range(1,11):
    folderName = folderPath + str(i) + '/'
    
    onlyfiles = [ f for f in listdir(folderName) if isfile(join(folderName,f)) ]
    onlyfiles.sort()
    
    print onlyfiles
    
    # Write it to an output file
    outputPath = folderName + 'stats.txt'
    outputFile = open(outputPath,'w')
    
    for j in range(0,len(onlyfiles)):
        filePath = folderName + onlyfiles[j]
        
        if (onlyfiles[j]!= 'stats.txt'):
            rawInput = open(filePath)
        else:
            continue
        
        col = []
        
        while 1:
            line = rawInput.readline()
            
            if not line:
                break
            else:
                col.append(float(line))
                
        
        print filePath
        print str(np.mean(col))
        
        outputFile.write(onlyfiles[j] + ' std ' + str(np.std(col)) + '\n')
        outputFile.write(onlyfiles[j] + ' mean ' + str(np.mean(col)) + '\n')

# First we load all ten histogram results for each facies model
##resultFolder = '/home/lewisli/Desktop/3dsl_allresults/'
##outputFolder = '/home/lewisli/Desktop/Test/Results/'
##numWells = 1
##numHistograms = 5
##histoStart = [0,3,8,10,15]