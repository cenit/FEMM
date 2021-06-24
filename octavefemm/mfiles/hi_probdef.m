% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function hi_probdef(units, ptype, precision, depth, minangle, prevsoln, dt)
if (nargin==3)
	callfemm(['hi_probdef(' , quotec(units) , quotec(ptype) , num(precision) , ')']);
elseif (nargin==4)
	callfemm(['hi_probdef(' , quotec(units) , quotec(ptype) , numc(precision) , num(depth) , ')']);
elseif (nargin==5)
	callfemm(['hi_probdef(' , quotec(units) , quotec(ptype) , numc(precision) , numc(depth) , num(minangle) , ')']);
elseif (nargin==6)
	callfemm(['hi_probdef(' , quotec(units) , quotec(ptype) , numc(precision) , numc(depth) , numc(minangle) , quote(prevsoln), ')']);
elseif (nargin==7)
	callfemm(['hi_probdef(' , quotec(units) , quotec(ptype) , numc(precision) , numc(depth) , numc(minangle) , quotec(prevsoln), num(dt), ')']);
end

