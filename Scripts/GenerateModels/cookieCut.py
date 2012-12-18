from os import listdir
from os.path import isfile, join
def cookieCut(faciesPath, faciesName, propertyPath, propertyName, setNum, outputPath):

    # Load the facies
    # We assume that each folder will only have 4 files in it
    onlyfiles = [ f for f in listdir(propertyPath) if isfile(join(propertyPath,f)) ]

    # For some reason, onlyfiles isn't necessarily sorted.
    onlyfiles.sort()

    Facies0 = open(propertyPath + '/' + onlyfiles[0],'r')
    Facies1 = open(propertyPath + '/' + onlyfiles[1],'r')
    Facies2 = open(propertyPath + '/' + onlyfiles[2],'r')
    Facies3 = open(propertyPath + '/' + onlyfiles[3],'r')
    

    print onlyfiles[0]
    print onlyfiles[1]
    print onlyfiles[2]
    print onlyfiles[3]
    print '--------------'
    # Read Facie data
    allLines0 = Facies3.readlines()
    allLines1 = Facies2.readlines()
    allLines2 = Facies1.readlines()
    allLines3 = Facies0.readlines()
    
    count = 0
    
    MPS = open(faciesPath+'/'+faciesName);
    allLinesMPS = MPS.readlines()
    outputFileStr = outputPath +faciesName + '/' + propertyName + '_Real' + str(setNum) + '_Sgems.out'
    fid = open(outputFileStr,'w');
    
    fid.write('temp (78x59x116)'+'\n')
    fid.write('1'+'\n')
    fid.write(propertyName + '\n')
    
    for l in range(3,len(allLinesMPS)):
        if allLinesMPS[l].strip() == '1':
               fid.write(allLines1[l])
        elif allLinesMPS[l].strip() == '2':
               fid.write(allLines2[l])
        elif allLinesMPS[l].strip() == '3':
               fid.write(allLines3[l])
        else:
               fid.write(allLines0[l])
               
    fid.close()
    MPS.close()
    
        
