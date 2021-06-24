% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mi_movetranslate(bx,by)
if (nargin==2)
	callfemm(['mi_movetranslate(' , numc(bx) , num(by) ')' ]);
elseif (nargin==1)
	callfemm(['mi_movetranslate(' , numc(bx(1)) , num(bx(2)) ')' ]);
end

