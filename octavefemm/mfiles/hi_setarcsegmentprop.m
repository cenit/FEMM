% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function hi_setarcsegmentprop(maxsegdeg,propname,hide,group,incond)
callfemm(['hi_setarcsegmentprop(' , numc(maxsegdeg) , quotec(propname) , numc(hide) , numc(group) , quote(incond) , ')' ]);

