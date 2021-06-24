% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function co_selectpoint(x,y)
if (nargin==2)
	callfemm(['co_selectpoint(' , numc(x) , num(y) , ')' ]);
elseif (nargin==1)
	callfemm(['co_selectpoint(' , numc(x(1)) , num(x(2)) , ')' ]);
end

