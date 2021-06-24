% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function hi_addconductorprop(pname,Tc,qc,ptype)
switch nargin
	case 0
		callfemm('hi_addconductorprop()');
	case 1
		callfemm(['hi_addconductorprop(' , quote(pname) , ')' ]);
	case 2
		callfemm(['hi_addconductorprop(' , quotec(pname) , num(Tc) , ')' ]);
	case 3
		callfemm(['hi_addconductorprop(' , quotec(pname) , numc(Tc) , num(qc) , ')' ]);
	otherwise
		callfemm(['hi_addconductorprop(' , quotec(pname) , numc(Tc) , numc(qc) , num(ptype) , ')' ]);
end

