% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ci_modifypointprop(pname,pnum,pval)
        if (pnum==0)
                callfemm(['ci_modifypointprop(' , quotec(pname) , numc(pnum) , quote(pval) , ')']);
        else
                callfemm(['ci_modifypointprop(' , quotec(pname) , numc(pnum) , num(pval) , ')']);
        end

