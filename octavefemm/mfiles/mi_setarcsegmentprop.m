% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mi_setarcsegmentprop(maxsegdeg,propname,hide,group)
callfemm(['mi_setarcsegmentprop(' , numc(maxsegdeg) , quotec(propname) , numc(hide) , num(group) , ')' ]);

