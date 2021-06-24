% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function eo_setgrid(density,ptype)
callfemm(['eo_setgrid(' , numc(density) , quote(ptype) , ')' ]);

