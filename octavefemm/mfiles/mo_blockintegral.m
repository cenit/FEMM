% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=mo_blockintegral(ptype)
	z=callfemm(['mo_blockintegral(' , num(ptype) , ')' ]);

