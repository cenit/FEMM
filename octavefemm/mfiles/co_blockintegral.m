% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=co_blockintegral(ptype)
z=callfemm(['co_blockintegral(' , num(ptype) , ')' ]);
        
