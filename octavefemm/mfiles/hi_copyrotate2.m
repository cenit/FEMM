% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function hi_copyrotate2(p1,p2,p3,p4,p5)
if (nargin==5)
	bx=p1; by=p2; angle=p3; copies=p4; editaction=p5;
elseif (nargin==4)
	bx=p1(1); by=p1(2); angle=p2; copies=p3; editaction=p4;
end
callfemm(['hi_copyrotate(' , numc(bx) , numc(by) , numc(angle) , numc(copies) , num(editaction) , ')' ]);

