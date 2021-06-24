% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ei_analyze(n)
if (nargin==1)
	callfemm(['ei_analyze(' , num(n) , ')' ]);
elseif (nargin==0)
	callfemm('ei_analyze()');
end

