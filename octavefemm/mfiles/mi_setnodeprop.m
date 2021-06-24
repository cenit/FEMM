% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mi_setnodeprop(nodeprop,groupno)
callfemm(['mi_setnodeprop(' , quotec(nodeprop) , num(groupno) , ')']);

