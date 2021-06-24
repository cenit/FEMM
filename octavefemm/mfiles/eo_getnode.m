% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=eo_getnode(n)
z=callfemm(['eo_getnode(' , num(n) , ')' ]);

