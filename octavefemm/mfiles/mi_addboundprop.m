% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mi_addboundprop(pname,a0,a1,a2,phi,mu,sig,c0,c1,fmt,ia,oa)

switch nargin
	case 0
		callfemm('mi_addboundprop()');
	case 1
        callfemm(['mi_addboundprop(' , quote(pname) , ')' ]);
	case 2
		callfemm(['mi_addboundprop(' , quotec(pname) , num(a0) , ')' ]);
	case 3
		callfemm(['mi_addboundprop(' , quotec(pname) , numc(a0) , num(a1) , ')' ]);
	case 4
		callfemm(['mi_addboundprop(' , quotec(pname) , numc(a0) , numc(a1) , num(a2) ,  ')' ]);
	case 5
		callfemm(['mi_addboundprop(' , quotec(pname) , numc(a0) , numc(a1) , numc(a2) , num(phi)  , ')' ]);
	case 6
		callfemm(['mi_addboundprop(' , quotec(pname) , numc(a0) , numc(a1) , numc(a2) , numc(phi) , num(mu) , ')' ]);
	case 7
		callfemm(['mi_addboundprop(' , quotec(pname) , numc(a0) , numc(a1) , numc(a2) , numc(phi) , numc(mu) , num(sig) , ')' ]);
	case 8
		callfemm(['mi_addboundprop(' , quotec(pname) , numc(a0) , numc(a1) , numc(a2) , numc(phi) , numc(mu) , numc(sig) , num(c0) , ')' ]);
	case 9
		callfemm(['mi_addboundprop(' , quotec(pname) , numc(a0) , numc(a1) , numc(a2) , numc(phi) , numc(mu) , numc(sig) , numc(c0) , num(c1) , ')' ]);
	case 10
		callfemm(['mi_addboundprop(' , quotec(pname) , numc(a0) , numc(a1) , numc(a2) , numc(phi) , numc(mu) , numc(sig) , numc(c0) , numc(c1) , num(fmt) , ')' ]);
	case 11
		callfemm(['mi_addboundprop(' , quotec(pname) , numc(a0) , numc(a1) , numc(a2) , numc(phi) , numc(mu) , numc(sig) , numc(c0) , numc(c1) , numc(fmt) , num(ia), ')' ]);
	case 12
		callfemm(['mi_addboundprop(' , quotec(pname) , numc(a0) , numc(a1) , numc(a2) , numc(phi) , numc(mu) , numc(sig) , numc(c0) , numc(c1) , numc(fmt) , numc(ia), num(oa), ')' ]);
end 