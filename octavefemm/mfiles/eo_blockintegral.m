% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=eo_blockintegral(ptype)
z=callfemm(['eo_blockintegral(' , num(ptype) , ')' ]);
        
