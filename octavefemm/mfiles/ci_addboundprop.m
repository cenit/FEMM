% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ci_addboundprop(pname,Vs,qs,c0,c1,fmt)
switch nargin
	case 0
		callfemm('ci_addboundprop()');
	case 1
        callfemm(['ci_addboundprop(' , quote(pname) , ')' ]);
	case 2
        callfemm(['ci_addboundprop(' , quotec(pname) , num(Vs) , ')' ]);
	case 3
        callfemm(['ci_addboundprop(' , quotec(pname) , numc(Vs) , num(qs) , ')' ]);
	case 4
        callfemm(['ci_addboundprop(' , quotec(pname) , numc(Vs) , numc(qs) , num(c0) , ')' ]);
	case 5
        callfemm(['ci_addboundprop(' , quotec(pname) , numc(Vs) , numc(qs) , numc(c0) , num(c1) , ')' ]);
	otherwise
        callfemm(['ci_addboundprop(' , quotec(pname) , numc(Vs) , numc(qs) , numc(c0) , numc(c1) , num(fmt), ')' ]);
end

