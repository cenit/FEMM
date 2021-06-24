% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=hi_selectnode(x,y) 
if (nargin==2)
	z=callfemm(['hi_selectnode(' , numc(x) , num(y) , ')']);
elseif (nargin==1)
	z=callfemm(['hi_selectnode(' , numc(x(1)) , num(x(2)) , ')']);
end

