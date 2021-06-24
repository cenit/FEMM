% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function hi_addpointprop(pname,Tp,qp)
switch nargin
	case 0
		callfemm('hi_addpointprop()')
	case 1
		callfemm(['hi_addpointprop(' , quote(pname) , ')']);
	case 2
		callfemm(['hi_addpointprop(' , quotec(pname) , num(Tp) , ')']);
	otherwise
		callfemm(['hi_addpointprop(' , quotec(pname) , numc(Tp) , num(qp) , ')']);
end

