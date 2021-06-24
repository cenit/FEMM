% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mi_resize(nWidth,nHeight)
	callfemm(['mi_resize(', numc(nWidth), num(nHeight), ')' ]);

