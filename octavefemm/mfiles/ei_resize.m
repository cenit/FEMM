% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ei_resize(nWidth,nHeight)
	callfemm(['ei_resize(', numc(nWidth), num(nHeight), ')' ]);

