% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ei_moverotate(p1,p2,p3)
if (nargin==3)
	bx=p1; by=p2; shiftangle=p3;
elseif (nargin==2)
	bx=p1(1); by=p1(2); shiftangle=p2;
end
callfemm(['ei_moverotate(' , numc(bx) , numc(by) , num(shiftangle) , ')' ]);

