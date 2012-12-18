clear all;
permHistBase0 = load('perm_hist0.out');
permHistBase1 = load('perm_hist1.out');
permHistBase2 = load('perm_hist2.out');
permHistBase3 = load('perm_hist3.out');

poroHistBase0 = load('poro_hist0.out');
poroHistBase1 = load('poro_hist1.out');
poroHistBase2 = load('poro_hist2.out');
poroHistBase3 = load('poro_hist3.out');

% Store range and std
rangePerm = [25,75 ; 1000,1400; 1430,1830;1875,2475];
varPerm = [std(permHistBase0);std(permHistBase1);std(permHistBase2);std(permHistBase3)];

rangePoro = [0.0193, 0.03 ; 0.113,0.213;0.151,0.231;0.19,0.27];
varPoro = [std(poroHistBase0);std(poroHistBase1);std(poroHistBase2);std(poroHistBase3)];

stdPoro = [0.022, 0.032; 0.045, 0.065; 0.048, 0.068; 0.035, 0.055];
stdPerm = [25, 50; 550, 750; 485, 685; 295, 395];

% Normalize 
perm0_norm = (permHistBase0 - mean(permHistBase0))/varPerm(1);
perm1_norm = (permHistBase1 - mean(permHistBase1))/varPerm(2);
perm2_norm = (permHistBase2 - mean(permHistBase2))/varPerm(3);
perm3_norm = (permHistBase3 - mean(permHistBase3))/varPerm(4);

poro0_norm = (poroHistBase0 - mean(poroHistBase0))/varPoro(1);
poro1_norm = (poroHistBase1 - mean(poroHistBase1))/varPoro(2);
poro2_norm = (poroHistBase2 - mean(poroHistBase2))/varPoro(3);
poro3_norm = (poroHistBase3 - mean(poroHistBase3))/varPoro(4);

% Generate random histograms
nReal = 10;
nFacies = 4;

stdRange = [0.7, 1.25];
newMeans = zeros(nReal,nFacies);
newVar = zeros(nReal,nFacies);

newMeansPoro = zeros(nReal,nFacies);
newVarPoro = zeros(nReal,nFacies);
for i = 1:nReal
   
    % Generate a new mean for each facie
    
    for j = 1:nFacies
        newMeans(i,j) = trirnd(rangePerm(j,1), 0.5*(rangePerm(j,1) + rangePerm(j,2)), rangePerm(j,2),1);
        newVar(i,j) =  trirnd(stdPerm(j,1), varPerm(j), stdPerm(j,2),1);
        
        newMeansPoro(i,j) = trirnd(rangePoro(j,1), 0.5*(rangePoro(j,1) + rangePoro(j,2)), rangePoro(j,2),1);
        newVarPoro(i,j)   = trirnd(stdPoro(j,1), varPoro(j), stdPoro(j,2),1);
    end
    
    % Generate new histogram
    permHistNew0 = perm0_norm *newVar(i,1) + newMeans(i,1);
    permHistNew1 = perm1_norm *newVar(i,2) + newMeans(i,2);
    permHistNew2 = perm2_norm *newVar(i,3) + newMeans(i,3);
    permHistNew3 = perm3_norm *newVar(i,4) + newMeans(i,4);
    
        
    permHistNew0 = max(permHistNew0, 0);
    permHistNew1 = max(permHistNew1, 0);
    permHistNew2 = max(permHistNew2, 0);
    permHistNew3 = max(permHistNew3, 0);
    
    
    permHistNew0 = min(permHistNew0, 2600);
    permHistNew1 = min(permHistNew1, 2600);
    permHistNew2 = min(permHistNew2, 2600);
    permHistNew3 = min(permHistNew3, 2600);
    
    
    poroHistNew0 = max(poro0_norm *newVarPoro(i,1) + newMeansPoro(i,1),0);
    poroHistNew1 = max(poro1_norm *newVarPoro(i,2) + newMeansPoro(i,2),0);
    poroHistNew2 = max(poro2_norm *newVarPoro(i,3) + newMeansPoro(i,3),0);
    poroHistNew3 = max(poro3_norm *newVarPoro(i,4) + newMeansPoro(i,4),0);
    
    poroHistNew0 = min(poroHistNew0,0.36);
    poroHistNew1 = min(poroHistNew1,0.36);
    poroHistNew2 = min(poroHistNew2,0.36);
    poroHistNew3 = min(poroHistNew3,0.36);
    
    folderName = ['real_' num2str(i)]; 
    if (exist(folderName,'dir') == 0)
        mkdir(folderName);
    end
    
    % Save realization into folder
    fileID = fopen([folderName '/perm_hist0.out'],'w');
    fprintf(fileID,'%-6.8f\n',permHistNew0);
    fclose(fileID);
    
    fileID = fopen([folderName '/perm_hist1.out'],'w');
    fprintf(fileID,'%-6.8f\n',permHistNew1);
    fclose(fileID);
    
    
    fileID = fopen([folderName '/perm_hist2.out'],'w');
    fprintf(fileID,'%-6.8f\n',permHistNew2);
    fclose(fileID);
    
    fileID = fopen([folderName '/perm_hist3.out'],'w');
    fprintf(fileID,'%-6.8f\n',permHistNew3);
    fclose(fileID);
    
    % Save realization into folder
    fileID = fopen([folderName '/poro_hist0.out'],'w');
    fprintf(fileID,'%-6.8f\n',poroHistNew0);
    fclose(fileID);
    
    fileID = fopen([folderName '/poro_hist1.out'],'w');
    fprintf(fileID,'%-6.8f\n',poroHistNew1);
    fclose(fileID);
    
    fileID = fopen([folderName '/poro_hist2.out'],'w');
    fprintf(fileID,'%-6.8f\n',poroHistNew2);
    fclose(fileID);
    
    fileID = fopen([folderName '/poro_hist3.out'],'w');
    fprintf(fileID,'%-6.8f\n',poroHistNew3);
    fclose(fileID);
    
    
%     save([folderName '/perm_hist0.out'], 'permHistNew0','-ascii');
%     save([folderName '/perm_hist1.out'], 'permHistNew1','-ascii');
%     save([folderName '/perm_hist2.out'], 'permHistNew2','-ascii');
%     save([folderName '/perm_hist3.out'], 'permHistNew3','-ascii');
%     
%     save([folderName '/poro_hist0.out'], 'poroHistNew0','-ascii');
%     save([folderName '/poro_hist1.out'], 'poroHistNew1','-ascii');
%     save([folderName '/poro_hist2.out'], 'poroHistNew2','-ascii');
%     save([folderName '/poro_hist3.out'], 'poroHistNew3','-ascii');
end

x = 1:length(perm0_norm);
% 
% % Generate a new std
% std_new = 1.05*std(permHistBase0);
% 
% rn = trirnd(rangePerm0(1), 0.5*(rangePerm0(1) + rangePerm0(2)),rangePerm0(2),1)
