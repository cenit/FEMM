% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mi_copytranslate(p1,p2,p3)
if (nargin==3)
	bx=p1; by=p2; copies=p3;
elseif (nargin==2)
	bx=p1(1); by=p1(2); copies=p2;
end
callfemm(['mi_copytranslate(' , numc(bx) , numc(by) , num(copies) , ')' ]);

