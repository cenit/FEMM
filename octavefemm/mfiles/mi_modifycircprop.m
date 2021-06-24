% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mi_modifycircprop(pname,pnum,pval)
        if (pnum==0)
                callfemm(['mi_modifycircprop(' , quotec(pname) , numc(pnum) , quote(pval) , ')']);
        else
                callfemm(['mi_modifycircprop(' , quotec(pname) , numc(pnum) , num(pval) , ')']);
        end

