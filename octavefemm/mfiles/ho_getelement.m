% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=ho_getelement(n)
z=callfemm(['ho_getelement(' , num(n) , ')' ]);

