% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mo_setgrid(density,ptype)
callfemm(['mo_setgrid(' , numc(density) , quote(ptype) , ')' ]);

