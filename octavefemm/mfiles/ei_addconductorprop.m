% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ei_addconductorprop(pname,vc,qc,ptype)
callfemm(['ei_addconductorprop(' , quotec(pname) , numc(vc) , numc(qc) , num(ptype) , ')' ]);

