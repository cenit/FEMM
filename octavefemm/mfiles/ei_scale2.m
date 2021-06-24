% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ei_scale2(p1,p2,p3,p4)
if (nargin==4)
	bx=p1; by=p2; sc=p3; ea=p4;
elseif (nargin==3)
	bx=p1(1); by=p1(2); sc=p2; ea=p3;
end
callfemm(['ei_scale(' , numc(bx) , numc(by) , numc(sc) , num(ea) , ')' ]);

