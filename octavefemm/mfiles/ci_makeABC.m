% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ci_makeABC(n,R,x,y,bc)
if (nargin==5)
	callfemm(['ci_makeABC(' , numc(n) , numc(R), numc(x), numc(y) , num(bc), ')' ]);
elseif (nargin==4)
	callfemm(['ci_makeABC(' , numc(n) , numc(R), numc(x), num(y) , ')' ]);
elseif (nargin==3)
	callfemm(['ci_makeABC(' , numc(n) , numc(R), num(x) , ')' ]);
elseif (nargin==2)
	callfemm(['ci_makeABC(' , numc(n) , num(R), ')' ]);
elseif (nargin==1)
	callfemm(['ci_makeABC(' , num(n) , ')' ]);
else
	callfemm('ci_makeABC()');
end
