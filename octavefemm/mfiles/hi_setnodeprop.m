% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function hi_setnodeprop(nodeprop,groupno,inconductor)
callfemm(['hi_setnodeprop(' , quotec(nodeprop) , numc(groupno) , quote(inconductor) , ')']);

