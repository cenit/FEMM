% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=mo_lineintegral(ptype)
	z=callfemm(['mo_lineintegral(' , num(ptype) , ')']);

