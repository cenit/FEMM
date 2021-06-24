% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ei_setgrid(density,ptype)
callfemm(['ei_setgrid(' , numc(density) , quote(ptype) , ')' ]);

