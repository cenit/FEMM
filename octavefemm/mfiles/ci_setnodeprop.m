% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ci_setnodeprop(nodeprop,groupno,inconductor)
callfemm(['ci_setnodeprop(' , quotec(nodeprop) , numc(groupno) , quote(inconductor) , ')']);

