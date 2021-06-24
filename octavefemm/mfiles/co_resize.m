% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function co_resize(nWidth,nHeight)
	callfemm(['co_resize(', numc(nWidth), num(nHeight), ')' ]);

