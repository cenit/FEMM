% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function hi_mirror2(p1,p2,p3,p4,p5)
if (nargin==5)
	x1=p1; y1=p2; x2=p3; y2=p4; editaction=p5;
elseif (nargin==3)
	x1=p1(1); y1=p1(2); x2=p2(1); y2=p2(2); editaction=p3;
elseif (nargin==2)
	x1=p1(1,1); y1=p1(1,2); x2=p1(2,1); y2=p1(2,2); editaction=p2;
end
callfemm(['hi_mirror(' , numc(x1) , numc(y1) , numc(x2) , numc(y2) , num(editaction) , ')' ]);

