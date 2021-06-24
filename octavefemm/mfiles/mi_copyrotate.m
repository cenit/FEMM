% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mi_copyrotate(p1,p2,p3,p4)
if (nargin==4)
	bx=p1; by=p2; angle=p3; copies=p4;
elseif (nargin==3)
	bx=p1(1); by=p1(2); angle=p2; copies=p3;
end
callfemm(['mi_copyrotate(' , numc(bx) , numc(by) , numc(angle) , num(copies) , ')' ]);

