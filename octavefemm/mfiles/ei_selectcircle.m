% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ei_selectcircle(x,y,R,editmode)
if (nargin==3)
	callfemm(['ei_selectcircle(' , numc(x) , numc(y), num(R) , ')' ]);
elseif (nargin==4)
	callfemm(['ei_selectcircle(' , numc(x) , numc(y), numc(R), num(editmode) , ')' ]);
end

