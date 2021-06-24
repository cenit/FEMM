% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function hi_copytranslate2(p1,p2,p3,p4)
if (nargin==4)
	bx=p1; by=p2; copies=p3; editaction=p4;
elseif (nargin==3)
	bx=p1(1); by=p1(2); copies=p2; editaction=p3;
end
callfemm(['hi_copytranslate(' , numc(bx) , numc(by) , numc(copies) , num(editaction) , ')' ]);

