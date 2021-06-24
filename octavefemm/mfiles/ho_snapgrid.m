% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ho_snapgrid(flag)
callfemm(['ho_gridsnap(' , quote(flag) , ')' ]);

