% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ei_drawline(p1,p2,p3,p4)
if (nargin==4)
	x1=p1; y1=p2; x2=p3; y2=p4;
elseif (nargin==2)
	x1=p1(1); y1=p1(2); x2=p2(1); y2=p2(2);
elseif (nargin==1)
	x1=p1(1,1); y1=p1(1,2); x2=p1(2,1); y2=p1(2,2);
end
	ei_addnode(x1,y1);
	ei_addnode(x2,y2);
	ei_addsegment(x1,y1,x2,y2);

