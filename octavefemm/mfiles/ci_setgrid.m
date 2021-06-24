% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ci_setgrid(density,ptype)
callfemm(['ci_setgrid(' , numc(density) , quote(ptype) , ')' ]);

