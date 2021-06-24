% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mo_resize(nWidth,nHeight)
	callfemm(['mo_resize(', numc(nWidth), num(nHeight), ')' ]);

