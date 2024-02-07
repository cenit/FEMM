% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org
% Modified 04Jun2023 to add in support for combined radiation/convection BC
function hi_addboundprop(pname,fmt,Tset,qs,Tinf,h,bta,TinfRad)
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
	case 7
        callfemm(['hi_addboundprop(' , quotec(pname) , numc(fmt) , numc(Tset) , numc(qs) , numc(Tinf) , numc(h) , num(bta) , ')' ]);
	otherwise
		callfemm(['hi_addboundprop(' , quotec(pname) , numc(fmt) , numc(Tset) , numc(qs) , numc(Tinf) , numc(h) , numc(bta) , num(TinfRad) , ')' ]);
end

