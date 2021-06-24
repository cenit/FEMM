% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function hi_analyse(n)
if (nargin==1)
	callfemm(['hi_analyze(' , num(n) , ')' ]);
elseif (nargin==0)
	callfemm('hi_analyze()');
end

