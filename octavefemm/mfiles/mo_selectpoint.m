% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mo_selectpoint(x,y)
if (nargin==2)
	callfemm(['mo_selectpoint(' , numc(x) , num(y) , ')' ]);
elseif (nargin==1)
	callfemm(['mo_selectpoint(' , numc(x(1)) , num(x(2)) , ')' ]);
end

