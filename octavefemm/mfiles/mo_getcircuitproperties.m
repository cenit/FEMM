% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=mo_getcircuitproperties(name)
	z=callfemm(['mo_getcircuitproperties(' , quote(name) , ')' ]);

