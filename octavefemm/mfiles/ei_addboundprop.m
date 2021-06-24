% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ei_addboundprop(pname,vs,qs,c0,c1,fmt)
        callfemm(['ei_addboundprop(' , quotec(pname) , numc(vs) , numc(qs) , numc(c0) , numc(c1) , num(fmt) , ')' ]);

