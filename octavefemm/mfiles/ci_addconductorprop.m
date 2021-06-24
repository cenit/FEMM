% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ci_addconductorprop(pname,Tc,qc,ptype)
switch nargin
	case 0
		callfemm('ci_addconductorprop()');
	case 1
		callfemm(['ci_addconductorprop(' , quote(pname) , ')' ]);
	case 2
		callfemm(['ci_addconductorprop(' , quotec(pname) , num(Tc) , ')' ]);
	case 3
		callfemm(['ci_addconductorprop(' , quotec(pname) , numc(Tc) , num(qc) , ')' ]);
	otherwise
		callfemm(['ci_addconductorprop(' , quotec(pname) , numc(Tc) , numc(qc) , num(ptype) , ')' ]);
end

