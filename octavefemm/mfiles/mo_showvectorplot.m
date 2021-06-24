% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mo_showvectorplot(plottype,scalefactor)
if (nargin==2)
	callfemm(['mo_showvectorplot(' , numc(plottype) , num(scalefactor) , ')' ]);
elseif (nargin==1)
	callfemm(['mo_showvectorplot(' , num(plottype) , ')' ]);
end

