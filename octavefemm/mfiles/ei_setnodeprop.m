% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ei_setnodeprop(nodeprop,groupno,inconductor)
callfemm(['ei_setnodeprop(' , quotec(nodeprop) , numc(groupno) , quote(inconductor) , ')']);

