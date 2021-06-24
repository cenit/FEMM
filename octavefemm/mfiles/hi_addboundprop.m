% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function hi_addboundprop(pname,fmt,Tset,qs,Tinf,h,bta)
switch nargin
	case 0
		callfemm('hi_addboundprop()');
	case 1
        callfemm(['hi_addboundprop(' , quote(pname) , ')' ]);
	case 2
        callfemm(['hi_addboundprop(' , quotec(pname) , num(fmt) , ')' ]);
	case 3
        callfemm(['hi_addboundprop(' , quotec(pname) , numc(fmt) , num(Tset) , ')' ]);
	case 4
        callfemm(['hi_addboundprop(' , quotec(pname) , numc(fmt) , numc(Tset) , num(qs) , ')' ]);
	case 5
        callfemm(['hi_addboundprop(' , quotec(pname) , numc(fmt) , numc(Tset) , numc(qs) , num(Tinf) , ')' ]);
	case 6
        callfemm(['hi_addboundprop(' , quotec(pname) , numc(fmt) , numc(Tset) , numc(qs) , numc(Tinf) , num(h), ')' ]);
	otherwise
        callfemm(['hi_addboundprop(' , quotec(pname) , numc(fmt) , numc(Tset) , numc(qs) , numc(Tinf) , numc(h), num(bta) , ')' ]);
end

