% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mi_selectcircle(x,y,R,editmode)
if (nargin==3)
	callfemm(['mi_selectcircle(' , numc(x) , numc(y), num(R) , ')' ]);
elseif (nargin==4)
	callfemm(['mi_selectcircle(' , numc(x) , numc(y), numc(R), num(editmode) , ')' ]);
end

