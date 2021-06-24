% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mo_snapgrid(flag)
callfemm(['mo_gridsnap(' , quote(flag) , ')' ]);

