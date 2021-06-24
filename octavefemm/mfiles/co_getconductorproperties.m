% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=co_getconductorproperties(pname)
z=callfemm(['co_getconductorproperties(' , quote(pname) , ')' ]);

