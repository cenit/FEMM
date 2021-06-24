% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ei_modifyconductorprop(pname,pnum,pval)
        if (pnum==0)
                callfemm(['ei_modifyconductorprop(' , quotec(pname) , numc(pnum) , quote(pval) , ')']);
	  else
                callfemm(['ei_modifyconductorprop(' , quotec(pname) , numc(pnum) , num(pval) , ')']);
        end

