% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=ho_getpointvalues(x,y)
if (nargin==2)
	n=length(x);
	if (length(y)~=n)
		error('must be same number of entries in X and Y arrays');
	end
	z=zeros(n,7);
	for k=1:n
		z(k,:)=callfemm(['ho_getpointvalues(' , numc(x(k)) , num(y(k)) , ')' ]);
	end
elseif (nargin==1)
	n=size(x); n=n(1);
	z=zeros(n,7);
	for k=1:n
		z(k,:)=callfemm(['ho_getpointvalues(' , numc(x(k,1)) , num(x(k,2)) , ')' ]);
	end
end

