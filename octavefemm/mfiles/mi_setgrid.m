% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mi_setgrid(density,ptype)
callfemm(['mi_setgrid(' , numc(density) , quote(ptype) , ')' ]);

