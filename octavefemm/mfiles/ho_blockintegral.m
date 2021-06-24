% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=ho_blockintegral(ptype)
z=callfemm(['ho_blockintegral(' , num(ptype) , ')' ]);
        
