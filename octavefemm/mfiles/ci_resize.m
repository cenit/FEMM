% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ci_resize(nWidth,nHeight)
	callfemm(['ci_resize(', numc(nWidth), num(nHeight), ')' ]);

