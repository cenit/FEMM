% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=co_lineintegral(ptype)
z=callfemm(['co_lineintegral(' , num(ptype) , ')' ]);

