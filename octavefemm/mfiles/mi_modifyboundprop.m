% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mi_modifyboundprop(pname,pnum,pval)
	if (pnum==0)
      	callfemm(['mi_modifyboundprop(' , quotec(pname) , numc(pnum) , quote(pval) , ')']);
      else
		callfemm(['mi_modifyboundprop(' , quotec(pname) , numc(pnum) , num(pval) , ')']);
      end

