% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function hi_scale(p1,p2,p3)
if (nargin==3)
	bx=p1; by=p2; sc=p3;
elseif (nargin==2)
	bx=p1(1); by=p1(2); sc=p2;
end
callfemm(['hi_scale(' , numc(bx) , numc(by) , num(sc) , ')' ]);

