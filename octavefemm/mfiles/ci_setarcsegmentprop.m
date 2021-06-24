% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ci_setarcsegmentprop(maxsegdeg,propname,hide,group,incond)
callfemm(['ci_setarcsegmentprop(' , numc(maxsegdeg) , quotec(propname) , numc(hide) , numc(group) , quote(incond) , ')' ]);

