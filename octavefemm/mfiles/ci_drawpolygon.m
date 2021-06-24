% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ci_drawpolygon(p)
	ci_drawpolyline(p);
	n=length(p);
	ci_drawline(p(1,1),p(1,2),p(n,1),p(n,2));

