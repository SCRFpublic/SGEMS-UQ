
%% This function do a classical sensitivity analysis (experimental 
%% design type of sensitivity) and return a Pareto  plot.  

%% Author: Celine Scheidt

%% Reference: Dejean and Blanc (SPE 56696)

function t = Sensitivity_Analysis_ED(real_params_val,Response,param_names,inter,quad)

%% Input Parameters
% - real_params_val: property values of the models (preferably defined by
%   ED). One column is one parameter.  Should be numerical values.
% - Response: vector of reponse of interest at one particular time
% - param_names: names of the parameters
% - inter:  if inter = 'y', all interations between parameters are considered.
%           if inter = 'n' interactions are ignored
% - quad:   if quad = 'y', all interations between parameters are considered.
%           if quad = 'n' interactions are ignored 

[nb_real,nb_param] = size(real_params_val);

X = ones(nb_real,1);
X = horzcat(X,real_params_val);
ParamNamesInter = param_names;

if nargin > 3  && inter == 'y' % interractions are used
     for i = 1:nb_param-1
         for j = i+1:nb_param
              X = horzcat(X,real_params_val(:,i).*real_params_val(:,j));
              ParamNamesInter = [ParamNamesInter,strcat(param_names{i},':',param_names{j})];
         end
     end    
end

if nargin > 4  && quad == 'y' % interractions are used
    X = horzcat(X,real_params_val.^2);
     for i = 1:nb_param
          ParamNamesInter = [ParamNamesInter,strcat(param_names{i},'^2')];
     end    
end

if length(X) > nb_real
    error('There are not enough responses to analyze interractions');
end
%% Compute the regression coefficients

 C = pinv(X'*X);
 b = C*X'*Response;
 Y_hat = X*b;

 p = length(b);

 sigma2 = (norm(Response-Y_hat,2))^2; 
 sigma2 = sigma2/(nb_real-p);

 denominator = sqrt(diag(C*sigma2));
 t = b./denominator;
 
 
 [valsort,idxsort] = sort(abs(t(2:end)));
 idxsort = idxsort +1;
 v = tinv(0.99,nb_real-size(X,2)-1);

 figure
 h = axes('FontSize',13); hold on
 barh(abs(t(idxsort)))
 plot([v,v],[0,size(X,2)-0.5],'r','LineWidth',3)
 title('Sensitivity of parameters on Response','FontSize',15)
 set(gca,'YTickLabel',ParamNamesInter(idxsort-1))

 

end
