% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ei_addmaterial(pname,ex,ey,qv)
callfemm(['ei_addmaterial(' , quotec(pname) , numc(ex) , numc(ey) , num(qv) , ')' ]);

