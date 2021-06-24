% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mi_drawpolygon(p)
	mi_drawpolyline(p);
	n=length(p);
	mi_drawline(p(1,1),p(1,2),p(n,1),p(n,2));

