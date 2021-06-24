% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mo_groupselectblock(n)
if(nargin==1)
callfemm(['mo_groupselectblock(' , num(n) , ')' ]);
else
callfemm(['mo_groupselectblock()' ]);
end

