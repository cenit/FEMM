% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ci_addpointprop(pname,Tp,qp)
switch nargin
	case 0
		callfemm('ci_addpointprop()')
	case 1
		callfemm(['ci_addpointprop(' , quote(pname) , ')']);
	case 2
		callfemm(['ci_addpointprop(' , quotec(pname) , num(Tp) , ')']);
	otherwise
		callfemm(['ci_addpointprop(' , quotec(pname) , numc(Tp) , num(qp) , ')']);
end

