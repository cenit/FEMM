% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=ho_getnode(n)
z=callfemm(['ho_getnode(' , num(n) , ')' ]);

