% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=eo_getelement(n)
z=callfemm(['eo_getelement(' , num(n) , ')' ]);

