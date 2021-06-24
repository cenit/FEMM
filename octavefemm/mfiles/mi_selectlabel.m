% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=mi_selectlabel(x,y)
if (nargin==2)
	z=callfemm(['mi_selectlabel(' , numc(x) , num(y) , ')']);
elseif (nargin==1)
	z=callfemm(['mi_selectlabel(' , numc(x(1)) , num(x(2)) , ')']);
end

