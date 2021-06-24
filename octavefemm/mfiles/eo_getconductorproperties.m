% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=eo_getconductorproperties(pname)
z=callfemm(['eo_getconductorproperties(' , quote(pname) , ')' ]);

