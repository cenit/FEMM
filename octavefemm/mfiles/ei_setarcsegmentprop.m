% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ei_setarcsegmentprop(maxsegdeg,propname,hide,group,incond)
callfemm(['ei_setarcsegmentprop(' , numc(maxsegdeg) , quotec(propname) , numc(hide) , numc(group) , quote(incond) , ')' ]);

