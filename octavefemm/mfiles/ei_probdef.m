% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ei_probdef(units, ptype, precision, depth, minangle)
if (nargin==3)
callfemm(['ei_probdef(' , quotec(units) , quotec(ptype) , num(precision) , ')']);
elseif (nargin==4)
callfemm(['ei_probdef(' , quotec(units) , quotec(ptype) , numc(precision) , num(depth) , ')']);
elseif (nargin==5)
callfemm(['ei_probdef(' , quotec(units) , quotec(ptype) , numc(precision) , numc(depth) , num(minangle) , ')']);
end