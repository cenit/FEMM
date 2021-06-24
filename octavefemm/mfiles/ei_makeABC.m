% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ei_makeABC(n,R,x,y,bc)
if (nargin==5)
	callfemm(['ei_makeABC(' , numc(n) , numc(R), numc(x), numc(y) , num(bc), ')' ]);
elseif (nargin==4)
	callfemm(['ei_makeABC(' , numc(n) , numc(R), numc(x), num(y) , ')' ]);
elseif (nargin==3)
	callfemm(['ei_makeABC(' , numc(n) , numc(R), num(x) , ')' ]);
elseif (nargin==2)
	callfemm(['ei_makeABC(' , numc(n) , num(R), ')' ]);
elseif (nargin==1)
	callfemm(['ei_makeABC(' , num(n) , ')' ]);
else
	callfemm('ei_makeABC()');
end
