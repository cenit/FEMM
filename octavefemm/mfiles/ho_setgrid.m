% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ho_setgrid(density,ptype)
callfemm(['ho_setgrid(' , numc(density) , quote(ptype) , ')' ]);

