% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mi_addpointprop(pname,ap,jp)
callfemm(['mi_addpointprop(' , quotec(pname) , numc(ap), num(jp), ')']);

