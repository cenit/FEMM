% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function eo_showvectorplot(plottype,scalefactor)
if (nargin==2)
	callfemm(['eo_showvectorplot(' , numc(plottype) , num(scalefactor) , ')' ]);
elseif (nargin==1)
	callfemm(['eo_showvectorplot(' , num(plottype) , ')' ]);
end

