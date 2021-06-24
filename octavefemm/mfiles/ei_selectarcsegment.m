% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=ei_selectarcsegment(x,y)
if (nargin==2)
	z=callfemm(['ei_selectarcsegment(' , numc(x) , num(y) , ')']);
elseif (nargin==1)
	z=callfemm(['ei_selectarcsegment(' , numc(x(1)) , num(x(2)) , ')']);
end
 
