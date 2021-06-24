% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function eo_zoom(x1,y1,x2,y2)
if (nargin==4)
	callfemm(['eo_zoom(' , numc(x1) , numc(y1) , numc(x2) , num(y2) , ')' ]);
elseif (nargin==2)
	callfemm(['eo_zoom(' , numc(x1(1)) , numc(x1(2)) , numc(y1(1)) , num(y1(2)) , ')' ]);
elseif (nargin==1)
	callfemm(['eo_zoom(' , numc(x1(1,1)) , numc(x1(1,2)) , numc(x1(2,1)) , num(x1(2,2)) , ')' ]);
end


