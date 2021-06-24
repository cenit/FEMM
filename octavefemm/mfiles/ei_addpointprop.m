% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ei_addpointprop(pname,vp,qp)
callfemm(['ei_addpointprop(' , quotec(pname) , numc(vp) , num(qp) , ')']);

