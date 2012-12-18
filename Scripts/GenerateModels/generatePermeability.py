import sgems
from random import random
from math import sqrt
from math import tanh

# The output path of where to store the generated permeability models
path = '/home/lewisli/Desktop/WCA/generatedModels'
# Path where histogram data is stored
histogramFolder = '/home/lewisli/Desktop/WCA/Scripts/1GenerateModels/Histograms/real_'
# Path where initial data is stored
initalDataDir = '/home/lewisli/Desktop/WCA/initialData/'


# Use sgsim for each facies for vshale
# Since we are not conditioning

def triRand(a, c, b):
    c = float(c)
    t = (c-a)/(b-a)
    y = sqrt(random())
    d = a if random() < t else b
    return d + (c-d) * y


sgems.execute('LoadProject  /home/lewisli/Desktop/WCA/generatedModels/Porosity/PoroProjectFull.prj')

print 'here'

nbfacies = 4
SaveRealName='VShalesity.prj'
nx = '78'
ny = '59'
nz = '116'
seed = 14071789

MinValueVShale = [0.20736,0.040977,0.032918,4.0396e-06]
MaxValueVShale = [1.0001,0.97252,0.947399,0.882]

MinValuePoro = [1.4e-07,0.002,0.001,0.04]
MaxValuePoro = [0.2199,0.304,0.334,0.3604]

MinValuePerm = []
MaxValuePerm = []

# Convert Vshale min/max values to perm
b = 3
c = 1.22544413
d = 2.2

numReal = 1
numRealMaxMin = 3
varioMax = []

# Generate Variogram range (for test case we will use these ranges)
varioMax.append(31.1226962639)
varioMax.append(36.3891077478)
varioMax.append(36.6972324935)
varioMax.append(36.9167218124)
varioMax.append(39.2613411318)


sgems.execute('NewCartesianGrid  perm::'+nx+'::'+ny+'::'+nz+'::1.0::1.0::1.0::0::0::0')
count = 0
numHisto = 10

# Run algorithm for all facies
for k in range(0,numHisto):

    for i in range(0,nbfacies):
        
        histoName = histogramFolder + str(k+1) + '/perm_hist' + str(i) + '.out'
        print histoName
        for j in range(0,numReal):
            rangeMax = varioMax[j]
            rangeMid = float(rangeMax)*26.0/35.0
            rangeLow = float(rangeMax)*8.6/35.0
            
            poroName = 'poro_facies'+str(i)+'_' + str(k)+'_'+str(j)

            sgems.execute('RunGeostatAlgorithm  cosgsim::/GeostatParamUtils/XML::\
            <parameters>  <algorithm name="cosgsim" />     \
            <Grid_Name  value="poro"  />     \
            <Property_Name  value="perm_facies'+str(i)+'" />     \
            <Nb_Realizations  value="1" />     <Seed  value="'+str(seed)+'" /> \
            <Kriging_Type  value="Simple Kriging (SK)"  />     \
            <Cokriging_Type  value="Markov Model 1"  />     \
            <Primary_Harddata_Grid  value="" region="" />    \
            <Primary_Variable  value=""  />     \
            <Assign_Hard_Data  value="0"  />   \
            <Max_Conditioning_Data_1  value="16" />   \
            <Search_Ellipsoid_1  value="50 50 25  100 0 0" />   \
            <AdvancedSearch  use_advanced_search="0"></AdvancedSearch> \
            <Transform_Primary_Variable  value="1"  />    \
            <nonParamCdf_primary  ref_in_distribution ="0" ref_on_file ="1"  ref_on_grid ="0"  \
            break_ties ="0" filename ="'+histoName+'"   grid =""  \
            property ="">  \
            <LTI_type  function ="No extrapolation"  \
            extreme ="0.20736"  omega ="3" />  \
            <UTI_type  function ="No extrapolation"  \
            extreme ="1.0001"  omega ="0.333" /> \
            </nonParamCdf_primary>    \
            <Secondary_Harddata_Grid  value="poro"  region="" />   \
            <Secondary_Variable  value="'+poroName+'"  />   \
            <Max_Conditioning_Data_2  value="16" /> \
            <Search_Ellipsoid_2  value="50 50 25 100 0 0" /> \
            <AdvancedSearch_2  use_advanced_search="0"></AdvancedSearch_2> \
            <Transform_Secondary_Variable  value="1"  />     \
            <nonParamCdf_secondary  ref_in_distribution ="0" ref_on_file ="0"  ref_on_grid ="1"  \
            break_ties ="0" filename =""   grid ="poro"  \
            property ="'+poroName+'">  \
            <LTI_type  function ="No extrapolation"  \
            extreme ="0"  omega ="3" />  \
            <UTI_type  function ="No extrapolation"  extreme ="0" \
            omega ="0.333" />  </nonParamCdf_secondary>  \
            <Variogram_C11  nugget="0.01" structures_count="1"  >    \
            <structure_1  contribution="0.99"  type="Spherical"   >      \
            <ranges max="'+str(rangeMax)+'"  medium="'+str(rangeMid)+'"  min="'+str(rangeLow)+'"   />      \
            <angles x="100"  y="0"  z="0"   />    </structure_1>  \
            </Variogram_C11>    <Correl_Z1Z2  value="0.85" />   </parameters>')
            
            sgems.execute('CopyProperty  poro::perm_facies'+str(i)+'__real0'+'::temp::perm::0::0')
            sgems.execute('CopyProperty  poro::perm_facies'+str(i)+'__real0'+'::perm::perm_facies'+str(i)+'_'+ str(count)+'::0::0')
            sgems.execute('DeleteObjectProperties  poro::perm_facies'+str(i)+'__real0')
            filename='Perm_Facies_'+str(i)+'_'+str(rangeMax)+'_' + str(k) + '_'+str(j)+'.out::gslib'    
            count = count+1
            print filename
            sgems.execute('SaveGeostatGrid  temp::'+path+'/Permeability/'+filename+'::0::perm')
sgems.execute('SaveProject  ' + path + '/Permeability/PermProjectFull.prj')

