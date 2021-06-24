% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mo_showcontourplot(numcontours,al,au,ptype)
callfemm(['mo_showcontourplot(' , numc(numcontours) , numc(al) ,  numc(au) , quote(ptype) , ')' ]);

