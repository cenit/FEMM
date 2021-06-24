% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function eo_snapgrid(flag)
callfemm(['eo_gridsnap(' , quote(flag) , ')' ]);

