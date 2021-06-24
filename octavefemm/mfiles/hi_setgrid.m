% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function hi_setgrid(density,ptype)
callfemm(['hi_setgrid(' , numc(density) , quote(ptype) , ')' ]);

