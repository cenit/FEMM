% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function hi_resize(nWidth,nHeight)
	callfemm(['hi_resize(', numc(nWidth), num(nHeight), ')' ]);

