% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function co_setgrid(density,ptype)
callfemm(['co_setgrid(' , numc(density) , quote(ptype) , ')' ]);

