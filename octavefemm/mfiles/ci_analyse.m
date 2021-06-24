% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ci_analyse(n)
if (nargin==1)
	callfemm(['ci_analyze(' , num(n) , ')' ]);
elseif (nargin==0)
	callfemm('ci_analyze()');
end

