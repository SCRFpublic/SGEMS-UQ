%Script by Dr.Mongkut Piantanakulchai
%To simulate the triangular distribution
%Return a vector of random variable
%The range of the value is between (a,b)
%The mode is c (most probable value)
%n is to tatal number of values generated
%Example of using
%X = trirnd(1,5,10,100000);
% this will generate 100000 random numbers between 1 and 10 (where most probable
% value is 5)
% To visualize the result use the command
% hist(X,50); 

function X=trirnd(a,c,b,n)
X=zeros(n,1);
for i=1:n
%Assume a<X<c
z=rand;
if sqrt(z*(b-a)*(c-a))+a<c
    X(i)=sqrt(z*(b-a)*(c-a))+a;
else
    X(i)=b-sqrt((1-z)*(b-a)*(b-c));
end
end %for
%hist(X,50); Remove this comment % to look at histogram of X
end %function