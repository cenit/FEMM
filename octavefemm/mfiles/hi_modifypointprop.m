% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function hi_modifypointprop(pname,pnum,pval)
        if (pnum==0)
                callfemm(['hi_modifypointprop(' , quotec(pname) , numc(pnum) , quote(pval) , ')']);
        else
                callfemm(['hi_modifypointprop(' , quotec(pname) , numc(pnum) , num(pval) , ')']);
        end

