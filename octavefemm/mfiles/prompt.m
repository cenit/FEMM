% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=prompt(msg)
z=callfemm(['prompt(' , quote(msg) , ')' ]);
        
