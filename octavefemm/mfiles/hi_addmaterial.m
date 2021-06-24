% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function hi_addmaterial(pname,kx,ky,qv,kt)
switch nargin
	case 0 
		callfemm('hi_addmaterial()');
	case 1
		callfemm(['hi_addmaterial(' , quote(pname) ')' ]);
	case 2
		callfemm(['hi_addmaterial(' , quotec(pname) , num(kx) , ')' ]);
	case 3 
		callfemm(['hi_addmaterial(' , quotec(pname) , numc(kx) , num(ky) , ')' ]);
	case 4
		callfemm(['hi_addmaterial(' , quotec(pname) , numc(kx) , numc(ky) , num(qv) , ')' ]);
	otherwise
		callfemm(['hi_addmaterial(' , quotec(pname) , numc(kx) , numc(ky) , numc(qv) , num(kt) , ')' ]);
end

