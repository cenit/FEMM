% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function co_showvectorplot(plottype,scalefactor)
if (nargin==2)
	callfemm(['co_showvectorplot(' , numc(plottype) , num(scalefactor) , ')' ]);
elseif (nargin==1)
	callfemm(['co_showvectorplot(' , num(plottype) , ')' ]);
end

