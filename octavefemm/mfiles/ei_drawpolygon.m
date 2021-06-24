% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ei_drawpolygon(p)
	ei_drawpolyline(p);
	n=length(p);
	ei_drawline(p(1,1),p(1,2),p(n,1),p(n,2));

