% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function co_groupselectblock(n)
if(nargin==1)
callfemm(['co_groupselectblock(' , num(n) , ')' ]);
else
callfemm('co_groupselectblock()');
end

