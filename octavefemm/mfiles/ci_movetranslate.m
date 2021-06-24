% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ci_movetranslate(bx,by)
if (nargin==2)
	callfemm(['ci_movetranslate(' , numc(bx) , num(by) , ')' ]);
elseif (nargin==1)
	callfemm(['ci_movetranslate(' , numc(bx(1)) , num(bx(2)) ,')' ]);
end

