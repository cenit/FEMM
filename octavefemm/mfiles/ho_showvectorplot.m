% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ho_showvectorplot(plottype, scalefactor)
if (nargin==2)
	callfemm(['ho_showvectorplot(' , numc(plottype) , num(scalefactor) , ')' ]);
elseif (nargin==1)
	callfemm(['ho_showvectorplot(' , num(plottype) , ')' ]);
end

