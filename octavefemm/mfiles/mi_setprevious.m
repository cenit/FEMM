% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mi_setprevious(fn,pt)

if (nargin==1)
	callfemm(['mi_setprevious(' , quote(fn) , ')' ]);
elseif (nargin==2)
	callfemm(['mi_setprevious(' , quotec(fn) , num(pt) , ')' ]);
end
