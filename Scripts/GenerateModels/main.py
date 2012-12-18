from cookieCut import *
import os
from os import listdir
from os.path import isfile, join

mypath = '/home/lewisli/code-dev/research/Generated/FaciesModel/'
onlyfiles = [ f for f in listdir(mypath) if isfile(join(mypath,f)) ]

propertyFolder1 = '/home/lewisli/code-dev/research/Generated/Properties/Porosity/'
propertyFolder2 = '/home/lewisli/code-dev/research/Generated/Properties/Permeability/'

realFolders = ['Set1', 'Set2', 'Set3', 'Set4', 'Set5', 'Set6', 'Set7', 'Set8', 'Set9', 'Set10'] 

outputPath = '/home/lewisli/code-dev/research/Generated/Models/'



# For each facie model we will run both porosity
for i in range(len(onlyfiles)):
    dir = outputPath + onlyfiles[i]
    
    # Generate a folder for each facies model
    if not os.path.exists(dir):
        os.makedirs(dir)

    # For each model, we have porosity and permeability
    for j in range(len(realFolders)):
        # For each facies model, we have to generate both porosity and permeability
        cookieCut(mypath, onlyfiles[i], propertyFolder1+realFolders[j], "Poro", j)
        cookieCut(mypath, onlyfiles[i], propertyFolder2+realFolders[j], "Perm", j)
    
