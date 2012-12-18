from os import listdir
from os.path import isfile, join
import shutil, errno
import sgems


baseProjectName = '/home/lewisli/Desktop/WCA/generatedModels/FaciesModel/FaciesModel.prj'
sgems.execute('LoadProject ' + baseProjectName)

numFaciesModelperGrid = 10
grids = ['TI1']
numCopiesPerFacieModel = 10

for k in range(0,len(grids)):
    # First make copies of facies models
    for i in range(0,numFaciesModelperGrid):
        propertyName = grids[k] + '::NTG__real' + str(i)
        tempName = grids[k] + '::temp__real' + str(i)
        sgems.execute('CopyProperty ' + propertyName + '::' + tempName + '::0::0')
        
        # Delete original
        sgems.execute('DeleteObjectProperties '+ propertyName)
    
    for i in range(0,numFaciesModelperGrid):  
        tempName = grids[k] + '::temp__real' + str(i)  
        for j in range(0,numCopiesPerFacieModel):
            newID = k*numFaciesModelperGrid*numCopiesPerFacieModel + i*numCopiesPerFacieModel + j
            newPropertyName = grids[k] + '::NTG__real' + str(newID)
            
            print newPropertyName + ' from ' + tempName

            sgems.execute('CopyProperty ' + tempName + '::' + newPropertyName + '::0::0')
            
        # Delete temp
        sgems.execute('DeleteObjectProperties '+ tempName)
