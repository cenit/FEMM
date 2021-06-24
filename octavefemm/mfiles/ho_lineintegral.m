% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=ho_lineintegral(ptype)
z=callfemm(['ho_lineintegral(' , num(ptype) , ')' ]);

