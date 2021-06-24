% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=mo_getnode(n)
z=callfemm(['mo_getnode(' , num(n) , ')' ]);

