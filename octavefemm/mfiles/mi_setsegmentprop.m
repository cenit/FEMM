% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mi_setsegmentprop(pn,es,am,hi,gr)
callfemm(['mi_setsegmentprop(' , quotec(pn) , numc(es) , numc(am) , numc(hi) , num(gr) , ')']);

