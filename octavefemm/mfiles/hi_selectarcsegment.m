% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=hi_selectarcsegment(x,y)
if (nargin==2)
	z=callfemm(['hi_selectarcsegment(' , numc(x) , num(y) , ')']);
elseif (nargin==1)
	z=callfemm(['hi_selectarcsegment(' , numc(x(1)) , num(x(2)) , ')']);
end
 
