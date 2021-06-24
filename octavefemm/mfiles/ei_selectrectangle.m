% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ei_selectrectangle(x1,y1,x2,y2,editmode)
if (nargin==4)
	callfemm(['ei_selectrectangle(' , numc(x1) , numc(y1), numc(x2), num(y2) , ')' ]);
elseif (nargin==5)
	callfemm(['ei_selectrectangle(' , numc(x1) , numc(y1), numc(x2), numc(y2), num(editmode) , ')' ]);
end

