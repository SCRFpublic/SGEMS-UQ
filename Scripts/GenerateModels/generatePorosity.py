import sgems
from random import random
from math import sqrt

# The output path of where to store the generated facies models
path = '/home/lewisli/Desktop/WCA/generatedModels/'

# Path where initial data is stored
initalDataDir = '/home/lewisli/Desktop/WCA/initialData/'

# Path where histograms are stored
histoPath = '/home/lewisli/Desktop/WCA/Scripts/1GenerateModels/Histograms/real_'

# Use sgsim for each facies for porosity
# Since we are not conditioning

def triRand(a, c, b):
    c = float(c)
    t = (c-a)/(b-a)
    y = sqrt(random())
    d = a if random() < t else b
    return d + (c-d) * y

nbfacies = 4
SaveRealName='Porosity.prj'
nx = '78'
ny = '59'
nz = '116'
seed = 14071789
MinValuePoro = [1.498e-07,0.002419,0.001699,0.04092]
MaxValuePoro = [0.23558,0.30323,0.33307,0.36032]
numReal = 1
numRealMaxMin = 0
varioMax = []
# Generate Variogram range
for i in range (0,numReal):
    varioMax.append(triRand(30,40,35))
    print varioMax[i]
    

# Number of histograms
numHisto = 10;

TI_for_Study = ["TI1","TI2","TI3","TI4","TI5","TI8","TI9","TI10","TI11","TI12","TI13"]  # TI used for the current study

sgems.execute("LoadObjectFromFile " + initalDataDir + "WellsDataPerFacies/WellData_facies0_Sgems.out::All")
sgems.execute("LoadObjectFromFile " + initalDataDir + "WellsDataPerFacies/WellData_facies1_Sgems.out::All")
sgems.execute("LoadObjectFromFile " + initalDataDir + "WellsDataPerFacies/WellData_facies2_Sgems.out::All")
sgems.execute("LoadObjectFromFile " + initalDataDir + "WellsDataPerFacies/WellData_facies3_Sgems.out::All")

# We have to clear well data of NaN
sgems.execute("ClearPropertyValueIf WellFacies0::Porosity::-996660000::-10000")
sgems.execute("ClearPropertyValueIf WellFacies1::Porosity::-996660000::-10000")
sgems.execute("ClearPropertyValueIf WellFacies2::Porosity::-996660000::-10000")
sgems.execute("ClearPropertyValueIf WellFacies3::Porosity::-996660000::-10000")

sgems.execute('NewCartesianGrid  poro::'+nx+'::'+ny+'::'+nz+'::1.0::1.0::1.0::0::0::0')
sgems.execute('NewCartesianGrid  temp::'+nx+'::'+ny+'::'+nz+'::1.0::1.0::1.0::0::0::0')

# Run algorithm for all facies

for k in range(0,numHisto):
    histogramFolder = histoPath + str(k+1) + '/'
    for i in range(0,nbfacies):
        count = 0
        histoName = histogramFolder + 'poro_hist' + str(i) + '.out'
        for j in range(0,numReal):
            rangeMax = varioMax[j]
            rangeMid = float(rangeMax)*26.0/35.0
            rangeLow = float(rangeMax)*8.6/35.0
            
            sgems.execute('RunGeostatAlgorithm  sgsim::/GeostatParamUtils/XML::<parameters>\
            <algorithm name="sgsim" />     \
            <Grid_Name  value="poro"  />     \
            <Property_Name  value="poro_facies'+str(i)+'" />    \
            <Nb_Realizations  value="1" />     \
            <Seed  value="'+str(seed)+'" />     \
            <Kriging_Type  value="Simple Kriging (SK)"  />     \
            <Trend  value="0 0 0 0 0 0 0 0 0 " />    \
            <Local_Mean_Property  value=""  />    \
            <Assign_Hard_Data  value="0"  />     \
            <Hard_Data  grid=""   property=""  />     \
            <Max_Conditioning_Data  value="16" />     \
            <Search_Ellipsoid  value="50 50 20  100 0 0" />    \
            <Use_Target_Histogram  value="1"  />     \
            <nonParamCdf  ref_on_file ="1"  ref_on_grid ="0"  break_ties ="0" filename ="'+histoName+'"   grid =""  property ="">  \
            <LTI_type  function ="No extrapolation"  extreme ="'+str(MinValuePoro[i])+'"  omega ="3" />  \
            <UTI_type  function ="No extrapolation"  extreme ="'+str(MaxValuePoro[i])+'"  omega ="0.333" />  \
            </nonParamCdf>    \
            <Variogram  nugget="0.01" structures_count="1"  >    \
            <structure_1  contribution="0.99"  type="Spherical"   >   \
            <ranges max="'+str(rangeMax)+'"  medium="'+str(rangeMid)+'"  min="'+str(rangeLow)+'"/>      \
            <angles x="100"  y="0"  z="0"   />    \
            </structure_1>  \
            </Variogram>  \
            </parameters> ') 
            sgems.execute('CopyProperty  poro::poro_facies'+str(i)+'__real0'+'::temp::PORO::0::0')
            sgems.execute('CopyProperty  poro::poro_facies'+str(i)+'__real0'+'::poro::poro_facies'+str(i)+'_'+str(k)+'_'+str(j)+'::0::0')
            sgems.execute('DeleteObjectProperties poro::poro_facies'+str(i)+'__real0')
            count = count + 1
            filename='PORO_Facies_'+str(i)+'_'+str(rangeMax)+'_' + str(k)+'_'+str(j)+'.out::gslib'    
            sgems.execute('SaveGeostatGrid  temp::'+path+'/Porosity/'+filename+'::0::PORO')

sgems.execute('SaveProject  ' + path + '/Porosity/PoroProjectFull.prj')

