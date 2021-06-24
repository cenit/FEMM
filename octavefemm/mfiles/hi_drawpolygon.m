% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function hi_drawpolygon(p)
	hi_drawpolyline(p);
	n=length(p);
	hi_drawline(p(1,1),p(1,2),p(n,1),p(n,2));

