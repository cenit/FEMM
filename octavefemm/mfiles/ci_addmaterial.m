% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ci_addmaterial(pname,ox,oy,ex,ey,ltx,lty)
switch nargin
	case 0 
		callfemm('ci_addmaterial()');
	case 1
		callfemm(['ci_addmaterial(' , quote(pname) ')' ]);
	case 2
		callfemm(['ci_addmaterial(' , quotec(pname) , num(ox) , ')' ]);
	case 3 
		callfemm(['ci_addmaterial(' , quotec(pname) , numc(ox) , num(oy) , ')' ]);
	case 4
		callfemm(['ci_addmaterial(' , quotec(pname) , numc(ox) , numc(oy) , num(ex) , ')' ]);
	case 5
		callfemm(['ci_addmaterial(' , quotec(pname) , numc(ox) , numc(oy) , numc(ex) , num(ey) , ')' ]);
	case 6
		callfemm(['ci_addmaterial(' , quotec(pname) , numc(ox) , numc(oy) , numc(ex) , numc(ey), num(ltx) , ')' ]);
	otherwise
		callfemm(['ci_addmaterial(' , quotec(pname) , numc(ox) , numc(oy) , numc(ex) , numc(ey), numc(ltx) , num(lty) , ')' ]);
end

