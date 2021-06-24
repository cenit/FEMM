% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mi_probdef(freq, units, ptype, precision, depth, minangle, acsolver)
if (nargin==1)
callfemm(['mi_probdef(' , num(freq) , ')' ]);
elseif (nargin==2)
callfemm(['mi_probdef(' , numc(freq) , quote(units) , ')' ]);
elseif (nargin==3)
callfemm(['mi_probdef(' , numc(freq) , quotec(units) , quote(ptype) ')' ]);
elseif (nargin==4)
callfemm(['mi_probdef(' , numc(freq) , quotec(units) , quotec(ptype) , num(precision) , ')' ]);
elseif (nargin==5)
callfemm(['mi_probdef(' , numc(freq) , quotec(units) , quotec(ptype) , numc(precision) , num(depth) , ')' ]);
elseif (nargin==6)
callfemm(['mi_probdef(' , numc(freq) , quotec(units) , quotec(ptype) , numc(precision) , numc(depth) , num(minangle) , ')' ]);
elseif (nargin==7)
callfemm(['mi_probdef(' , numc(freq) , quotec(units) , quotec(ptype) , numc(precision) , numc(depth) , numc(minangle) , num(acsolver) ')' ]);
end

