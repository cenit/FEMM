% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function p=mo_getpointvalues(x,y)
global pointvaluesfilter
if (nargin==2)
	n=length(x);
	if (length(y)~=n)
		error('must be same number of entries in X and Y arrays');
	end
	z=zeros(n,14);
	for k=1:n
		z(k,:)=callfemm(['mo_getpointvalues(' , numc(x(k)) , num(y(k)) , ')']);
	end
elseif (nargin==1)
	n=size(x); n=n(1);
	z=zeros(n,14);
	for k=1:n
		z(k,:)=callfemm(['mo_getpointvalues(' , numc(x(k,1)) , num(x(k,2)) , ')']);
	end
else
	z=0;
end
if (length(z)>1)
	p=z; 	
else
	p=zeros(length(x),14);
end

