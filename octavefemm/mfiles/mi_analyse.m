% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mi_analyse(n)
if (nargin==1)
    callfemm(['mi_analyze(' , num(n) , ')' ]);
elseif (nargin==0)
    callfemm('mi_analyze()');
end

