% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=eo_lineintegral(ptype)
z=callfemm(['eo_lineintegral(' , num(ptype) , ')' ]);

