% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=co_getnode(n)
z=callfemm(['co_getnode(' , num(n) , ')' ]);

