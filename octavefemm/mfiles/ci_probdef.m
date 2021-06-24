% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ci_probdef(units, ptype, frequency, precision, depth, minangle)
if (nargin==4)
callfemm(['ci_probdef(' , quotec(units) , quotec(ptype) , numc(frequency) , num(precision) , ')']);
elseif (nargin==5)
callfemm(['ci_probdef(' , quotec(units) , quotec(ptype) , numc(frequency) , numc(precision) , num(depth) , ')']);
elseif (nargin==6)
callfemm(['ci_probdef(' , quotec(units) , quotec(ptype) , numc(frequency) , numc(precision) , numc(depth) , num(minangle) , ')']);
end